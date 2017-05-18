#!/usr/bin/gnuplot
#
# Colored tics with the epslatex terminal
#
# AUTHOR: Hagen Wierstorf

reset

# epslatex
#set terminal epslatex size 10.4cm,6.35cm color colortext standalone 'phv,9' \
#header '\definecolor{t}{rgb}{0.5,0.5,0.5}'
#set output 'epslatex_correct.tex'
# wxt
#set terminal wxt size 410,250 enhanced font 'Verdana,9' persist
# png
#set terminal pngcairo size 1600,600 enhanced font 'Verdana,9'
set terminal pngcairo size 650,300 enhanced dashed font 'Verdana,10'
set output 'means2.png'
# svg
#set terminal svg size 410,250 fname 'Verdana, Helvetica, Arial, sans-serif' fsize '9' rounded dashed
#set output 'nice_web_plot.svg'

# define axis
# remove border on top and right and set color to gray
set style line 11 lc rgb '#808080' lt 1
set border 3 front ls 11
set tics nomirror
# define grid
set style line 12 lc rgb'#808080' lt 0 lw 1
set grid back ls 12

#show style lines

# color definitions
#set style line 1 lc rgb '#000000' pt 1 ps 1.5 lw 1
#set style line 2 lc rgb '#000000' pt 2 ps 1.5 lw 1
#set style line 3 lc rgb '#000000' pt 3 ps 1.5 lw 1
#set style line 4 lc rgb '#000000' pt 4 ps 1.5 lw 1
#set style line 5 lc rgb '#000000' pt 8 ps 1.5 lw 1
#set style line 6 lc rgb '#000000' pt 6 ps 1.5 lw 1

set style line 1 lt 1 lw 2 lc rgb '#1B9E77' # dark teal
set style line 2 lt 1 lw 2 lc rgb '#D95F02' # dark orange
set style line 3 lt 1 lw 2 lc rgb '#7570B3' # dark lilac
set style line 4 lt 1 lw 2 lc rgb '#E7298A' # dark magenta
set style line 5 lt 1 lw 2 lc rgb '#66A61E' # dark lime green
set style line 6 lt 1 lw 2 lc rgb '#E6AB02' # dark banana
set style line 7 lt 1 lw 2 lc rgb '#A6761D' # dark tan
set style line 8 lt 1 lw 2 lc rgb '#666666' # dark gray

set key top right

set key samplen 4

#set title 'Average Fitness'

#set format '$'
set xlabel 'Generation'
set ylabel 'Average Number of Cleared Lines'

set xrange [0:]
#set xrange [0:30]
set yrange [0:1200]
set xtics rotate by -55

plot \
'log_CMA_avg.txt'     u (column(0)):2 t 'CMA'     w l ls 1, \
'log_HA_avg.txt'      u (column(0)):2 t 'HA'      w l ls 3, \
'log_LELmark_avg.txt' u (column(0)):2 t 'LELmark' w l ls 5, \
'log_ALL_avg.txt'     u (column(0)):2 t 'ALL'     w l ls 6









