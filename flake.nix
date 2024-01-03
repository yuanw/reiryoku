{
  description = "my keeb";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
    flake-root.url = "github:srid/flake-root";
    treefmt-nix.url = "github:numtide/treefmt-nix";
    treefmt-nix.inputs.nixpkgs.follows = "nixpkgs";
    keymap_drawer = {
      url = "github:caksoylar/keymap-drawer";
      flake = false;
    };
    poetry2nix = {
      url = "github:nix-community/poetry2nix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
    # must be git not github for submodules
    qmk_firmware = {
      url =
        "git+https://github.com/Bastardkb/bastardkb-qmk?ref=bkb-master&submodules=1&shallow=1";
      flake = false;
    };
  };

  outputs = inputs:
    inputs.flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [
        "aarch64-darwin"
        "aarch64-linux"
        "x86_64-darwin"
        "x86_64-linux"
      ];
      imports = [
        # inputs.haskell-flake.flakeModule
        inputs.treefmt-nix.flakeModule
        inputs.flake-root.flakeModule
      ];
      perSystem =
        { pkgs
        , lib
        , config
        , system
        , ...
        }:
        let

        inherit (inputs.poetry2nix.lib.mkPoetry2Nix { inherit pkgs; }) defaultPoetryOverrides  mkPoetryApplication;
        in
        {
          packages.drawer = mkPoetryApplication {
            projectDir = inputs.keymap_drawer;
            overrides = defaultPoetryOverrides.extend
              (self: super: {
                deptry = super.deptry.overridePythonAttrs
                  (
                    old: {
                      buildInputs = (old.buildInputs or [ ]) ++ [ super.poetry ];
                    }
                  );
              });
          };
          treefmt.config = {
            inherit (config.flake-root) projectRootFile;
            package = pkgs.treefmt;

            programs.nixpkgs-fmt.enable = true;

          };
          packages.firmware = pkgs.stdenv.mkDerivation rec {
            name = "firmware.uf2";
            src = inputs.qmk_firmware;

            nativeBuildInputs = [ pkgs.qmk ];
            buildInputs = with pkgs; [
              cacert
              (python3.withPackages (ps: [ ps.pyyaml ]))
            ];

            postUnpack = ''
              ln -s ${
                ./qmk/yuanw/.
              } $sourceRoot/keyboards/bastardkb/charybdis/3x5/keymaps/yuanw
              ln -s ${./process.py} $sourceRoot/process.py
            '';

            # this allows us to not need the .git folder
            SKIP_VERSION = "1";
            SSL_CERT_FILE = "${pkgs.cacert}/etc/ssl/certs/ca-bundle.crt";

            buildPhase = ''
              make bastardkb/charybdis/3x5/v2/splinky_3:yuanw
              ${pkgs.qmk}/bin/qmk  -v c2json -kb bastardkb/charybdis/3x5/v2/splinky_3 -km yuanw ./keyboards/bastardkb/charybdis/3x5/keymaps/yuanw/keymap.c > reiryoku.json
              mkdir $out
              mkdir -p $out/share
            '';

            installPhase = ''
              mv bastardkb_charybdis_3x5_v2_splinky_3_yuanw.uf2 $out/share
              mv reiryoku.json $out/share
            '';
          };
          packages.flash = pkgs.writeScriptBin "reiryoku-flash" ''
            cd ${inputs.qmk_firmware}
             ${pkgs.qmk}/bin/qmk flash ${config.packages.firmware}/share/bastardkb_charybdis_3x5_v2_splinky_3_yuanw.uf2
          '';
          # Default shell.
          devShells.default = pkgs.mkShell {
            buildInputs = with pkgs; [
              (python3.withPackages (ps: [ ps.pyyaml ]))
            ];

            # See https://haskell.flake.page/devshell#composing-devshells
            inputsFrom = [
              config.flake-root.devShell
              config.treefmt.build.devShell
            ];
          };
          packages.default = config.packages.firmware;
          apps.flash = {
            type = "app";
            program = config.packages.flash;
          };
          apps.default = config.apps.flash;
      };
    };
}
