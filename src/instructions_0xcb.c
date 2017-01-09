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
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "types.h"
#include "memory.h"
#include "microcode.h"
#include "time_keeper.h"
#include "instructions_data_transfer.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
      (byte & 0x80 ? '1' : '0'), \
      (byte & 0x40 ? '1' : '0'), \
      (byte & 0x20 ? '1' : '0'), \
      (byte & 0x10 ? '1' : '0'), \
      (byte & 0x08 ? '1' : '0'), \
      (byte & 0x04 ? '1' : '0'), \
      (byte & 0x02 ? '1' : '0'), \
      (byte & 0x01 ? '1' : '0')

void emulate_RLC ( _cpu_info *cpu, uint8_t target ) {
    if ( target == 6 ) {
        uint8_t  a   = read_byte_with_tick ( cpu, read_hl ( cpu  ));

        uint8_t t    = (a     >> 7) & 0x01;
        cpu->flags.c =   t;
        a            = (a     << 1)  | t  ;

        write_byte_with_tick ( cpu, read_hl ( cpu ), a);
        cpu->flags.z =  a     == 0   ;
    } else {
        uint8_t *a   = get_reg_ref(cpu, target);

        uint8_t t    = (*a     >> 7) & 0x01;
        cpu->flags.c =   t;
        *a           = (*a     << 1)  | t  ;
        cpu->flags.z =  *a     == 0   ;
    }

    cpu->flags.h = 0;
    cpu->flags.n = 0;
}

void emulate_RRC ( _cpu_info *cpu, uint8_t target ) {
    if ( target == 6 ) {
        uint8_t  a   = read_byte_with_tick ( cpu, read_hl ( cpu  ));

        uint8_t t    =  a     &  0x01;
        cpu->flags.c =  a     &  0x01;
        a            = (a     >> 1)  | ( t << 7 );

        write_byte_with_tick ( cpu, read_hl ( cpu ), a);

        cpu->flags.z =   a     == 0   ;
    } else {
        uint8_t *a   = get_reg_ref(cpu, target);

        uint8_t t    =  *a     &  0x01;
        cpu->flags.c =  *a     &  0x01;
        *a           = (*a     >> 1)  | ( t << 7 );
        cpu->flags.z =  *a     == 0   ;
    }

    cpu->flags.h = 0;
    cpu->flags.n = 0;
}

void emulate_RL    ( _cpu_info *cpu, uint8_t target ) {
    if ( target == 6 ) {
        uint8_t  a   = read_byte_with_tick ( cpu, read_hl ( cpu  ));

        uint8_t t    = cpu->flags.c != 0;
        cpu->flags.c = (a     >> 7) & 0x01;
        a            = (a     << 1) | t   ;

        write_byte_with_tick ( cpu, read_hl ( cpu ), a);

        cpu->flags.z =  a     == 0        ;
    } else {
        uint8_t *a   = get_reg_ref(cpu, target);

        uint8_t t    = cpu->flags.c != 0;
        cpu->flags.c = (*a     >> 7) & 0x01;
        *a           = (*a     << 1) | t   ;
        cpu->flags.z =  *a     == 0        ;
    }

    cpu->flags.h = 0;
    cpu->flags.n = 0;
}

void emulate_RR ( _cpu_info *cpu, uint8_t target ) {
    if ( target == 6 ) {
        uint8_t  a   = read_byte_with_tick ( cpu, read_hl ( cpu  ));

        uint8_t t    = cpu->flags.c != 0;
        cpu->flags.c =  a     &  0x01;
        a            = (a     >> 1)  | ( t << 7 );

        write_byte_with_tick ( cpu, read_hl ( cpu ), a);

        cpu->flags.z =  a     == 0   ;
    } else {
        uint8_t *a   = get_reg_ref(cpu, target);

        uint8_t t    = cpu->flags.c != 0;
        cpu->flags.c =  *a     &  0x01;
        *a           = (*a     >> 1)  | ( t << 7 );
        cpu->flags.z =  *a     == 0   ;
    }

    cpu->flags.h = 0;
    cpu->flags.n = 0;
}

void emulate_SLA   ( _cpu_info *cpu, uint8_t target ) {
    if ( target == 6 ) {
        uint8_t  a   = read_byte_with_tick ( cpu, read_hl ( cpu  ));

        cpu->flags.c = (a     >> 7) & 0x01;
        a            = (a     << 1)       ;

        write_byte_with_tick ( cpu, read_hl ( cpu ), a);

        cpu->flags.z =  a     == 0        ;
    } else {
        uint8_t *a   = get_reg_ref(cpu, target);

        cpu->flags.c = (*a     >> 7) & 0x01;
        *a           = (*a     << 1)       ;
        cpu->flags.z =  *a     == 0        ;
    }

    cpu->flags.h = 0;
    cpu->flags.n = 0;
}

void emulate_SRA   ( _cpu_info *cpu, uint8_t target ) {
    if ( target == 6 ) {
        uint8_t  a   = read_byte_with_tick ( cpu, read_hl ( cpu  ));

        uint8_t t    =  a     &  0x80   ;
        cpu->flags.c =  a     &  0x01   ;
        a            = (a     >> 1)  | t;

        write_byte_with_tick ( cpu, read_hl ( cpu ), a);

        cpu->flags.z =  a     == 0      ;
    } else {
        uint8_t *a   = get_reg_ref(cpu, target);

        uint8_t t    =  *a     &  0x80   ;
        cpu->flags.c =  *a     &  0x01   ;
        *a           = (*a     >> 1)  | t;
        cpu->flags.z =  *a     == 0      ;
    }

    cpu->flags.h = 0;
    cpu->flags.n = 0;
}

void emulate_SWAP  ( _cpu_info *cpu, uint8_t target ) {
    if ( target == 6 ) {
        uint8_t  a   = read_byte_with_tick ( cpu, read_hl ( cpu  ));

        uint8_t lo   = a & 0x0f;
        uint8_t hi   = a & 0xf0;

        a            = (lo << 4) | (hi >> 4);

        write_byte_with_tick ( cpu, read_hl ( cpu ), a);

        cpu->flags.z = a == 0;
    } else {
        uint8_t *a   = get_reg_ref(cpu, target);

        uint8_t lo   = *a & 0x0f;
        uint8_t hi   = *a & 0xf0;

        *a           = (lo << 4) | (hi >> 4);
        cpu->flags.z =  *a     == 0         ;
    }

    cpu->flags.h = 0;
    cpu->flags.c = 0;
    cpu->flags.n = 0;
}

void emulate_SRL   ( _cpu_info *cpu, uint8_t target ) {
    if ( target == 6 ) {
        uint8_t  a   = read_byte_with_tick ( cpu, read_hl ( cpu  ));
        write_byte_with_tick ( cpu, read_hl ( cpu ), a >> 1);
        cpu->flags.c =   a     &  0x01;
        cpu->flags.z = ( a >> 1 ) == 0;
    } else {
    uint8_t *a   = get_reg_ref(cpu, target);
        cpu->flags.c =  *a     &  0x01;
        *a           = (*a     >> 1)  ;
        cpu->flags.z =  *a     == 0   ;
    }

    cpu->flags.h = 0;
    cpu->flags.n = 0;
}

void emulate_BIT   ( _cpu_info *cpu, uint8_t target, uint8_t data ) {
    if ( target == 6 ) {
        uint8_t  a   = read_byte_with_tick ( cpu, read_hl ( cpu  ));
        cpu->flags.z = (a & data) == 0;
    } else {
        uint8_t *a   = get_reg_ref(cpu, target);
        cpu->flags.z = (*a & data) == 0;
    }

    cpu->flags.h = 1;
    cpu->flags.n = 0;
}

void emulate_RES   ( _cpu_info *cpu, uint8_t target, uint8_t data ) {
    if ( target == 6 ) {
        uint8_t  a   = read_byte_with_tick ( cpu, read_hl ( cpu  ));
        write_byte_with_tick ( cpu, read_hl ( cpu ), a &  (~data ));
    } else {
        uint8_t *a   = get_reg_ref(cpu, target);
        *a          &= ~data;
    }
}

void emulate_SET   ( _cpu_info *cpu, uint8_t target, uint8_t data ) {
    if ( target == 6 ) {
        uint8_t  a   = read_byte_with_tick ( cpu, read_hl ( cpu ));
        write_byte_with_tick ( cpu, read_hl ( cpu ), a | data );
    } else {
        uint8_t *a   = get_reg_ref(cpu, target);
        *a          |= data;
    }
}

/*
 * 00000xxx = RLC  xxx
 * 00001xxx = RRC  xxx
 * 00010xxx = RL   xxx
 * 00011xxx = RR   xxx
 * 00100xxx = SLA  xxx
 * 00101xxx = SRA  xxx
 * 00110xxx = SWAP xxx
 * 00111xxx = SRL  xxx
 * 01yyyxxx = BIT  yyy, xxx
 * 10yyyxxx = RES  yyy, xxx
 * 11yyyxxx = SET  yyy, xxx
 */

void decode_0xcb( _cpu_info *cpu ) {
    uint8_t target;
    uint8_t data  ;
    uint8_t opcode;

    void (*f1[])(_cpu_info *, uint8_t) =
            {
                emulate_RLC ,
                emulate_RRC ,
                emulate_RL  ,
                emulate_RR  ,
                emulate_SLA ,
                emulate_SRA ,
                emulate_SWAP,
                emulate_SRL
            };

    void (*f2[])(_cpu_info *, uint8_t, uint8_t) =
            {
                NULL       ,
                emulate_BIT,
                emulate_RES,
                emulate_SET
            };

    target = cpu->opcode &  0x07;
    opcode = cpu->opcode >> 3   ;

    if ( !(opcode & 0xf8) ) {
        f1[opcode](cpu, target);
    } else {
        data     = opcode & 0x07;
        opcode >>= 3;
        f2[opcode](cpu, target, 1 << data);
    }
}
