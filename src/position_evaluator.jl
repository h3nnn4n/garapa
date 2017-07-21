include("types.jl")
include("feature_functions.jl")
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

function evaluate_board(gs :: game_state)

    px = div(garapa_read_byte(0xff92) - 8 , 8)
    py = div(garapa_read_byte(0xff93) - 16, 8)

    bx, by = px, py
    best = 0

    piece = garapa_read_byte(0xc203)

    for dx in -10:10
        for dy in -10:20
            if is_piece_inside_bounds(piece, dx, dy) && does_piece_fits(gs.board, piece, px + dx, py + dy)
                store_board(gs.board)
                @printf("%4d %4d\n", px + dx, py + dy)
                place_piece(gs.board, piece, px + dx, py + dy)
                print_board(gs.board)
                @printf("\n\n")
                gs.board = load_board()
            end
        end
    end

end

function does_piece_fits(board :: BitArray{2}, piece_id :: Int64, x :: Int64, y :: Int64)
    piece = get_piece_offsets(piece_id)

    if board[x + piece[2, 1], y + piece[1, 2]] return false end
    if board[x + piece[2, 1], y + piece[2, 2]] return false end
    if board[x + piece[3, 1], y + piece[3, 2]] return false end
    if board[x + piece[4, 1], y + piece[4, 2]] return false end

    return true
end

function is_piece_inside_bounds(piece_id :: Int64, dx :: Int64, dy :: Int64) :: Bool
    px = div((garapa_read_byte(0xff92) - 8 ), 8) + dx
    py = div((garapa_read_byte(0xff93) - 16), 8) + dy

    piece = get_piece_offsets(piece_id)

    if px + piece[1, 1] < 1 || px + piece[1, 1] > 10 return false end
    if py + piece[1, 2] < 1 || py + piece[1, 2] > 17 return false end

    if px + piece[2, 1] < 1 || px + piece[2, 1] > 10 return false end
    if py + piece[2, 2] < 1 || py + piece[2, 2] > 17 return false end

    if px + piece[3, 1] < 1 || px + piece[3, 1] > 10 return false end
    if py + piece[3, 2] < 1 || py + piece[3, 2] > 17 return false end

    if px + piece[4, 1] < 1 || px + piece[4, 1] > 10 return false end
    if py + piece[4, 2] < 1 || py + piece[4, 2] > 17 return false end

    return true
end

function place_piece(board :: BitArray{2}, piece_id :: Int64, x :: Int64, y :: Int64)
    piece = get_piece_offsets(piece_id)

    board[x + piece[1, 1], y + piece[1, 2]] = true
    board[x + piece[2, 1], y + piece[2, 2]] = true
    board[x + piece[3, 1], y + piece[3, 2]] = true
    board[x + piece[4, 1], y + piece[4, 2]] = true
end
