include("types.jl")

#=cpu->joystick.button_start  = !(keys & 0x01);=#
#=cpu->joystick.button_select = !(keys & 0x02);=#
#=cpu->joystick.button_b      = !(keys & 0x04);=#
#=cpu->joystick.button_a      = !(keys & 0x08);=#
#=cpu->joystick.button_down   = !(keys & 0x10);=#
#=cpu->joystick.button_up     = !(keys & 0x20);=#
#=cpu->joystick.button_left   = !(keys & 0x40);=#
#=cpu->joystick.button_right  = !(keys & 0x80);=#

function input_handler(gs :: game_state, ga :: ga_brain)
    gs.input_controller = !gs.input_controller

    #=@printf("pc: 0x%04x\n", garapa_read_pc())=#

    if gs.input_controller
        if gs.screen != ingame
            keys = 0xff + 0

            keys &= 0b11111110

            garapa_write_buttons(keys)
        end
    else
        garapa_write_buttons(0xff + 0)
    end
end

