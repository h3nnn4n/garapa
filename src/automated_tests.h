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
#ifndef AUTOMATED_TESTS_Hj
#define AUTOMATED_TESTS_H

#include <stdint.h>

typedef struct {
    char name[4096];
    uint32_t frames;
    uint32_t hash;
} _test_info;

typedef struct {
    uint32_t *test_bitmap;
    uint32_t  test_spent_frames;
    uint32_t  test_needed_frames;
    uint8_t   test_bitmap_inialized;
    uint8_t   test_enable;
} _test_control;

_test_control test_control;

uint32_t test_get_hash    (                                                ) ;
uint8_t test_step         ( _test_control *t                               ) ;
void test_create_buffer   ( _test_control* t                               ) ;
void test_reset_buffer    ( _test_control* t                               ) ;
void test_free_buffer     ( _test_control* t                               ) ;
void test_write_to_buffer ( _test_control* t, uint16_t addr, uint32_t data ) ;
void test_run             (                                                ) ;
void test_run_adjust      (                                                ) ;


#endif /* AUTOMATED_TESTS_H */
