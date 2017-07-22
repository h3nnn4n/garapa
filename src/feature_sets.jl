include("types.jl")
include("feature_functions.jl")

feature_basic = [aggegrate_height, number_of_holes, surface_variance, cleared_rows]

function get_raw_features( gs :: game_state, ga :: ga_brain ) :: Array{Int64, 1}
    values = zeros(ga.number_of_features)

    for i in 1:ga.number_of_features
        values[i] = feature_basic[i](gs.board)
    end

    return values
end
