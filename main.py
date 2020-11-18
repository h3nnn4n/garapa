import garapa


def on_vblank():
    pass


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
