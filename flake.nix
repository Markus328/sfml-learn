{
  inputs = {
    nixpkgs = {
      url = "github:nixos/nixpkgs/nixpkgs-unstable";
    };
    flake-utils = {
      url = "github:numtide/flake-utils";
    };
    nixGL = {
      url = "github:guibou/nixGL";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };
  outputs = {
    nixpkgs,
    flake-utils,
    nixGL,
    ...
  }:
    flake-utils.lib.eachDefaultSystem (system: let
      pkgs = import nixpkgs {
        inherit system;
      };
      nixgl = nixGL.packages.${system}.default;

      llvmPackages = pkgs.llvmPackages_16;
      stdenv = llvmPackages.stdenv;

      mkShell = pkgs.mkShell.override {inherit stdenv;};

      sfml-learn = {
        linux = (import ./sfml-learn.nix {
          inherit pkgs llvmPackages;
          linux = true;
          windows = false;
        }).linux;
        windows = (import ./sfml-learn.nix {
          inherit pkgs llvmPackages;
          linux = false;
          windows = true;
        }).windows;
        multi = (import ./sfml-learn.nix {
          inherit pkgs llvmPackages;
        }).multi;
      };
      shell = rec {
        configurePhase = "mkdir -p $PRJ_ROOT/build && cd $PRJ_ROOT/build && cmake ..";
        buildPhase = "cd $PRJ_ROOT/build && make -j $NIX_BUILD_CORES";
        inputsFrom = [sfml-learn];
        packages = with pkgs; [
          clang-tools
          cmake-language-server
          (writeShellScriptBin "bu" ''${configurePhase} && ${buildPhase}'')
        ];
        shellHook = ''
          export PRJ_ROOT=$PWD
        '';
      };
    in rec {
      defaultPackage = sfml-learn.windows;
      packages = {
        default = defaultPackage;
        linux = sfml-learn.linux;
        windows = sfml-learn.windows;
        genericLinux = pkgs.writeShellScriptBin "sfml-learn" ''
          ${nixgl}/bin/nixGL ${defaultPackage}/bin/sfml-learn $@
        '';
      };
      devShells = {
        test = import ./dev.nix {
          inherit pkgs llvmPackages stdenv;
          app = sfml-learn.multi;
        };
        windows = import ./dev.nix {
          inherit pkgs llvmPackages stdenv;
          app = sfml-learn.windows;
        };
        linux = import ./dev.nix {
          inherit pkgs llvmPackages stdenv;
          app = sfml-learn.linux;
          extraPackages = [pkgs.gdb];
        };
      };
      apps = {
        default = flake-utils.lib.mkApp {drv = defaultPackage;};
        genericLinux = flake-utils.lib.mkApp {drv = packages.genericLinux;};
      };
    });
}
