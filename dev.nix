{
  pkgs,
  llvmPackages,
  stdenv ? llvmPackages.stdenv,
  app,
  extraPackages ? [],
  ...
}: let
  mkShell = pkgs.mkShell.override {inherit stdenv;};
in
  mkShell rec {
    configurePhase = "cmake -S . -B ";
    buildPhase = "make -j $NIX_BUILD_CORES";
    inputsFrom = [app];

    # Add dev deps from app
    nativeBuildInputs = pkgs.lib.attrsets.mapAttrsToList (n: v: v) app.passthru;
    shellHook = ''
      export PRJ_ROOT=$PWD
    '';
    packages = with pkgs;
      [
        clang-tools
        cmake-language-server
        fmt

        (writeShellScriptBin "bul" ''
          cd $PRJ_ROOT
          ${configurePhase} build/linux && cd build/linux && ${buildPhase}
        '')
        (writeShellScriptBin "buw" ''
          export NIX_LDFLAGS="$(sed 's/-rpath [^ ]\+ //g' <<< $NIX_LDFLAGS)"
          # export NIX_LDFLAGS=""
          cd $PRJ_ROOT
          ${configurePhase} build/windows -DCMAKE_TOOLCHAIN_FILE=$PRJ_ROOT/TC-win.cmake && cd build/windows && ${buildPhase}
        '')
      ]
      ++ extraPackages;
  }
