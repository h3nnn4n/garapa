queue_bg_text    = []
queue_rectangles = []

function garapa_draw_test_with_bg( text :: AbstractString, x :: Int, y :: Int, r :: Int, g :: Int, b :: Int)
    push!(queue_bg_text, (text, x, y, r, g, b))
end

function garapa_draw_rectangle( x :: Int, y :: Int, x2 :: Int, y2 :: Int, r :: Int, g :: Int, b :: Int)
    push!(queue_rectangles, (x, y, x2, y2, r, g, b))
end

function drawer()
    if length(queue_bg_text) > 0
        for q in queue_bg_text
            (text, x, y, r, g, b) = q
            ccall(:draw_text_with_bg, Void, (Cstring, Int, Int, Int, Int, Int), text, x, y, r, g, b)
        end

        for _ in 1:length(queue_bg_text)
            pop!(queue_bg_text)
        end
    end

    if length(queue_rectangles) > 0
        for q in queue_rectangles
            (x, y, x2, y2, r, g, b) = q
            ccall(:draw_rectangle, Void, (Int, Int, Int, Int, Int, Int, Int), x, y, x2, y2, r, g, b)
        end

        for _ in 1:length(queue_rectangles)
            pop!(queue_rectangles)
        end
    end
end

