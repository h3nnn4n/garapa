Garapa
======================
A Game Boy emulator as tasty as the Brazilian Garapa
--------------

Garapa is GameBoy Emulator written in C. The main objective of the emulator was to be a leaning experiece for me. I am trying to make it as accurate as possible, using several test roms available on the internet,
namely [blargg's](http://gbdev.gg8.se/files/roms/blargg-gb-tests/) and [gekkio's](https://github.com/Gekkio/mooneye-gb/tree/master/tests). Last time I checked, Garapa passed all of gekkio's tests and failed only on
interrupt timing for blargg's.
The emulator is very accurate so far to what has been measured. The APU is fully functional and so is the PPU. In some edge cases the PPU draws incorrectly, even though it is cycle accurate.

On the accuracy side the emulator is pretty good. On the other hand... The usability side is pretty much non existent and should stay as is.


The master branch stores the pure emulator. The branch

Garapa is divided in several subprojects:

 * [master](https://github.com/h3nnn4n/garapa/) - The standalone emulator;
 * [gabate](https://github.com/h3nnn4n/garapa/tree/gabate) - Genetic Algorithm Based Agent for TEtris is a Tetris AI that learns how to play it from scratch. It runs on top of garapa;
 * [gabate_tests](https://github.com/h3nnn4n/garapa/tree/gabate_tests) - Same as above, but it is a frozen version that was used for a paper;
 * [julia_api](https://github.com/h3nnn4n/garapa/tree/julia_api) - The full emulator with a simple julia api. For now it has a simpler version of gabate in julia being written.


Acknowledgments
===============

I would like to thank mehcode and gekkio for all the help and support they gave me, also for everyone else at [EmuDev's slack](https://slofile.com/slack/emudev).

License
=======
See `LICENSE` for more info.
