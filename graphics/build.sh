#!/bin/bash

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root. Re-running with sudo..."
   exec sudo "$0" "$@"
fi

rm -r build
mkdir build
cmake -S . -B ./build
cmake --build ./build
