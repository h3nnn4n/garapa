include("j_utils.jl")

function frame_update()
    #=println("Frame!")=#
end

function garapa_init()
    garapa_register_callback_display_update(frame_update)
end

