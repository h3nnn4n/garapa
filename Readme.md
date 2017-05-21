Garapa
======================
A Game Boy emulator as tasty as the Brazilian Garapa
--------------

Garapa is GameBoy Emulator written in C. The main objective of the emulator was to be a leaning experiece for me. I am trying to make it as accurate as possible, using several test roms available on the internet, namely [blargg's](http://gbdev.gg8.se/files/roms/blargg-gb-tests/) and [gekkio's](https://github.com/Gekkio/mooneye-gb/tree/master/tests).
The emulator is very accurate so far to what has been measured. APU part is missing and PPU renders incorrectly, even though it is cycle accurate.

I would like to thank mehcode and gekkio for all the help and support they gave me, also for everyone else at [EmuDev's slack](https://slofile.com/slack/emudev).

TODO list
=========

 - [ ] Savestates
 - [ ] APU emulation (Audio Processing Unit)
 - [ ] Make the PPU render correctly (it passes (most) tests but draws incorrectly)
 - [ ] Pass all of blargg's tests
 - [ ] cgb_sound
 - [ ] dmg_sound
 - [ ] interrupt_time
 - [ ] oam_bug
 - [ ] MCB2
 - [ ] MCB3
 - [ ] MCB5

DONE list
=========

 - [x] Pass all of Gekkio's tests
 - [x] MCB1
 - [x] cpu_instrs
 - [x] halt_bug.gb
 - [x] instr_timing
 - [x] mem_timing
 - [x] mem_timing-2
 - [x] SDL2
 - [x] Scalling
 - [x] Playable Tetris


License
=======
See `LICENSE` for more info.
