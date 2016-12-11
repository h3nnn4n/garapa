i8080 Emulator
==============

This is a i8080 full set emulator, written in C. The original purpose of this code is to run Space Invaders. So far it runs and passes a couple of testers, space invaders is kidna buggy.

Building and Usage
==================

The building process is very simple, just run `make`. The executable `main` contains the main emulation loop, which by default runs the Space Invaders game ROM (not shipped with the code).
One can easily change it to run other ROMs.

Tests
=====

The folder `roms` contain 4 ROMs designed to test the i8080 instruction set. To build and run the test suit run `make test`. This is build a executable file named `main_test` and run it.
The ROM files for testing were collected from [this repository](https://github.com/begoon/i8080-core) and more info about the ROMs can be found there.

License
=======
See `LICENSE` for more info.
