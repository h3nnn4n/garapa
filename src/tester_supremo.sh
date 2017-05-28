#!/bin/bash

TESTF=tests_folder
TESTS="FBDP NDP KBR CMA HA LELmark ALL"

if [[ ! -d "$TESTF" ]]; then
    mkdir $TESTF
    echo Created $TESTF
fi

for name in $TESTS; do
    path=$TESTF/$name
    if [[ ! -d "$path" ]]; then
        mkdir $path
        echo mkdir $path
    fi

    make clean -s
    cmd="make -j -s CFLAGS=-D$name"
    $cmd 2> /dev/null
    echo $cmd

    mv main "main_$name"

    now=$(date "+%d_%m_%H_%M_%S")
    (./"main_$name" tetris.gb > $path/$now) & disown %
    echo "./"main_$name" tetris.gb > $path/$now"
done

