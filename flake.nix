{
  description = "WEED";

  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";

  outputs = {
    self,
    nixpkgs,
    ...
  } @ inputs: let
    inherit (nixpkgs) lib;

    eachSystem = fn:
      lib.foldl' (
        acc: system:
          lib.recursiveUpdate
          acc
          (lib.mapAttrs (_: value: {${system} = value;}) (fn system))
      ) {}
      lib.platforms.linux;
    perSystem = system:
      let
        pkgs = import nixpkgs { inherit system; };
      in {
        devShell = pkgs.mkShell {
          buildInputs = with pkgs; [
            gcc
            gdb
            gnumake
          ];
          shellHook = ''
            export CPATH=$(pwd)
          '';
        };
      };
  in eachSystem perSystem;
}
