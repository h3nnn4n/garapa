include("j_utils.jl")
include("input_handler.jl")
include("types.jl")
include("feature_functions.jl")
include("position_evaluator.jl")
include("draw.jl")

brain = ga_brain()
gs    = game_state() #init_game_state()

Ta_piece = [ -1 -1 ;  0 -1 ;  1 -1 ; 0 0 ] # Ta
Tb_piece = [  0 -2 ;  0 -1 ;  1 -1 ; 0 0 ] # Tb
Tc_piece = [ -1 -1 ; -2  0 ; -1  0 ; 0 0 ] # Tc
Td_piece = [  0 -2 ; -1 -1 ;  0 -1 ; 0 0 ] # Td

Ja_piece = [ -2 -1 ; -1 -1 ;  0 -1 ; 0 0 ] # Ja
Jb_piece = [  0 -2 ;  1 -2 ;  0 -1 ; 0 0 ] # Jb
Jc_piece = [ -2 -1 ; -2  0 ; -1  0 ; 0 0 ] # Jc
Jd_piece = [  0 -2 ;  0 -1 ; -1  0 ; 0 0 ] # Jd

La_piece = [  0 -1 ;  1 -1 ;  2 -1 ; 0 0 ] # La
Lb_piece = [ -1 -2 ; -1 -1 ; -1  0 ; 0 0 ] # Lb
Lc_piece = [  0 -1 ; -2  0 ; -1  0 ; 0 0 ] # Lc
Ld_piece = [ -1 -2 ;  0 -2 ;  0 -1 ; 0 0 ] # Ld

Sa_piece = [  0 -1 ;  1 -1 ; -1  0 ; 0 0 ] # Sa
Sb_piece = [ -1 -2 ; -1 -1 ;  0 -1 ; 0 0 ] # Sb

Ia_piece = [ -3  0 ; -2  0 ; -1  0 ; 0 0 ] # Ia
Ib_piece = [  0 -3 ;  0 -2 ;  0 -1 ; 0 0 ] # Ib

Za_piece = [ -2 -1 ; -1 -1 ; -1  0 ; 0 0 ] # Za
Zb_piece = [  1 -2 ;  0 -1 ;  1 -1 ; 0 0 ] # Zb

O_piece  = [ -1 -1 ;  0 -1 ; -1  0 ; 0 0 ] # O

function get_piece_n_rotations(piece :: Int64)
    d = Dict{Int64, Int64}()

    d[0x00] = 4
    d[0x01] = 4
    d[0x02] = 4
    d[0x03] = 4

    d[0x04] = 4
    d[0x05] = 4
    d[0x06] = 4
    d[0x07] = 4

    d[0x08] = 2
    d[0x09] = 2
    d[0x0a] = 2
    d[0x0b] = 2

    d[0x0c] = 1
    d[0x0d] = 1
    d[0x0e] = 1
    d[0x0f] = 1

    d[0x10] = 2
    d[0x11] = 2
    d[0x12] = 2
    d[0x13] = 2

    d[0x14] = 2
    d[0x15] = 2
    d[0x16] = 2
    d[0x17] = 2

    d[0x18] = 4
    d[0x19] = 4
    d[0x1a] = 4
    d[0x1b] = 4

    return d[piece]
end

function get_piece_offsets(piece :: Int64)
    d = Dict{Int64, Array{Int64, 2}}()

    d[0x00] = La_piece
    d[0x01] = Lb_piece
    d[0x02] = Lc_piece
    d[0x03] = Ld_piece

    d[0x04] = Ja_piece
    d[0x05] = Jb_piece
    d[0x06] = Jc_piece
    d[0x07] = Jd_piece

    d[0x08] = Ia_piece
    d[0x09] = Ib_piece
    d[0x0a] = Ia_piece
    d[0x0b] = Ib_piece

    d[0x0c] = O_piece
    d[0x0d] = O_piece
    d[0x0e] = O_piece
    d[0x0f] = O_piece

    d[0x10] = Za_piece
    d[0x11] = Zb_piece
    d[0x12] = Za_piece
    d[0x13] = Zb_piece

    d[0x14] = Sa_piece
    d[0x15] = Sb_piece
    d[0x16] = Sa_piece
    d[0x17] = Sb_piece

    d[0x18] = Ta_piece
    d[0x19] = Tb_piece
    d[0x1a] = Tc_piece
    d[0x1b] = Td_piece

    return d[piece]
end

function get_next_piece_rotation(piece :: Int64)
    d = Dict{Int64, Int64}()

    d[0x00] = 0x03
    d[0x01] = 0x00
    d[0x02] = 0x01
    d[0x03] = 0x02

    d[0x04] = 0x07
    d[0x05] = 0x04
    d[0x06] = 0x05
    d[0x07] = 0x06

    d[0x08] = 0x0b
    d[0x09] = 0x08
    d[0x0a] = 0x09
    d[0x0b] = 0x0a

    d[0x0c] = 0x0f
    d[0x0d] = 0x0c
    d[0x0e] = 0x0d
    d[0x0f] = 0x0e

    d[0x10] = 0x13
    d[0x11] = 0x10
    d[0x12] = 0x11
    d[0x13] = 0x12

    d[0x14] = 0x17
    d[0x15] = 0x14
    d[0x16] = 0x15
    d[0x17] = 0x16

    d[0x18] = 0x1b
    d[0x19] = 0x18
    d[0x1a] = 0x19
    d[0x1b] = 0x1a

    return d[piece]
end

function init_game_state()
    brain.mutation_rate  = 0.02
    brain.crossover_rate = 0.85

    brain.number_of_features = 4
    brain.fields_per_feature = 2
    brain.population_size    = 20

    brain.current_generation = 0
    brain.individual_index   = 1

    brain.number_of_evaluations_per_agent      = 5
    brain.number_of_evaluations_per_agent_left = brain.number_of_evaluations_per_agent

    brain.function_evaluations_total = 100
    brain.function_evaluations_left  = brain.function_evaluations_total

    range = 5.0

    brain.population = rand(brain.population_size,
                            brain.number_of_features * brain.fields_per_feature) .* (range * 2.0) .- range

    brain.agent_data = zeros(Float64, (brain.population_size, brain.number_of_evaluations_per_agent))

    brain.fitness = zeros(brain.population_size)
end

function frame_update()
    update_game_state()
    extract_board()
end

function check_for_new_piece()
    if garapa_read_pc() == 0x2062 && gs.screen == ingame
        new_piece()
    end

    #=if garapa_read_pc() == 0x2a0e=#
        #=input_handler(gs, brain)=#
    #=end=#
end

function new_piece()
    #=println("New Piece")=#
    gs.new_piece = true
    #=find_best_move(gs)=#
end

function garapa_init()
    println("Starting GABATE")
    garapa_register_callback_display_update(frame_update)
    garapa_register_callback_cpu_loop(check_for_new_piece)
end

function entered_game()
    #=println("Entered a GAME")=#
    gs.new_piece = true
end

function game_over()
    #=println("Game over")=#

    brain.number_of_evaluations_per_agent_left -= 1
    brain.function_evaluations_left            -= 1

    if brain.number_of_evaluations_per_agent_left <= 0
        brain.number_of_evaluations_per_agent_left = brain.number_of_evaluations_per_agent
        brain.individual_index += 1

        if brain.individual_index > brain.population_size
            evo_step()
            brain.individual_index = 1
        end
    end
end

function evo_step()
    evaluate_fitness()
end

function evaluate_fitness()
    for i in 1:brain.population_size
        brain.fitness[i] = mean(brain.agent_data[1:1:brain.number_of_evaluations_per_agent])
    end
end

function finished_boot()
    init_game_state()
    #=println("BOOT")=#
end

function entered_menu()
    #=println("MENU")=#
end

function draw_overlay()
    posx, posy = 400, 10

    text = @sprintf("screen: 0x%04x", garapa_read_byte(0xffe1))
    garapa_draw_test_with_bg(text, posx, posy, 0, 255, 255)
    posy += 20

    text = @sprintf("gen: %4d - %4d - %4d", brain.current_generation, brain.individual_index, brain.number_of_evaluations_per_agent_left)
    garapa_draw_test_with_bg(text, posx, posy, 0, 255, 255)
    posy += 20

    #=text = @sprintf("generation: %4d", brain.current_generation)=#
    #=garapa_draw_test_with_bg(text, posx, posy, 0, 255, 255)=#
    #=posy += 20=#

    if gs.screen == ingame
        px = (garapa_read_byte(0xff92) - 8 ) * 4
        py = (garapa_read_byte(0xff93) - 16) * 4

        piece = get_piece_offsets(garapa_read_byte(0xc203))

        garapa_draw_rectangle(px + piece[1, 1] * 4 * 8, py + piece[1, 2] * 4 * 8, 8 * 4, 8 * 4, 0  , 0  , 255)
        garapa_draw_rectangle(px + piece[2, 1] * 4 * 8, py + piece[2, 2] * 4 * 8, 8 * 4, 8 * 4, 0  , 0  , 255)
        garapa_draw_rectangle(px + piece[3, 1] * 4 * 8, py + piece[3, 2] * 4 * 8, 8 * 4, 8 * 4, 0  , 0  , 255)
        garapa_draw_rectangle(px + piece[4, 1] * 4 * 8, py + piece[4, 2] * 4 * 8, 8 * 4, 8 * 4, 127, 127, 255)

        best_piece = get_piece_offsets(gs.best_piece)

        garapa_draw_rectangle((gs.best_x + 1 + best_piece[1, 1]) * 4 * 8, (gs.best_y + best_piece[1, 2]) * 4 * 8, 8 * 4, 8 * 4, 0  , 255, 0  )
        garapa_draw_rectangle((gs.best_x + 1 + best_piece[2, 1]) * 4 * 8, (gs.best_y + best_piece[2, 2]) * 4 * 8, 8 * 4, 8 * 4, 0  , 255, 0  )
        garapa_draw_rectangle((gs.best_x + 1 + best_piece[3, 1]) * 4 * 8, (gs.best_y + best_piece[3, 2]) * 4 * 8, 8 * 4, 8 * 4, 0  , 255, 0  )
        garapa_draw_rectangle((gs.best_x + 1 + best_piece[4, 1]) * 4 * 8, (gs.best_y + best_piece[4, 2]) * 4 * 8, 8 * 4, 8 * 4, 127, 255, 127)
    end
end

function piece_moved()

end

olds = 0xff

function update_game_state()
    input_handler(gs, brain)

    if gs.new_piece
        gs.new_piece = false
        bx, by, rotation, best_piece = find_best_move(gs, brain)

        gs.best_x = bx
        gs.best_y = by
        gs.best_r = rotation
        gs.best_piece = best_piece
    end

    draw_overlay()
    screen = garapa_read_byte(0xffe1)

    #=if olds != screen=#
        #=@printf("%02x %02x\n", olds, screen)=#
    #=end=#

    global olds = screen

    px = div(garapa_read_byte(0xff92) - 8 , 8)
    py = div(garapa_read_byte(0xff93) - 16, 8)

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
    elseif screen == 0x10 || screen == 0x11 || screen == 0x0a
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

