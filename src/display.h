/******************************************************************************
 * Copyright (C) 2016-2018  Renan S. Silva                                    *
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
