@enum SCREEN boot credits menu gametype ingame atype gameover

# Screen codes
# 0xff this seems to be the initial value, maybe it is just junk from my RAM
# 0x24 a pre credits menu frame
# 0x25 is the credits screen
# 0x06 the transition between the credits and menu
# 0x07 is the main menu
# 0x08 transition to the gametype screen
# 0x0e the gametype menu
# 0x10 transition to the A-Type game screen
# 0x11 A-Type level select screen
# 0x00 The game screen
# 0x01 transition screen
# 0x0d the animation to the gameover screen
# 0x04 the gameover screen, or the waiting screen after the B-Type game score screen
# 0x10 transition to the A-Type game screen
# 0x15 A/B-Type score screen, where you can enter your name for a high score. The id is the same for every position on the highscore list
# 0x12 transition to the A/B-Type game screen
# 0x13 B-Type game screen
# 0x05 Transition to the B-Type game score screen
# 0x0b B-Type game score screen
# 0x34 This is one of the winning screens. I was afk when I got it, so I am not sure what code corresponds to what part
# 0x2e same as ab:ve
# 0x2f same as ab:ve
# 0x30 same as above
# 0x31 same as above
# 0x32 same as above
# 0x33 same as above

type game_state
    board  :: BitArray{2}
    screen :: SCREEN

    px :: Int64
    py :: Int64

    bsize_x :: Int64
    bsize_y :: Int64
end

type agent
end

type ga_brain
    mutation_rate  :: Float64
    crossover_rate :: Float64
    population     :: Array{agent, 1}
end
