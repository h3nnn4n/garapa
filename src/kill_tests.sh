#!/bin/bash

TESTS="FBDP NDP KBR CMA HA LELmark"

for name in $TESTS; do
    killall -9 main_$name
done
