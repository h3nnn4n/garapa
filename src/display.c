#include "types.h"
#include "memory.h"
#include "display.h"

// 0 = Off
// 1 = On
uint8_t display_test_lcdpower ( _cpu_info *cpu ) {
    return read_byte(cpu, 0xff40) & 0x80;
}

// 0 = 0x9800 - 0x9bff
// 1 = 0x9c00 - 0x9fff
uint8_t display_test_windowtilemap ( _cpu_info *cpu ) {
    return read_byte(cpu, 0xff40) & 0x40;
}

// 0 = Disabled
// 1 = Disabled
uint8_t display_test_windowenable ( _cpu_info *cpu ) {
    return read_byte(cpu, 0xff40) & 0x20;
}

// 0 = 0x8800 - 0x97ff
// 1 = 0x8000 - 0xfff
uint8_t display_test_bg_tileset_select ( _cpu_info *cpu ) {
    return read_byte(cpu, 0xff40) & 0x10;
}

// 0 = 0x9800 - 0x9bff
// 1 = 0x9c00 - 0x9fff
uint8_t display_test_tilemap_select ( _cpu_info *cpu ) {
    return read_byte(cpu, 0xff40) & 0x08;
}

// 0 = 8x8
// 1 = 8x16
uint8_t display_test_sprite_size ( _cpu_info *cpu ) {
    return read_byte(cpu, 0xff40) & 0x04;
}

// 0 = Disabled
// 1 = Disabled
uint8_t display_test_sprite_enabled ( _cpu_info *cpu ) {
    return read_byte(cpu, 0xff40) & 0x02;
}

// 0 = Disabled
// 1 = Disabled
uint8_t display_test_bg_enabled ( _cpu_info *cpu ) {
    return read_byte(cpu, 0xff40) & 0x01;
}

// Current horizontal line being draw
uint8_t display_read_LY ( _cpu_info *cpu ) {
    if ( display_test_lcdpower(cpu) )
        return read_byte (cpu, 0xff44 );
    return 0x00;
}

// LY = LYC check enable
uint8_t display_test_LYC_enable ( _cpu_info *cpu ) {
    return read_byte ( cpu, 0xff41 ) & 0x40;
}

uint8_t display_test_mode2_oam ( _cpu_info *cpu ) {
    return read_byte ( cpu, 0xff41 ) & 0x20;
}

uint8_t display_test_mode1_vblank ( _cpu_info *cpu ) {
    return read_byte ( cpu, 0xff41 ) & 0x10;
}

uint8_t display_test_mode0_hblank ( _cpu_info *cpu ) {
    return read_byte ( cpu, 0xff41 ) & 0x08;
}

uint8_t display_test_LYC_LY_trigger ( _cpu_info *cpu ) {
    return read_byte ( cpu, 0xff41 ) & 0x04;
}

uint8_t display_test_screenmode ( _cpu_info *cpu ) {
    if ( display_test_lcdpower(cpu) )
        return read_byte ( cpu, 0xff41 ) & 0x03;

    return 0x00;
}

void timer_update( _cpu_info *cpu ) {

}
