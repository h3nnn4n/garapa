import garapa

old_screen_id = 0


def on_vblank():
    global old_screen_id

    current_screen_id = garapa.peek(0xffe1)

    if old_screen_id == current_screen_id:
        return

    old_screen_id = current_screen_id

    if current_screen_id == 0xff:
        print('booting')
    elif current_screen_id == 0x24:
        print('preboot')
    elif current_screen_id == 0x25:
        print('credits')
    elif current_screen_id == 0x35:
        print('after credits')
    elif current_screen_id == 0x06:
        print('pre main menu')
    elif current_screen_id == 0x07:
        print('main menu')
    elif current_screen_id == 0x00:
        print('game menu')
    elif current_screen_id == 0x0a:
        print('demo ?? 0x0a')
    else:
        print('?? %02x' % current_screen_id)


def dump_bytes():
    counter = 0
    for addr in range(0x0000, 0x00ff + 1):
        print('%02x' % garapa.peek(addr), end=' ')
        counter += 1

        if counter >= 16:
            counter = 0
            print()


def main():
    print(garapa.hello_world())

    garapa.set_vblank_callback(on_vblank)

    dump_bytes()
