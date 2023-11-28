{
  pkgs,
  llvmPackages,
  stdenv ? llvmPackages.stdenv,
  ...
}: let
  version = "2.5.1";
  vc = "15";

  win = import ./win.nix {
    inherit pkgs stdenv version vc;
  };

  base-app = with pkgs; {
    pname = "sfml-learn";
    version = "1.0";
    src = ./.;
    nativeBuildInputs = [cmake llvmPackages.lld win.xwin];
    buildInputs = [];
    installPhase = "
      mkdir -p $out/bin
      mv bin/* $out/bin
    ";
  };
  windows-build = with win;
    stdenv.mkDerivation
    (base-app
      // {
        preConfigurePhases = ["sfmlInstall"];
        nativeBuildInputs = base-app.nativeBuildInputs ++ [msvc];
        # dontInstall = true;

        sfmlInstall = ''
          rm -rf libs/windows &&
          mkdir -p libs/windows/SFML-${version} &&
          ln -sf ${sfml}/include libs/windows/SFML-${version}/include &&
          ln -sf ${sfml}/lib64 libs/windows/SFML-${version}/lib &&
          ln -sf ${sfml}/bin libs/windows/SFML-${version}/bin &&

          mkdir -p libs/windows/MSVC${msvc.version} &&
          ln -sf ${msvc}/include libs/windows/MSVC${msvc.version}/include &&
          ln -sf ${msvc}/lib libs/windows/MSVC${msvc.version}/lib
        '';
        configurePhase = ''
          export NIX_LDFLAGS="$(sed 's/-rpath [^ ]\+ //g' <<< $NIX_LDFLAGS)"
          cmake -S . -B build/windows -DCMAKE_TOOLCHAIN_FILE=./msvc/TC-win.cmake
        '';
        buildPhase = ''
          cd build/windows
          make -j $NIX_BUILD_CORE
        '';

        # Dev deps from windows build
        passthru = {
          sfml-win = sfml;
        };
      });
  linux-build = with pkgs;
    stdenv.mkDerivation
    (base-app
      // {
        preConfigurePhases = ["sfmlInstall"];
        buildInputs =
          base-app.buildInputs
          ++ [
            sfml
            fmt
            gperftools
            # freetype
            # fontconfig
            # xorg.xorgproto
            # xorg.libX11
            # xorg.libXt
            # xorg.libXft
            # xorg.libXext
            # xorg.libSM
            # xorg.libICE
            # xorg.libXrandr
            # udev
            # libglvnd
            # flac
            # libogg
            # libvorbis
            # openal
            # libevent
          ];
        # dontInstall = true;

        sfmlInstall = ''
          rm -rf libs/linux &&
          mkdir -p libs/linux/SFML-${version} &&
          ln -sf ${sfml}/include libs/linux/SFML-${version}/include &&
          ln -sf ${sfml}/lib libs/linux/SFML-${version}/lib
        '';
        configurePhase = ''
          cmake -S . -B build/linux
        '';
        buildPhase = ''
          cd build/linux
          make -j $NIX_BUILD_CORES
        '';

        # Dev deps from linux build
        passthru = {
          sfml-linux = sfml;
        };
      });
in
  with pkgs; {
    linux = linux-build;
    windows = windows-build;
    multi = symlinkJoin {
      name = "sfml-learn-linux_and_windows";
      nativeBuildInputs = linux-build.nativeBuildInputs ++ windows-build.nativeBuildInputs;
      buildInputs = linux-build.nativeBuildInputs ++ windows-build.nativeBuildInputs;
      paths = [
        linux-build
        windows-build
      ];

      # Merge dev deps from both builds
      passthru = linux-build.passthru // windows-build.passthru;
    };
  }
# with pkgs;
#   stdenv.mkDerivation {
#     pname = "sfml-learn";
#     version = "1.0";
#     src = ./.;
#     nativeBuildInputs =
#       [cmake llvmPackages.bintools]
#       ++ (
#         if windows
#         then [winSdk]
#         else []
#       );
#     buildInputs = [
#       sfml
#     ];
#     configurePhase =
#       (
#         if linux
#         then ''
#           cmake -S . -B build/linux
#         ''
#         else ""
#       )
#       + (
#         if windows
#         then ''
#           cmake -S . -B build/windows -DCMAKE_TOOLCHAIN_FILE=./msvc/TC-win.cmake
#         ''
#         else ""
#       );
#     buildPhase =
#       (
#         if linux
#         then ''
#           cd build/linux
#           make -j $NIX_BUILD_CORES
#         ''
#         else ""
#       )
#       + (
#         if windows
#         then ''
#           cd build/windows
#           export NIX_LDFLAGS="$(sed 's/-rpath [^ ]\+ //g' <<< $NIX_LDFLAGS)"
#           make -j $NIX_BUILD_CORES
#         ''
#         else ""
#       );
#   }
