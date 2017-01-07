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

void test_run ( ) {
    _test_info tests_to_run[] = {
       { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/div_write.gb"            , 45, 0x27fb3d0d } ,
       { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/rapid_toggle.gb"         , 10, 0xd488a5c1 } ,
       { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tim00.gb"                , 10, 0x599243ab } ,
       { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tim00_div_trigger.gb"    , 10, 0x599243ab } ,
       { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tim01.gb"                , 10, 0x08174fe8 } ,
       { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tim01_div_trigger.gb"    , 10, 0x5a792211 } ,
       { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tim10.gb"                , 10, 0x599243ab } ,
       { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tim10_div_trigger.gb"    , 10, 0x811c3a34 } ,
       { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tim11.gb"                , 10, 0x599243ab } ,
       { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tim11_div_trigger.gb"    , 10, 0x599243ab } ,
       { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tima_reload.gb"          , 10, 0xd94f5db3 } ,
       { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tima_write_reloading.gb" , 10, 0xea44d4fa } ,
       { "../roms/gekkio.fi/files/mooneye-gb/nightly/tests/acceptance/timer/tma_write_reloading.gb"  , 10, 0x74f714b4 }
    };

    test_create_buffer( &test_control );

    _cpu_info cpu;

    for (int i = 0; i < 13; ++i) {
        test_reset_buffer( &test_control );
        test_control.test_needed_frames = tests_to_run[i].frames;

        init_cpu ( &cpu ); // Memory leak here

        load_rom ( &cpu, tests_to_run[i].name, 0x0000 );

        cpu.die = 0;

        while ( cpu.die == 0 ) {
            /*printf("cpu.die = %4d\n", test_control.test_spent_frames);*/
            decoder        ( &cpu );
        }

        uint32_t hash = hashlittle(test_control.test_bitmap, sizeof (uint32_t) * 144 * 160, 12);

        printf("[ %s ] 0x%08x -- %s\n", hash == tests_to_run[i].hash ? "PASS" : "FAIL", hash, tests_to_run[i].name );
    }

    test_free_buffer( &test_control );
}
