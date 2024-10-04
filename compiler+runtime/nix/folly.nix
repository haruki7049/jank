{
  pkgs ? import (builtins.fetchTarball {
    url = "https://github.com/NixOS/nixpkgs/archive/75b209227dff3cbfac19f510a62f9446c92beac4.tar.gz";
    sha256 = "166varzdamn2kkw5zslarzl6026q87wjnss1hlhdsq89hwvfgd2d";
  }) { },
}:

let
  stdenv = pkgs.stdenv;
  cc = stdenv.cc.cc;
in

stdenv.mkDerivation rec {
  pname = "folly";
  version = "jank-fork";

  src = pkgs.fetchFromGitHub {
    owner = "jank-lang";
    repo = pname;
    rev = "086e7f8ee80a613d1cddd52c3d6a2092e11195a1";
    hash = "sha256-r4yjioqOWxRAqWhzR8v0/m31Svlwd/VKV6isMAl5WOE=";
  };

  buildInputs = [
    pkgs.boost
    pkgs.double-conversion
    pkgs.fast-float
    pkgs.libevent
    pkgs.openssl
    pkgs.fmt
    pkgs.glog
    pkgs.zlib
  ];

  nativeBuildInputs = [
    pkgs.cmake
  ];
}
