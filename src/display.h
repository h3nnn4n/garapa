#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"

void    display_update                 ( _cpu_info *cpu ) ;
uint8_t display_test_screenmode        ( _cpu_info *cpu ) ;
uint8_t display_test_LYC_LY_trigger    ( _cpu_info *cpu ) ;
uint8_t display_test_mode0_hblank      ( _cpu_info *cpu ) ;
uint8_t display_test_mode1_vblank      ( _cpu_info *cpu ) ;
uint8_t display_test_mode2_oam         ( _cpu_info *cpu ) ;
uint8_t display_test_LYC_enable        ( _cpu_info *cpu ) ;
uint8_t display_read_LYC               ( _cpu_info *cpu ) ;
uint8_t display_read_LY                ( _cpu_info *cpu ) ;
uint8_t display_test_bg_enabled        ( _cpu_info *cpu ) ;
uint8_t display_test_sprite_enabled    ( _cpu_info *cpu ) ;
uint8_t display_test_sprite_size       ( _cpu_info *cpu ) ;
uint8_t display_test_tilemap_select    ( _cpu_info *cpu ) ;
uint8_t display_test_bg_tileset_select ( _cpu_info *cpu ) ;
uint8_t display_test_windowenable      ( _cpu_info *cpu ) ;
uint8_t display_test_windowtilemap     ( _cpu_info *cpu ) ;
uint8_t display_test_lcdpower          ( _cpu_info *cpu ) ;
uint8_t display_read_stat              ( _cpu_info *cpu ) ;

void    display_write_LYC              ( _cpu_info *cpu, uint8_t data ) ;

void    display_write_stat             ( _cpu_info *cpu, uint8_t data ) ;

void write_spr1_palette ( _cpu_info *cpu, uint8_t data ) ;
void write_spr2_palette ( _cpu_info *cpu, uint8_t data ) ;
void write_bg_palette   ( _cpu_info *cpu, uint8_t data ) ;

void  write_lcd_control  ( _cpu_info *cpu, uint8_t data ) ;
void    write_scroll_x   ( _cpu_info *cpu, uint8_t data ) ;
void    write_window_y   ( _cpu_info *cpu, uint8_t data ) ;
void    write_window_x   ( _cpu_info *cpu, uint8_t data ) ;
void    write_scroll_y   ( _cpu_info *cpu, uint8_t data ) ;
uint8_t read_scroll_y    ( _cpu_info *cpu ) ;
uint8_t read_scroll_x    ( _cpu_info *cpu ) ;
uint8_t read_window_y    ( _cpu_info *cpu ) ;
uint8_t read_window_x    ( _cpu_info *cpu ) ;
uint8_t read_active_line ( _cpu_info *cpu ) ;
uint8_t read_lcd_control ( _cpu_info *cpu ) ;

#endif /* DISPLAY_H */
