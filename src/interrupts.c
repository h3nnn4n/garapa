#include <stdio.h>
#include <assert.h>

#include "memory.h"
#include "utils.h"
#include "types.h"

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

// Bit 0: V-Blank  Interrupt Request (INT 40h)  (1=Request)
// Bit 1: LCD STAT Interrupt Request (INT 48h)  (1=Request)
// Bit 2: Timer    Interrupt Request (INT 50h)  (1=Request)
// Bit 3: Serial   Interrupt Request (INT 58h)  (1=Request)
// Bit 4: Joypad   Interrupt Request (INT 60h)  (1=Request)
void emulate_INTERRUPT ( _cpu_info *cpu ) {
    if ( cpu->pending_interrupts == 2 ) {
        cpu->pending_interrupts --;
        return;
    } else {
        cpu->pending_interrupts = 0;
    }

    uint8_t intn = read_byte(cpu, 0xff0f) & (read_byte(cpu, 0xffff));

    /*printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(intn));*/

    if ( cpu->enable_interrupts == 0 && (
            read_byte(cpu, 0xff0f) &
            read_byte(cpu, 0xffff)
            )
       ) {
        cpu->halted = 0;
        return;
    }

    uint8_t  doit = 0;
    uint16_t  ret = cpu->pc;

           if ( intn & 0x01 ) { // vblank
        cpu->pc = 0x0040;
        cpu->interrupts.pending_vblank = 0;
        doit = 1;
    } else if ( intn & 0x02 ) { // lcdstat
        cpu->pc = 0x0048;
        cpu->interrupts.pending_lcdstat = 0;
        doit = 1;
    } else if ( intn & 0x04 ) { // timer
        cpu->pc = 0x0050;
        cpu->interrupts.pending_timer = 0;
        doit = 1;
    } else if ( intn & 0x08 ) { // serial
        cpu->pc = 0x0058;
        doit = 1;
    } else if ( intn & 0x10 ) { // joypad
        cpu->pc = 0x0060;
        cpu->interrupts.pending_joypad = 0;
        doit = 1;
    }

    if ( doit ) {
        cpu->memory[cpu->sp-1] = (ret >> 8) & 0xff;
        cpu->memory[cpu->sp-2] = (ret & 0xff);
        cpu->sp                = cpu->sp - 2;
        cpu->enable_interrupts = 0;
    }
}

uint8_t interrupt_read_IF( _cpu_info* cpu ) {
    uint8_t mask = 0xE0;

    mask |= (cpu->interrupts.pending_vblank  << 0);
    mask |= (cpu->interrupts.pending_lcdstat << 1);
    mask |= (cpu->interrupts.pending_timer   << 2);
    mask |= (cpu->interrupts.pending_serial  << 3);
    mask |= (cpu->interrupts.pending_joypad  << 4);

    /*printf("MEU: IF reads %d %02x\n", mask, mask);*/

    return mask;
}

void interrupt_write_IF( _cpu_info *cpu, uint8_t mask) {
    cpu->interrupts.pending_vblank  = !!(mask & 0x01);
    cpu->interrupts.pending_lcdstat = !!(mask & 0x02);
    cpu->interrupts.pending_timer   = !!(mask & 0x04);
    cpu->interrupts.pending_serial  = !!(mask & 0x08);
    cpu->interrupts.pending_joypad  = !!(mask & 0x10);

    /*printf("MEU: IF set to %02x: %d %d %d %d %d\n", mask,*/
            /*cpu->interrupts.pending_vblank ,*/
            /*cpu->interrupts.pending_lcdstat,*/
            /*cpu->interrupts.pending_timer  ,*/
            /*cpu->interrupts.pending_serial ,*/
            /*cpu->interrupts.pending_joypad*/
          /*);*/

    if ( mask && cpu->enable_interrupts ) {
        cpu->pending_interrupts = 1;
    }
}

uint8_t interrupt_read_mask( _cpu_info *cpu ) {
    unsigned char mask = 0;

    mask |= (!cpu->interrupts.masked_vblank  << 0);
    mask |= (!cpu->interrupts.masked_lcdstat << 1);
    mask |= (!cpu->interrupts.masked_timer   << 2);
    mask |= (!cpu->interrupts.masked_serial  << 3);
    mask |= (!cpu->interrupts.masked_joypad  << 4);

    return mask;
}

void interrupt_write_mask( _cpu_info *cpu, uint8_t mask ) {
    cpu->interrupts.masked_vblank  = !(mask & 0x01);
    cpu->interrupts.masked_lcdstat = !(mask & 0x02);
    cpu->interrupts.masked_timer   = !(mask & 0x04);
    cpu->interrupts.masked_serial  = !(mask & 0x08);
    cpu->interrupts.masked_joypad  = !(mask & 0x10);
}
