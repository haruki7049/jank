{
  pkgs ? import (builtins.fetchTarball {
    url = "https://github.com/NixOS/nixpkgs/archive/75b209227dff3cbfac19f510a62f9446c92beac4.tar.gz";
    sha256 = "166varzdamn2kkw5zslarzl6026q87wjnss1hlhdsq89hwvfgd2d";
  }) { },
}:

let
  lib = pkgs.lib;

  # Deps
  boost = pkgs.callPackage ./nix/boost.nix { };
  boehmgc = pkgs.callPackage ./nix/boehmgc.nix { };
  libzippp = pkgs.callPackage ./nix/libzippp.nix { };
  openssl = pkgs.callPackage ./nix/openssl.nix { };
in

pkgs.llvmPackages_19.stdenv.mkDerivation {
  pname = "jank-lang";
  version = "dev";
  src = lib.cleanSource ./.;

  nativeBuildInputs = with pkgs; [
    llvm_19
    cmake
    git
    ninja
  ];

  patchPhase = ''
    patchShebangs --host "${pkgs.bash}" bin/build-pch
  '';

  buildInputs = [
    boost
    boehmgc
    libzippp
    openssl
    pkgs.libcxx
    pkgs.doctest
    pkgs.double-conversion
    pkgs.readline
    pkgs.libzip
    pkgs.immer
    pkgs.cli11
    pkgs.magic-enum
    pkgs.fmt
    pkgs.llvmPackages_19.clang-unwrapped
    pkgs.llvmPackages_19.clangUseLLVM
  ];

  cmakeFlags = [
    "-GNinja"
    "-DDMAKE_BUILD_TYPE=Debug"
    "-Djank_tests=on"
  ];
}
