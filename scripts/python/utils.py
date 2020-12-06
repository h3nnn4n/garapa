import garapa


def init_joystick():
    for key in ['right', 'left', 'up', 'down', 'a', 'b', 'select', 'start']:
        garapa.set_input(key, 0)
