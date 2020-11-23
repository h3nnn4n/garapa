import garapa


KEYS = ['right', 'left', 'up', 'down', 'a', 'b', 'select', 'start']
old_screen_id = 0
key_pivot = 0


def on_vblank():
    global old_screen_id
    global key_pivot

    current_screen_id = garapa.peek(0xffe1)

    if old_screen_id == current_screen_id:
        garapa.set_input(
            KEYS[key_pivot % 8],
            1 if key_pivot >= 8 else 0
        )

        key_pivot += 1

        if key_pivot >= 16:
            key_pivot = 0

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

    garapa.disable_user_input()
    garapa.set_vblank_callback(on_vblank)

    for key in ['right', 'left', 'up', 'down', 'a', 'b', 'select', 'start']:
        garapa.set_input(key, 0)

    dump_bytes()
