include("types.jl")

bsize_x = 10
bsize_y = 17

function aggegrate_height( board :: BitArray{2} )
    total = 0

    for x = 1:10
        for y = 1:17
            if board[y, x]
                total += 18 - y
                break
            end
        end
    end

    return total
end

