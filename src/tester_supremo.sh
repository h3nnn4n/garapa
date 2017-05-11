#!/bin/bash

TESTF=tests_folder
TESTS="FBDP NDP KBR CMA HA LELmark"

if [[ ! -d "$TESTF" ]]; then
    mkdir $TESTF
    echo Created $TESTF
fi

for name in $TESTS; do
    path=$TESTF/$name
    if [[ ! -d "$path" ]]; then
        mkdir $path
        echo Created $path
    fi

    make clean -s
    cmd="make -j -s CFLAGS=-D$name"
    $cmd 2> /dev/null

    mv main "main_$name"
    echo "main_$name" compiled

    now=$(date "+%d_%m_%H_%M_%S")
    (./"main_$name" tetris.gb > $path/$now) & disown %
done

