#!/bin/bash


if [ "$EUID" -ne 0 ] then 
	echo "you should run this script as root"
  exit
fi

set -e

apt update -y 

apt install -y software-properties-common

add-apt-repository -y ppa:ubuntu-toolchain-r/test

apt update -y

apt install -y make=4.3-4.1build1 \
							 cmake=3.22.1-1ubuntu1.22.04.2 \
							 gcc-13=13.1.0-8ubuntu1~22.04 \
							 g++-13=13.1.0-8ubuntu1~22.04 \
							 pkg-config=0.29.2-1ubuntu3 \
							 libconfig++-dev=1.5-0.4build1 \
							 libgtest-dev=1.11.0-3 \
							 nlohmann-json3-dev=3.10.5-2 \
							 clang=1:14.0-55~exp2 \

