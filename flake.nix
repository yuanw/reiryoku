{
  description = "my keeb";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
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

  outputs = { self, nixpkgs, flake-utils, qmk_firmware, poetry2nix
    , keymap_drawer, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };

        inherit (poetry2nix.legacyPackages.${system}) mkPoetryApplication;

        drawer = mkPoetryApplication { projectDir = keymap_drawer; };
        firmware = pkgs.stdenv.mkDerivation rec {
          name = "firmware.uf2";
          # src = pkgs.fetchFromGitHub {
          #   owner = "Bastardkb";
          #   repo = "bastardkb-qmk";
          #   rev = "d77f807d78bf19b6fd834b049103e127b5c760a9";
          #   # sha256 = "0000000000000000000000000000000000000000000000000000";
          #   sha256 = "AMKeHg5PaxMkzEDFFFck0w8lvqFMRdDGrgXDy4B6NDk=";
          #   fetchSubmodules = true;
          # };
          src = qmk_firmware;
          buildInputs = with pkgs; [ qmk ];

          postUnpack = ''
            ln -s ${
              ./qmk/yuanw/.
            } $sourceRoot/keyboards/bastardkb/charybdis/3x5/keymaps/yuanw
          '';

          buildPhase = ''
            # make bastardkb/charybdis/3x5/v2/splinky_3:yuanw SKIP_GIT=1
            ${pkgs.qmk}/bin/qmk  -v c2json -kb bastardkb/charybdis/3x5/v2/splinky_3 -km yuanw ./keyboards/bastardkb/charybdis/3x5/keymaps/yuanw/keymap.c > reiryoku.json
            ${drawer}/bin/keymap parse -c 10 -q reiryoku.json  > reiryoku.yaml
            sed -i -E "s/LAYOUT_charybdis_3x5/LAYOUT/g" reiryoku.yaml
            ${drawer}/bin/keymap draw reiryoku.yaml > reiryoku.svg
            mkdir -p $out/share
          '';

          installPhase = ''
            # mv bastardkb_charybdis_3x5_v2_splinky_3_yuanw.uf2 $out/
            mv reiryoku.svg $out/share
            mv reiryoku.yaml $out/share
          '';
        };
        flash = pkgs.writeScriptBin "reiryoku-flash" ''
          cd ${qmk_firmware}
          ${pkgs.qmk}/bin/qmk flash ${firmware}/bastardkb_charybdis_3x5_v2_splinky_3_yuanw.uf2
        '';

      in rec {
        packages.firmware = pkgs.symlinkJoin {
          name = "reiryoku-firmware";
          paths = [ firmware flash ];
        };
        packages.drawer = drawer;
        packages.flash = flash;
        apps.flash = {
          type = "app";
          program = "${packages.flash}";
        };

        # Defaults =======================

        packages.default = packages.firmware;
        apps.default = apps.flash;
      });
}
