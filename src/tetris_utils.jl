function print_board( board :: BitArray{2})
    x, y = size(board)

    @printf("+---------------------+\n")

    for i in 1:x
        @printf("| ")
        for j in 1:y
            @printf("%s ", board[i, j] ? "X" : " ")
        end
        @printf("|\n")
    end

    @printf("+---------------------+\n")

    @printf("\n")
end
