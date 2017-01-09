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

#include "cartridge.h"
#include "types.h"

char *mbc_code[] = {
    [0x00] = "ROM ONLY",
    [0x01] = "MBC1",
    [0x02] = "MBC1+RAM",
    [0x03] = "MBC1+RAM+BATTERY",
    [0x05] = "MBC2",
    [0x06] = "MBC2+BATTERY",
    [0x08] = "ROM+RAM",
    [0x09] = "ROM+RAM+BATTERY",
    [0x0B] = "MMM01",
    [0x0C] = "MMM01+RAM",
    [0x0D] = "MMM01+RAM+BATTERY",
    [0x0F] = "MBC3+TIMER+BATTERY",
    [0x10] = "MBC3+TIMER+RAM+BATTERY",
    [0x11] = "MBC3",
    [0x12] = "MBC3+RAM",
    [0x13] = "MBC3+RAM+BATTERY",
    [0x15] = "MBC4",
    [0x16] = "MBC4+RAM",
    [0x17] = "MBC4+RAM+BATTERY",
    [0x19] = "MBC5",
    [0x1A] = "MBC5+RAM",
    [0x1B] = "MBC5+RAM+BATTERY",
    [0x1C] = "MBC5+RUMBLE",
    [0x1D] = "MBC5+RUMBLE+RAM",
    [0x1E] = "MBC5+RUMBLE+RAM+BATTERY",
    [0x20] = "MBC6",
    [0x22] = "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
    [0xFC] = "POCKET CAMERA",
    [0xFD] = "BANDAI TAMA5",
    [0xFE] = "HuC3",
    [0xFF] = "HuC1+RAM+BATTERY"
};

uint32_t rom_size[] = {
    [0x00] = 2   * 0x4000, // " 32KByte (no ROM banking)",
    [0x01] = 4   * 0x4000, // " 64KByte (4 banks)",
    [0x02] = 8   * 0x4000, // "128KByte (8 banks)",
    [0x03] = 16  * 0x4000, // "256KByte (16 banks)",
    [0x04] = 32  * 0x4000, // "512KByte (32 banks)",
    [0x05] = 64  * 0x4000, // "  1MByte (64 banks)",
    [0x06] = 128 * 0x4000, // "  2MByte (128 banks)",
    [0x07] = 256 * 0x4000, // "  4MByte (256 banks)",
    [0x08] = 512 * 0x4000, // "  8MByte (512 banks)",
    [0x52] = 72  * 0x4000, // "1.1MByte (72 banks)",
    [0x53] = 80  * 0x4000, // "1.2MByte (80 banks)",
    [0x54] = 96  * 0x4000  // "1.5MByte (96 banks)"
};

char *rom_size_code[] = {
    [0x00] = " 32KByte (no ROM banking)",
    [0x01] = " 64KByte (4 banks)",
    [0x02] = "128KByte (8 banks)",
    [0x03] = "256KByte (16 banks)",
    [0x04] = "512KByte (32 banks)",
    [0x05] = "  1MByte (64 banks)",
    [0x06] = "  2MByte (128 banks)",
    [0x07] = "  4MByte (256 banks)",
    [0x08] = "  8MByte (512 banks)",
    [0x52] = "1.1MByte (72 banks)",
    [0x53] = "1.2MByte (80 banks)",
    [0x54] = "1.5MByte (96 banks)"
};

uint32_t ram_size[] = {
    [0x00] = 0  * 0x0800, // "None",
    [0x01] = 1  * 0x0800, // "2 KBytes",
    [0x02] = 1  * 0x2000, // "8 Kbytes",
    [0x03] = 4  * 0x2000, // "32 KBytes (4 banks of 8KBytes each)",
    [0x04] = 16 * 0x2000, // "128 KBytes (16 banks of 8KBytes each)",
    [0x05] = 8  * 0x2000  // "64 KBytes (8 banks of 8KBytes each)"
};

char *ram_size_code[] = {
    [0x00] = "None",
    [0x01] = "2 KBytes",
    [0x02] = "8 Kbytes",
    [0x03] = "32 KBytes (4 banks of 8KBytes each)",
    [0x04] = "128 KBytes (16 banks of 8KBytes each)",
    [0x05] = "64 KBytes (8 banks of 8KBytes each)"
};

uint8_t get_ram_code ( uint8_t *addr) {
    return *(addr+0x0149);
}

uint8_t get_rom_code ( uint8_t *addr) {
    return *(addr+0x0148);
}

uint32_t get_ram_size ( uint8_t *addr) {
    return ram_size[*(addr+0x0149)];
}

uint32_t get_rom_size ( uint8_t *addr) {
    return rom_size[*(addr+0x0148)];
}

void print_rom_info ( _cpu_info *cpu ) {
    printf("-------------------------\n");
    printf("Name: ");
    for (int i = 0; i < 16; ++i)
        printf("%c", cpu->mem_controller.memory[0x134 + i]);
    printf("\n");

    printf("Mapper: 0x%02x %s\n"  , cpu->mem_controller.memory[0x0147], mbc_code[cpu->mem_controller.memory[0x0147]]);
    printf("ROM size: 0x%02x %s\n", cpu->mem_controller.memory[0x0148], rom_size_code[cpu->mem_controller.memory[0x0148]]);
    printf("RAM size: 0x%02x %s\n", cpu->mem_controller.memory[0x0149], ram_size_code[cpu->mem_controller.memory[0x0149]]);

    printf("-------------------------\n");
}
