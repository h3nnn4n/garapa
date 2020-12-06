import garapa
import utils


frame_count = 0


def on_vblank():
    global frame_count

    frame_count += 1

    # wait 60 frames before dumping first sprite
    if frame_count <= 60: return

    utils.dump_sprite(0x8000)

    garapa.kill_emulator()


def main():
    print(garapa.hello_world())

    garapa.disable_user_input()
    garapa.set_vblank_callback(on_vblank)

    utils.init_joystick()
