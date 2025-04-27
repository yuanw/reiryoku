{
  description = "my keeb";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
    flake-root.url = "github:srid/flake-root";
    treefmt-nix.url = "github:numtide/treefmt-nix";
    treefmt-nix.inputs.nixpkgs.follows = "nixpkgs";
   
    # must be git not github for submodules
    bastardkb_firmware = {
      url =
        "git+https://github.com/Bastardkb/bastardkb-qmk?ref=bkb-master&submodules=1&shallow=1";
      flake = false;
    };

    svalboard_firmware = {
      url = "git+https://github.com/svalboard/vial-qmk?ref=vial&submodules=1&shallow=1";
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
  
        {
          packages.drawer =pkgs.python3Packages.callPackage ./nix/keymap-drawer.nix {};
           
          treefmt.config = {
            inherit (config.flake-root) projectRootFile;
            package = pkgs.treefmt;

            programs.nixpkgs-fmt.enable = true;

          };
          packages.shijushichishi = pkgs.stdenv.mkDerivation rec {
            name = "svalboard.uf2";
            src = inputs.svalboard_firmware;

            nativeBuildInputs = [ pkgs.qmk ];
            buildInputs = with pkgs; [
              cacert
            ];
            # this allows us to not need the .git folder
            SKIP_VERSION = "1";
            SSL_CERT_FILE = "${pkgs.cacert}/etc/ssl/certs/ca-bundle.crt";

            postUnpack = ''
              ln -s ${
                ./svalboard/yuanw/.
              } $sourceRoot/keyboards/svalboard/keymaps/yuanw
            '';

            buildPhase = ''
              make VERBOSE=1 svalboard/trackball/pmw3360/right:vial
              make VERBOSE=1 svalboard/trackball/pmw3360/left:vial
            '';

            installPhase = ''
              ls
              mkdir $out
              mkdir -p $out/share
              mv *.uf2 $out/share
            '';
          };
          packages.firmware = pkgs.stdenv.mkDerivation rec {
            name = "firmware.uf2";
            src = inputs.bastardkb_firmware;

            nativeBuildInputs = [ pkgs.qmk ];
            buildInputs = with pkgs; [
              cacert
              (python3.withPackages (ps: [ ps.pyyaml ]))
            ];

            postUnpack = ''
              ln -s ${
                ./bastardkb/yuanw/.
              } $sourceRoot/keyboards/bastardkb/charybdis/3x5/keymaps/yuanw
              ln -s ${./process.py} $sourceRoot/process.py
            '';

            # this allows us to not need the .git folder
            SKIP_VERSION = "1";
            SSL_CERT_FILE = "${pkgs.cacert}/etc/ssl/certs/ca-bundle.crt";

            buildPhase = ''
              make bastardkb/charybdis/3x5:yuanw
              ${pkgs.qmk}/bin/qmk  -v c2json -kb bastardkb/charybdis/3x5 -km yuanw ./keyboards/bastardkb/charybdis/3x5/keymaps/yuanw/keymap.c > reiryoku.json
              mkdir $out
              mkdir -p $out/share
            '';

            installPhase = ''
              mv bastardkb_charybdis_3x5_yuanw.uf2 $out/share
              mv reiryoku.json $out/share
            '';
          };
          packages.flash = pkgs.writeScriptBin "reiryoku-flash" ''
            cd ${inputs.qmk_firmware}
             ${pkgs.qmk}/bin/qmk flash ${config.packages.firmware}/share/bastardkb_charybdis_3x5_yuanw.uf2
           '';
           
          packages.draw = pkgs.writeShellApplication {
            name = "reiryoku-draw";
            runtimeInputs = with pkgs;
              [
                config.packages.drawer
                (python3.withPackages (ps: [ ps.pyyaml ]))
              ];
            text = ''
              keymap parse -c 10 -q ${config.packages.firmware}/share/reiryoku.json  > reiryoku.yaml
               sed -i -E "s/LAYOUT_charybdis_3x5/LAYOUT/g" reiryoku.yaml
              keymap draw reiryoku.yaml > reiryoku.svg
               python process.py
               keymap draw output.yaml > reiryoku.svg
            '';
          };

          # Default shell.
          devShells.default = pkgs.mkShell {
            buildInputs = with pkgs; [
              (python3.withPackages (ps: [ ps.pyyaml ]))
              #config.packages.draw
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
          apps.draw = {
            type = "app";
            program = config.packages.draw;
          };
          apps.default = config.apps.flash;
        };
    };
}
