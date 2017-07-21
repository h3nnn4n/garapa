include("j_utils.jl")
include("types.jl")
include("feature_functions.jl")

bsize_x = 10
bsize_y = 17

function init_game_state()
    gs = game_state(falses(bsize_y, bsize_x), boot)

    return gs
end

function frame_update()
    update_game_state()
    extract_board()
end

function check_for_new_piece()
    if garapa_read_pc() == 0x2062
        println("New piece")
        println(aggegrate_height(gs.board))
    end
end

function garapa_init()
    println("Starting GABATE")
    garapa_register_callback_display_update(frame_update)
    garapa_register_callback_cpu_loop(check_for_new_piece)
end

function entered_game()
    println("Entered a GAME")
end

function game_over()
    println("Game over")
end

function finished_boot()
    println("BOOT")
end

function entered_menu()
    println("MENU")
end

function update_game_state()
    screen = garapa_read_byte(0xffe1)

    old_screen = gs.screen

    if screen == 0x00
        global gs.screen = ingame
    elseif screen == 0xff
        global gs.screen = boot
    elseif screen == 0x24 || screen == 0x25
        global gs.screen = credits
    elseif screen == 0x06 || screen == 0x07
        global gs.screen = menu
    elseif screen == 0x08 || screen == 0x0e
        global gs.screen = gametype
    elseif screen == 0x10 || screen == 0x11
        global gs.screen = atype
    elseif screen == 0x00
        global gs.screen = ingame
    elseif screen == 0x01 || screen == 0x0d || screen == 0x04
        global gs.screen = gameover
    end

    if old_screen != gs.screen
        if old_screen == ingame && gs.screen == gameover
            game_over()
        elseif old_screen != ingame && gs.screen == ingame
            entered_game()
        elseif old_screen == boot && gs.screen != boot
            finished_boot()
        elseif old_screen != menu && gs.screen == menu
            entered_menu()
        end
    end
end

function extract_board()
    x = 0
    y = 0
    for base in 0xc822:0x20:0xca22
        y += 1
        for offset in 0x00:0x09
            x += 1
            address = base + offset

            byte = garapa_read_byte(address)

            occupied = (byte == 0x2f ? false : true)
            global gs.board[y, x] = occupied
        end

        x = 0
    end
end

gs = init_game_state()

