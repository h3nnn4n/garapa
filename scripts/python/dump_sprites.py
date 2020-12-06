import garapa
import utils
import png


frame_count = 0


def on_vblank():
    global frame_count

    frame_count += 1

    # wait 60 frames before dumping first sprite
    if frame_count <= 60: return

    png_data = [[]]
    colors = [
        0xffffff,
        0xc0c0c0,
        0x808080,
        0x000000,
    ]

    for index, addr in enumerate(range(0x8000, 0x800f + 1)):
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
    ).save("sprite.png")

    garapa.kill_emulator()


def main():
    print(garapa.hello_world())

    garapa.disable_user_input()
    garapa.set_vblank_callback(on_vblank)

    utils.init_joystick()
