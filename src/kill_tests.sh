#!/bin/bash

TESTS="FBDP NDP KBR CMA HA LELmark ALL"

for name in $TESTS; do
    killall -9 main_$name
done
