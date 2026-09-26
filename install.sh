#!/usr/bin/bash

set -e

cmake -S . -B build
cmake --build build
echo "Successful!"

read -p "Do you also want to install globally (requires sudo)? [y/N] " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    sudo cmake --install build
fi

