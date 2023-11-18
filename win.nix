{
  pkgs,
  stdenv,
  version ? null,
  vc,
  ...
}:
with pkgs; let
  xwin = rustPlatform.buildRustPackage rec {
    pname = "xwin";
    version = "0.3.1";
    src = fetchgit {
      url = "https://github.com/Jake-Shadle/xwin.git";
      rev = "6768d2ecf4a54a900ee456b3d6a9149ec781f22d";
      hash = "sha256-W2sNmEXWwuzLDZg3IrOvOL81K2zrqjUzqfaifqeTtqs=";
    };
    cargoLock.lockFile = "${src}/Cargo.lock";
    doCheck = false;
  };
  manifest = "17";

  xwin-cache = stdenv.mkDerivation rec {
    pname = "xwin-cache";
    version = "${manifest}";

    phases = ["installPhase" "fixupPhase"];

    outputHashAlgo = "sha256";
    outputHash = "sha256-SzOBDfTfY7xNhhVZp0Nm6ZBWKENc09nnAlV7H16dVT0=";
    outputHashMode = "recursive";
    nativeBuildInputs = [xwin];

    common_args = "--manifest-version ${manifest} --cache-dir $out/cache --accept-license";
    installPhase = ''
      export common_args=(${common_args})
      xwin ''${common_args[@]} download
      xwin ''${common_args[@]} unpack
    '';

    fixupPhase = ''
      xwin ''${common_args[@]} splat --include-debug-libs --include-debug-symbols
    '';
  };
in {
  msvc = stdenv.mkDerivation {
    pname = "msvc";
    version = "${manifest}";
    phases = ["installPhase"];
    buildInputs = [xwin-cache];
    installPhase = ''
      splat_dir="${xwin-cache}/cache/splat"
      includes=(crt/include sdk/include/{shared,ucrt,um,winrt} sdk/include/cppwinrt/winrt)
      libs=({crt/lib,sdk/lib/ucrt,sdk/lib/um}/x86_64)

      mkdir -p $out/include
      for i in "''${includes[@]}"; do
        ln -sf "$splat_dir/$i"/* $out/include/
      done

      mkdir -p $out/lib
      for l in "''${libs[@]}"; do
        ln -sf "$splat_dir/$l"/* $out/lib/
      done

      ln -sf $out/lib/opengl32.lib $out/lib/OpenGL32.lib
    '';
  };
  sfml = stdenv.mkDerivation {
    name = "SFML-${version}-windows-vc-${vc}";
    phases = ["unpackPhase" "installPhase"];
    nativeBuildInputs = [unzip];

    src = fetchurl {
      url = "https://www.sfml-dev.org/files/SFML-${version}-windows-vc${vc}-64-bit.zip";

      hash = "sha256-PoB/foENY1ft41rNl2FfH+Z7FwKP89PZRjKK+2EEq4Y=";
      # hash = "sha256-9vH2Ouau5c/yCNiEM1psj2+Df/BIgsdSWcZVf0md5WY=";
      # hash = "sha256-PoB/foENY1ft41rNl2FfH+Z7FwKP89PZRjKK+2EEq4Y=";
    };

    # unpackPhase = ''
    #   unzip "${name}-64-bit.zip"
    # '';
    installPhase = ''
      mkdir -p $out/lib64
      mv {bin,include} $out

      ## This is needed since bintools hook won't add libs to NIX_LDFLAGS
      ## if there's no $out/lib/lib* and the same doesn't occurs in lib64
      ## https://github.com/NixOS/nixpkgs/blob/5ba549eafcf3e33405e5f66decd1a72356632b96/pkgs/build-support/bintools-wrapper/setup-hook.sh#L16-L29
      mv lib/* $out/lib64
    '';
  };

  inherit xwin;
}
