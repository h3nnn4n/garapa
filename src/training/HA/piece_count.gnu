#!/usr/bin/gnuplot

reset

#set terminal pngcairo size 1600,600 enhanced font 'Verdana,9'
set terminal pngcairo size 650,300 enhanced dashed font 'Verdana,10'
set output 'piece_count.png'

set style line 11 lc rgb '#808080' lt 1
set border 3 front ls 11
set tics nomirror
set style line 12 lc rgb'#808080' lt 0 lw 1
set grid back ls 12

set style line 1 lt 1 lw 2 lc rgb '#1B9E77' # dark teal
set style line 2 lt 1 lw 2 lc rgb '#D95F02' # dark orange
set style line 3 lt 1 lw 2 lc rgb '#7570B3' # dark lilac
set style line 4 lt 1 lw 2 lc rgb '#E7298A' # dark magenta
set style line 5 lt 1 lw 2 lc rgb '#66A61E' # dark lime green
set style line 6 lt 1 lw 2 lc rgb '#E6AB02' # dark banana
set style line 7 lt 1 lw 2 lc rgb '#A6761D' # dark tan
set style line 8 lt 1 lw 2 lc rgb '#666666' # dark gray

#set key top left
#set key above width -2 vertical maxrows 2
set key top left vertical maxrows 2

#set key samplen 4

set title 'Piece Count'

set xlabel 'Number of games'
set ylabel 'Total of pieces'

#set xtics rotate by -55
#set xtics 4

plot \
'log_pieces.txt'     u 1:2 t 'O'     w l ls 1, \
'log_pieces.txt'     u 1:3 t 'I'     w l ls 2, \
'log_pieces.txt'     u 1:4 t 'S'     w l ls 3, \
'log_pieces.txt'     u 1:5 t 'Z'     w l ls 4, \
'log_pieces.txt'     u 1:6 t 'L'     w l ls 5, \
'log_pieces.txt'     u 1:7 t 'J'     w l ls 6, \
'log_pieces.txt'     u 1:8 t 'T'     w l ls 7, \
