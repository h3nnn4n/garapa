import garapa

import png


def init_joystick():
    for key in ['right', 'left', 'up', 'down', 'a', 'b', 'select', 'start']:
        garapa.set_input(key, 0)


def dump_sprite(addr=0x8000):
    name_suffix = (addr - 0x8000) / 0x000f

    png_data = [[]]
    colors = [
        0xffffff,
        0xc0c0c0,
        0x808080,
        0x000000,
    ]

    for index, addr in enumerate(range(addr, addr + 0xf + 1)):
        data = garapa.peek(addr)

        b1 = (data & 0b00000011) >> 0
        b2 = (data & 0b00001100) >> 2
        b3 = (data & 0b00110000) >> 4
        b4 = (data & 0b11000000) >> 6

        png_data[-1].append(colors[b1] // (256 ** 2))
        png_data[-1].append(colors[b2] // (256 ** 2))
        png_data[-1].append(colors[b3] // (256 ** 2))
        png_data[-1].append(colors[b4] // (256 ** 2))

        if index % 2 == 1:
            png_data.append([])

    png_data.pop()

    png.from_array(
        png_data,
        'L'
    ).save(f"sprite_{name_suffix:.0f}.png")
