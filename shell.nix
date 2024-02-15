let
  pkgs = (import <nixpkgs> {}).pkgsCross.x86_64-embedded;
  native = import <nixpkgs> {};
in
  pkgs.pkgsStatic.callPackage ({ mkShell, gcc, nasm }: mkShell {
    nativeBuildInputs = [
      gcc
      nasm
    ];
    packages = with native; [
      qemu_kvm
    ];
  }) {}
