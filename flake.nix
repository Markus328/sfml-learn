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
      nixgl = nixGL.defaultPackage.${system}.default;
      sfml-learn = with pkgs;
        stdenv.mkDerivation {
          pname = "sfml-learn";
          version = "1.0";
          src = ./.;
          nativeBuildInputs = [
            gcc
            cmake
          ];
          buildInputs = [
            sfml
          ];
          buildPhase = "make -j $NIX_BUILD_CORES";
        };
    in rec {
      defaultPackage = sfml-learn;
      nonNixosPackage = pkgs.writeShellScriptBin "sfml-learn" ''
        ${nixgl}/bin/nixGL ${sfml-learn}/bin/sfml-learn $@
      '';
      devShell = sfml-learn;
      defaultApp = flake-utils.lib.mkApp {
        drv = defaultPackage;
      };
    });
}
