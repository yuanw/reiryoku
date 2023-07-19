{
  description = "my keeb";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
    mission-control.url = "github:Platonic-Systems/mission-control";
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
        inputs.mission-control.flakeModule
      ];
      perSystem =
        { pkgs
        , lib
        , config
        , system
        , ...
        }:
        let
          inherit (inputs.poetry2nix.legacyPackages.${system}) defaultPoetryOverrides mkPoetryApplication;
        in {
          legacyPackages = pkgs;


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
            # src = pkgs.fetchFromGitHub {
            #   owner = "Bastardkb";
            #   repo = "bastardkb-qmk";
            #   rev = "d77f807d78bf19b6fd834b049103e127b5c760a9";
            #   # sha256 = "0000000000000000000000000000000000000000000000000000";
            #   sha256 = "AMKeHg5PaxMkzEDFFFck0w8lvqFMRdDGrgXDy4B6NDk=";
            #   fetchSubmodules = true;
            # };
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
            # outputs = [ "out" "hex" ];

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
          # Default shell.
          devShells.default = pkgs.mkShell {
            name = "haskell-template";
            # See https://haskell.flake.page/devshell#composing-devshells
            inputsFrom = [
              config.flake-root.devShell
              config.mission-control.devShell
              config.treefmt.build.devShell
            ];
          };
          packages.default = config.packages.firmware;
          #   flash = pkgs.writeScriptBin "reiryoku-flash" ''
          #     cd ${qmk_firmware}
          #     ${pkgs.qmk}/bin/qmk flash ${firmware}/share/bastardkb_charybdis_3x5_v2_splinky_3_yuanw.uf2
          #   '';
          #   flash-script = (pkgs.writeScript "qmk-flash" ''
          #     cd ${qmk_firmware}
          #     ${pkgs.qmk}/bin/qmk flash ${firmware}/share/bastardkb_charybdis_3x5_v2_splinky_3_yuanw.uf2
          #   '');

          # in
          # rec {
          #   packages.firmware = pkgs.symlinkJoin {
          #     name = "reiryoku-firmware";
          #     paths = [ firmware flash ];
          #   };
          #   packages.drawer = drawer;
          #   packages.flash-script = flash-script;
          #   apps.flash = {
          #     type = "app";
          #     program = "${packages.flash-script}";
          #   };
          #   devShells.default = pkgs.mkShell {
          #     buildInputs = with pkgs; [
          #       nixpkgs-fmt
          #       (python3.withPackages (ps: [ ps.pyyaml ]))
          #     ];
          #   };
          #   # Defaults =======================

          #   packages.default = packages.firmware;
          #   apps.default = apps.flash;
        };
    };
}
