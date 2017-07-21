include("types.jl")

bsize_x = 10
bsize_y = 17

function aggegrate_height( board :: BitArray{2} ) :: Int64
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

function number_of_holes( board :: BitArray{2} ) :: Int64

    return 0
end

function surface_variance( board :: BitArray{2} ) :: Int64

    return 0
end

function cleared_rows( board :: BitArray{2} ) :: Int64
    total = 0

    for x in 1:10
        if reduce((a, b) -> a && b, true, board[x, 1:10])
            total += 1
        end
    end

    return total
end

