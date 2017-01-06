Here comes dat gameboi
======================
Oh shit waddup
--------------

Here comes dat gameboi is GameBoy Emulator written in C. The main objective of the emulator was to be a leaning experiece for me. I am trying to make it as accurate as possible, using several test roms available on the internet, namely [blargg's](http://gbdev.gg8.se/files/roms/blargg-gb-tests/) and [gekkio's](https://github.com/Gekkio/mooneye-gb/tree/master/tests).
The emulator still is a WIP ( Work In Progress ) and it is somewhat messy, but I am working on it on a daily basis.

I would like to thank mehcode and gekkio for all the help and support they gave me, also for everyone else at [EmuDev's slack](https://slofile.com/slack/emudev).

TODO list
=========

 - [ ] Savestates
 - [ ] APU emulation (Audio Processing Unit)
 - [ ] Make the PPU render correctly (it passes (most) tests but draws incorrectly)

 - [x] Playable Tetris

 - [x] SDL2
  - [x] Scalling
  - [ ] Maybe filters

 - [ ] Pass all of blargg's tests
  - [ ] cgb_sound
  - [x] cpu_instrs
  - [ ] dmg_sound
  - [ ] halt_bug.gb
  - [x] instr_timing
  - [ ] interrupt_time
  - [x] mem_timing
  - [x] mem_timing-2
  - [ ] oam_bug

 - [x] Pass Gekkio's timer tests
  - [x] div_write.gb
  - [x] rapid_toggle.gb
  - [x] tim00.gb
  - [x] tim00_div_trigger.gb
  - [x] tim01.gb
  - [x] tim01_div_trigger.gb
  - [x] tim10.gb
  - [x] tim10_div_trigger.gb
  - [x] tim11.gb
  - [x] tim11_div_trigger.gb
  - [x] tima_reload.gb
  - [x] tima_write_reloading.gb
  - [x] tma_write_reloading.gb

 - [ ] Pass Gekkio's gpu tests
  - [x] hblank_ly_scx_timing-GS.gb
  - [x] intr_1_2_timing-GS.gb
  - [x] intr_2_0_timing.gb
  - [x] intr_2_mode0_timing.gb
  - [ ] intr_2_mode0_timing_sprites.gb
  - [x] intr_2_mode3_timing.gb
  - [x] intr_2_oam_ok_timing.gb
  - [ ] stat_irq_blocking.gb
  - [x] vblank_stat_intr-GS.gb

 - [ ] Pass Gekkio's bits tests
  - [x] mem_oam.gb
  - [x] reg_f.gb
  - [ ] unused_hwio-GS.gb

 - [ ] Pass Gekkio's general tests
  - [x] add_sp_e_timing.gb
  - [x] boot_regs-dmg.gb
  - [ ] call_cc_timing.gb
  - [ ] call_cc_timing2.gb
  - [x] call_timing.gb
  - [x] call_timing2.gb
  - [x] di_timing-GS.gb
  - [x] div_timing.gb
  - [ ] ei_timing.gb
  - [x] halt_ime0_ei.gb
  - [x] halt_ime0_nointr_timing.gb
  - [x] halt_ime1_timing.gb
  - [x] halt_ime1_timing2-GS.gb
  - [ ] if_ie_registers.gb
  - [ ] intr_timing.gb
  - [x] jp_cc_timing.gb
  - [x] jp_timing.gb
  - [x] ld_hl_sp_e_timing.gb
  - [x] oam_dma_restart.gb
  - [x] oam_dma_start.gb
  - [x] oam_dma_timing.gb
  - [x] pop_timing.gb
  - [x] push_timing.gb
  - [ ] rapid_di_ei.gb
  - [x] ret_cc_timing.gb
  - [x] ret_timing.gb
  - [ ] reti_intr_timing.gb
  - [x] reti_timing.gb
  - [x] rst_timing.gb

 - [ ] MCBs
  - [x] NONE :)
  - [x] MCB1
  - [ ] MCB2
  - [ ] MCB3
  - [ ] MCB5

License
=======
See `LICENSE` for more info.
