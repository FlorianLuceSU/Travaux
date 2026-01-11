set title "Please, click to select a point"
set xlabel "x"
set ylabel "y"
set grid
set xrange [0.300000:8.700000]
set yrange [0.400000:7.600000]
set style line 1 lt 1
set mouse
bind "Button1" "set print 'dat_gp_file/chosen_point.dat'; print MOUSE_X, MOUSE_Y; unset print; exit gnuplot"
plot "dat_gp_file/etoile.dat" using 1:2 with lines lw 2
pause mouse close
