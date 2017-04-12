#!/bin/bash

BIN=./main
ROM=tetris.gb

for n in `seq 1 $1` ; do
    $BIN $ROM > "log_${n}.txt" &
done
