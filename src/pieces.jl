include("types.jl")
include("j_utils.jl")


function get_current_piece() :: Int64
    return garapa_read_byte(0xc203)
end

function get_preview_piece() :: Int64
    return garapa_read_byte(0xc213)
end

function get_hidden_piece() :: Int64
    return garapa_read_byte(0xc213)
end
