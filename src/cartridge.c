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

char *ram_size_code[] = {
    [0x00] = "None",
    [0x01] = "2 KBytes",
    [0x02] = "8 Kbytes",
    [0x03] = "32 KBytes (4 banks of 8KBytes each)",
    [0x04] = "128 KBytes (16 banks of 8KBytes each)",
    [0x05] = "64 KBytes (8 banks of 8KBytes each)"
};

void print_rom_info ( _cpu_info *cpu ) {
    printf("-------------------------\n");
    printf("Name: ");
    for (int i = 0; i < 16; ++i)
        printf("%c", cpu->memory[0x134 + i]);
    printf("\n");

    printf("Mapper: 0x%02x %s\n", cpu->memory[0x0147], mbc_code[cpu->memory[0x0147]]);
    printf("ROM size: 0x%02x %s\n", cpu->memory[0x0148], rom_size_code[cpu->memory[0x0148]]);
    printf("RAM size: 0x%02x %s\n", cpu->memory[0x0149], ram_size_code[cpu->memory[0x0149]]);

    printf("-------------------------\n");
}
