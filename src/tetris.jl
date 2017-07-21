include("j_utils.jl")
include("types.jl")
include("feature_functions.jl")
include("position_evaluator.jl")
include("draw.jl")

brain = ga_brain()

function init_game_state()
    bsize_y = 17
    bsize_x = 10
    gs      = game_state(falses(bsize_y, bsize_x), boot, -1, -1, bsize_x, bsize_y)

    brain.mutation_rate  = 0.02
    brain.crossover_rate = 0.85

    brain.number_of_features = 4
    brain.fields_per_feature = 2
    brain.population_size    = 20

    range = 5.0

    brain.population = rand(brain.population_size,
                            brain.number_of_features * brain.fields_per_feature) .* (range * 2.0) .- range

    return gs
end

function frame_update()
    update_game_state()
    extract_board()
end

function check_for_new_piece()
    if garapa_read_pc() == 0x2062
        #=println("New piece")=#
        #=@printf("%4d %4d %4d %4d\n",=#
            #=aggegrate_height(gs.board),=#
            #=number_of_holes(gs.board),=#
            #=surface_variance(gs.board),=#
            #=cleared_rows(gs.board)=#
        #=)=#
    end
end

function new_piece()
    evaluate_board(gs)
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

function draw_overlay()
    text = @sprintf("screen: 0x%04x", garapa_read_byte(0xffe1))
    garapa_draw_test_with_bg(text, 500, 10, 0, 255, 255)

    if gs.screen == ingame
        px = (garapa_read_byte(0xff92) - 8 ) * 4
        py = (garapa_read_byte(0xff93) - 16) * 4

        garapa_draw_rectangle(px, py, 8 * 4, 8 * 4, 0, 255, 0)
    end
end


function piece_moved()
    px = garapa_read_byte(0xc202)
    py = garapa_read_byte(0xc201)

    new_piece()
end


function update_game_state()
    draw_overlay()
    screen = garapa_read_byte(0xffe1)

    px     = garapa_read_byte(0xc202)
    py     = garapa_read_byte(0xc201)

    if px != gs.px || py != gs.py
        if screen == 0x00
            piece_moved()
        end

        gs.px = px
        gs.py = py
    end

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

