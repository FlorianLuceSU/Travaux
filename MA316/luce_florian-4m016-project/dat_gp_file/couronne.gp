set title "Please, click to select a point"
set xlabel "x"
set ylabel "y"
set grid
set xrange [-0.200000:14.200000]
set yrange [0.600000:5.400000]
set style line 1 lt 1
set mouse
bind "Button1" "set print 'dat_gp_file/chosen_point.dat'; print MOUSE_X, MOUSE_Y; unset print; exit gnuplot"
plot "dat_gp_file/couronne.dat" using 1:2 with lines lw 2
pause mouse close
