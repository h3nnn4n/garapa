/******************************************************************************
 * Copyright (C) 2016  Renan S. Silva                                         *
 *                                                                            *
 * This software is provided 'as-is', without any express or implied          *
 * warranty. In no event will the authors be held liable for any damages      *
 * arising from the use of this software.                                     *
 *                                                                            *
 * Permission is granted to anyone to use this software for any purpose,      *
 * including commercial applications, and to alter it and redistribute it     *
 * freely, subject to the following restrictions:                             *
 *                                                                            *
 * 1. The origin of this software must not be misrepresented; you must not    *
 *    claim that you wrote the original software. If you use this software    *
 *    in a product, an acknowledgment in the product documentation would be   *
 *    appreciated but is not required.                                        *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "automated_tests.h"
#include "lookup3.h"
#include "types.h"
#include "utils.h"
#include "types.h"
#include "memory.h"
#include "cartridge.h"
#include "decoder.h"
#include "graphics.h"
#include "display.h"
#include "disassembler.h"
#include "time_keeper.h"

#define C_RED   "\x1B[31m"
#define C_GRN   "\x1B[32m"
#define C_YEL   "\x1B[33m"
#define C_BLU   "\x1B[34m"
#define C_MAG   "\x1B[35m"
#define C_CYN   "\x1B[36m"
#define C_WHT   "\x1B[37m"
#define C_RESET "\x1B[0m"

_test_info tests_to_run[] = {
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/add_sp_e_timing.gb"                 , 5  , 0x473989eb } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/boot_regs-dmg.gb"                   , 2  , 0x0b8ee224 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/call_cc_timing.gb"                  , 5  , 0x27fb3d0d } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/call_cc_timing2.gb"                 , 6  , 0x77dc532e } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/call_timing.gb"                     , 5  , 0x27fb3d0d } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/call_timing2.gb"                    , 6  , 0xaf069a4d } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/di_timing-GS.gb"                    , 8  , 0x27fb3d0d } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/div_timing.gb"                      , 2  , 0x9b0fdb21 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/ei_timing.gb"                       , 2  , 0xfb05a584 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/halt_ime0_ei.gb"                    , 3  , 0x27fb3d0d } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/halt_ime0_nointr_timing.gb"         , 6  , 0xa0fdf050 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/halt_ime1_timing.gb"                , 2  , 0x34af2966 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/halt_ime1_timing2-GS.gb"            , 10 , 0x67297ab3 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/if_ie_registers.gb"                 , 2  , 0xc6736172 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/intr_timing.gb"                     , 2  , 0xa0deff58 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/jp_cc_timing.gb"                    , 5  , 0x27fb3d0d } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/jp_timing.gb"                       , 5  , 0x27fb3d0d } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/ld_hl_sp_e_timing.gb"               , 5  , 0xadc4f626 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/oam_dma_restart.gb"                 , 5  , 0xd42bf4fb } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/oam_dma_start.gb"                   , 6  , 0xd7c76a01 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/oam_dma_timing.gb"                  , 5  , 0xd42bf4fb } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/pop_timing.gb"                      , 2  , 0xc419cfa5 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/push_timing.gb"                     , 6  , 0x279f2c82 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/rapid_di_ei.gb"                     , 2  , 0xce52a906 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/ret_cc_timing.gb"                   , 6  , 0x27fb3d0d } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/ret_timing.gb"                      , 6  , 0x27fb3d0d } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/reti_intr_timing.gb"                , 2  , 0x5f51bcf3 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/reti_timing.gb"                     , 6  , 0x27fb3d0d } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/rst_timing.gb"                      , 5  , 0x233ca064 } ,

    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/div_write.gb"                 , 43 , 0x27fb3d0d } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/rapid_toggle.gb"              , 2  , 0xd488a5c1 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tim00.gb"                     , 2  , 0x599243ab } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tim00_div_trigger.gb"         , 2  , 0x599243ab } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tim01.gb"                     , 2  , 0x08174fe8 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tim01_div_trigger.gb"         , 2  , 0x5a792211 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tim10.gb"                     , 2  , 0x599243ab } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tim10_div_trigger.gb"         , 2  , 0x811c3a34 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tim11.gb"                     , 2  , 0x599243ab } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tim11_div_trigger.gb"         , 2  , 0x599243ab } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tima_reload.gb"               , 2  , 0xd94f5db3 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tima_write_reloading.gb"      , 2  , 0xea44d4fa } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tma_write_reloading.gb"       , 2  , 0x74f714b4 } ,

    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/gpu/hblank_ly_scx_timing-GS.gb"     , 1  , 0x652505ed } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/gpu/intr_1_2_timing-GS.gb"          , 5  , 0x3b217b13 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/gpu/intr_2_0_timing.gb"             , 4  , 0xf8005057 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/gpu/intr_2_mode0_timing.gb"         , 4  , 0x9af87bb4 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/gpu/intr_2_mode0_timing_sprites.gb" , 213, 0x27fb3d0d } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/gpu/intr_2_mode3_timing.gb"         , 4  , 0x9bed9573 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/gpu/intr_2_oam_ok_timing.gb"        , 6  , 0x79330f39 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/gpu/stat_irq_blocking.gb"           , 4  , 0x27fb3d0d } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/gpu/vblank_stat_intr-GS.gb"         , 10 , 0x350c17f4 } ,

    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/bits/mem_oam.gb"                    , 2  , 0x27fb3d0d } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/bits/reg_f.gb"                      , 2  , 0xaaddf876 } ,
    { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/bits/unused_hwio-GS.gb"             , 2  , 0/*x1ce880f3*/ } ,
};

void test_create_buffer ( _test_control* t ) {
    t->test_bitmap = (uint32_t*) malloc ( sizeof (uint32_t) * 144 * 160 );
}

void test_reset_buffer ( _test_control* t ) {
    memset ( t->test_bitmap, 0,  sizeof (uint32_t) * 144 * 160 );
    t->test_spent_frames  = 0;
    t->test_needed_frames = 0;
}

void test_free_buffer ( _test_control* t ) {
    free ( t->test_bitmap );
}

void test_write_to_buffer ( _test_control* t, uint16_t addr, uint32_t data ) {
    if ( t->test_enable )
        t->test_bitmap[ addr ] = data;
}

uint8_t test_step ( _test_control *t ) {
    t->test_spent_frames += 1;

    return t->test_spent_frames >= t->test_needed_frames && t->test_enable;
}

uint32_t test_get_hash ( ) {
    int i = 22;

    test_create_buffer( &test_control );

    _cpu_info cpu;

    test_control.test_needed_frames = tests_to_run[i].frames;

    init_cpu ( &cpu ); // Memory leak here

    load_rom ( &cpu, tests_to_run[i].name, 0x0000 );

    cpu.die = 0;

    while ( cpu.die == 0 ) {
        decoder        ( &cpu );
    }

    uint32_t hash = hashlittle(test_control.test_bitmap, sizeof (uint32_t) * 144 * 160, 12);

    printf("#%3d [ %s "C_RESET"] 0x%08x -- %s\n", i + 1, hash == tests_to_run[i].hash ? C_GRN "PASS" : C_RED "FAIL", hash, tests_to_run[i].name );
    printf("{ \"%s\", %3d, 0x%08x },\n", tests_to_run[i].name, tests_to_run[i].frames, hash );
    test_reset_buffer( &test_control );

    test_free_buffer( &test_control );

    return hash;
}

void test_run ( ) {
    test_create_buffer( &test_control );

    _cpu_info cpu;

    for (int i = 0; i < 54; ++i) {
        test_control.test_needed_frames = tests_to_run[i].frames;

        init_cpu ( &cpu ); // Memory leak here

        load_rom ( &cpu, tests_to_run[i].name, 0x0000 );

        cpu.die = 0;

        while ( cpu.die == 0 ) {
            decoder        ( &cpu );
        }

        uint32_t hash = hashlittle(test_control.test_bitmap, sizeof (uint32_t) * 144 * 160, 12);

        printf("#%3d [ %s "C_RESET"] 0x%08x -- %s\n", i + 1, hash == tests_to_run[i].hash ? C_GRN "PASS" : C_RED "FAIL", hash, tests_to_run[i].name );
        test_reset_buffer( &test_control );
    }

    test_free_buffer( &test_control );
}

void test_run_adjust ( ) {
    test_create_buffer( &test_control );

    _cpu_info cpu;

    for (int i = 0; i < 54; ++i) {
        for (int j = 1; j < 50; ++j) {
            tests_to_run[i].frames = j;
            test_control.test_needed_frames = tests_to_run[i].frames;

            init_cpu ( &cpu ); // Memory leak here

            load_rom ( &cpu, tests_to_run[i].name, 0x0000 );

            cpu.die = 0;

            while ( cpu.die == 0 ) {
                decoder        ( &cpu );
            }

            uint32_t hash = hashlittle(test_control.test_bitmap, sizeof (uint32_t) * 144 * 160, 12);

            if ( hash == tests_to_run[i].hash ) {
                printf("{ \"%s\", %3d, 0x%08x },\n", tests_to_run[i].name, j, hash );
                break;
            }

            test_reset_buffer( &test_control );
        }
    }

    test_free_buffer( &test_control );
}
