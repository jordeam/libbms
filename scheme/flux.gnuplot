#set terminal postscript color
#set output "gwplot.dat"
set multiplot
set size 1,0.333333

set origin 0,0.66666
set nologscale y
plot  "gwplot.dat.0" using 1:2 title "I(R1)" with lines, \
 "gwplot.dat.1" using 1:2 title "V(C1)" with lines

set origin 0,0.33333
set nologscale y
plot  "gwplot.dat.2" using 1:2 title "V(R3)" with lines

set origin 0,0.0
set nologscale y
plot  "gwplot.dat.2" using 1:2 title "V(R3)" with lines

