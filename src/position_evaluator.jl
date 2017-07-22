include("types.jl")
include("feature_functions.jl")
include("feature_sets.jl")
include("pieces.jl")
include("draw.jl")
include("tetris_utils.jl")

stack = BitArray{2}[]

function store_board(board :: BitArray{2})
    push!(stack, copy(board))
end

function load_board() :: BitArray{2}
    return pop!(stack)
end

function evaluate_board(gs :: game_state, ga :: ga_brain) :: Float64
    total = 0
    rf = get_raw_features(gs, ga)

    for i in 1:ga.number_of_features
        a = ga.individual_index
        total += rf[i] * ga.population[a, i * 2 - 1] + ga.population[a, i * 2 + 0]
    end

    return total
end


function find_best_move(gs :: game_state, ga :: ga_brain)
    px = div(garapa_read_byte(0xff92) - 8 , 8)
    py = div(garapa_read_byte(0xff93) - 16, 8)

    starting_piece = garapa_read_byte(0xc203)
    piece          = starting_piece
    n_rotations    = get_piece_n_rotations(piece)

    bx, by, best, best_piece, rotation = px, py, typemin(Float64), piece, 1

    for n in 1:n_rotations
        for dx in -10:10
            if is_piece_inside_bounds_x(piece, px + dx)
                last_dy = -300
                found = false

                for dy in -10:20
                    if is_piece_inside_bounds(piece, px + dx, py + dy)
                        if does_piece_fits(gs.board, piece, px + dx, py + dy)
                            last_dy = dy
                            found   = true
                        else
                            store_board(gs.board)

                            place_piece(gs.board, piece, px + dx, py + last_dy)
                            w = evaluate_board(gs, ga)

                            if w > best
                                bx, by, best, rotation = px + dx, py + last_dy, w, n
                                best_piece = piece
                                print_board(gs.board)
                                @printf("%3d %3d\n", bx, by)
                            end

                            gs.board = load_board()
                            break
                        end
                    elseif found
                        store_board(gs.board)

                        place_piece(gs.board, piece, px + dx, py + last_dy)
                        w = evaluate_board(gs, ga)

                        if w > best
                            bx, by, best, rotation = px + dx, py + last_dy, w, n
                            best_piece = piece
                            print_board(gs.board)
                            @printf("%3d %3d\n", bx, by)
                        end

                        gs.board = load_board()
                        break
                    end
                end
            end
        end
        piece = get_next_piece_rotation(piece)
    end

    gs.piece_pending = true

    return bx, by, rotation, best_piece
end

function does_piece_fits(board :: BitArray{2}, piece_id :: Int64, x :: Int64, y :: Int64)
    piece = get_piece_offsets(piece_id)

    if board[y + piece[1, 2], x + piece[1, 1]] return false end
    if board[y + piece[2, 2], x + piece[2, 1]] return false end
    if board[y + piece[3, 2], x + piece[3, 1]] return false end
    if board[y + piece[4, 2], x + piece[4, 1]] return false end

    return true
end

function is_piece_inside_bounds_x(piece_id :: Int64, x :: Int64) :: Bool
    piece = get_piece_offsets(piece_id)

    if x + piece[1, 1] < 1 || x + piece[1, 1] > 10 return false end
    if x + piece[2, 1] < 1 || x + piece[2, 1] > 10 return false end
    if x + piece[3, 1] < 1 || x + piece[3, 1] > 10 return false end
    if x + piece[4, 1] < 1 || x + piece[4, 1] > 10 return false end

    return true
end

function is_piece_inside_bounds(piece_id :: Int64, x :: Int64, y :: Int64) :: Bool
    piece = get_piece_offsets(piece_id)

    if x + piece[1, 1] < 1 || x + piece[1, 1] > 10 return false end
    if y + piece[1, 2] < 1 || y + piece[1, 2] > 17 return false end

    if x + piece[2, 1] < 1 || x + piece[2, 1] > 10 return false end
    if y + piece[2, 2] < 1 || y + piece[2, 2] > 17 return false end

    if x + piece[3, 1] < 1 || x + piece[3, 1] > 10 return false end
    if y + piece[3, 2] < 1 || y + piece[3, 2] > 17 return false end

    if x + piece[4, 1] < 1 || x + piece[4, 1] > 10 return false end
    if y + piece[4, 2] < 1 || y + piece[4, 2] > 17 return false end

    return true
end

function place_piece(board :: BitArray{2}, piece_id :: Int64, x :: Int64, y :: Int64)
    piece = get_piece_offsets(piece_id)

    board[y + piece[1, 2], x + piece[1, 1]] = true
    board[y + piece[2, 2], x + piece[2, 1]] = true
    board[y + piece[3, 2], x + piece[3, 1]] = true
    board[y + piece[4, 2], x + piece[4, 1]] = true
end
