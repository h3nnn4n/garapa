function print_board( board :: BitArray{2})
    @printf("+---------------------+\n")
    for y in 1:17
        @printf("| ")
        for x in 1:10
            if board[y, x]
                @printf("X ")
            else
                @printf("  ")
            end
        end
        @printf("|\n")
    end

    @printf("+---------------------+\n")
    @printf("\n")
end

