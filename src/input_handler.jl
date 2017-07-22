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

    #=if !gs.input_controller=#
        #=if rand() < 0.990=#
            #=gs.input_controller = !gs.input_controller=#
        #=end=#
    #=end=#

    keys = 0xff + 0

    if gs.input_controller
        if gs.screen != ingame
            keys = 0xff + 0

            keys &= 0b11111110

            garapa_write_buttons(keys)
        else
            if garapa_read_byte(0xffab) != 0
                keys &= 0b11111110
            else
                if garapa_read_byte(0xc203) != gs.best_piece
                    keys &= 0b11110111
                else
                    if gs.px > gs.best_x + 1
                        keys &= 0b10111111
                    elseif gs.px < gs.best_x + 1
                        keys &= 0b01111111
                    else
                        keys &= 0b11101111
                    end
                end
            end
        end
    end

    garapa_write_buttons(keys)
end

