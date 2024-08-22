#!/bin/bash

dir="ctemplate"

if [[ -d "$dir" ]]; then
    echo "ctemplate already exist"
else
    git clone https://github.com/OlafvdSpek/ctemplate.git
    sudo apt install autoconf automake libtool
    cd ctemplate
    ./autogen.sh
    ./configure
    make
    sudo make install
    cd ..
fi

echo "please do: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${PWD}/ctemplate/.libs"


