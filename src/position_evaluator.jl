include("types.jl")
include("feature_functions.jl")
include("pieces.jl")
include("draw.jl")

stack = Array{BitArray{2}, 1}[]

function store_board(board :: BitArray{2})
    push!(stack, board)
end

function load_board()
    return pop!(stack)
end

function evaluate_board(gs :: game_state)
    #=store_board(gs.board)=#

    px = garapa_read_byte(0xc202)
    py = garapa_read_byte(0xc201)

    #=gs.board = load_board()=#
end

