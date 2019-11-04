
set terminal png size 2048,1600 enhanced
set terminal png font "Garamond"
set output "cache_size.png"


set lmargin 14
set rmargin 14
set bmargin 9
set tmargin 4

set xlabel "SET SIZE (in L1 double capacity)" font ", 40" offset 0,-4
set ylabel ""

set tics font ", 32"
set xtics offset 0,-1.5

set yrange [0:0.14]
set y2range [0:0.22]
set ytics nomirror
set y2tics nomirror

set key on inside center right font ", 32"
plot "cache_size.dat" u ($2/4096):14 axis x1y2 w p ps 4 pt 3 lc -1 title "L2 miss", "" u ($2/4096):10 w lp ps 4 pt 7 lw 2 lc 1 title "L1 miss"


set output "cache_size.cyc.png"
set yrange [0:0.22]
set y2range [0.5:0.85]

set y2tics nomirror

set key on inside center right font ", 32"
plot "cache_size.dat" u ($2/4096):14 axis x1y1 w p ps 4 pt 3 lc -1 title "L2 miss", "" u ($2/4096):6 axis x1y2 w lp ps 4 pt 7 lw 2 lc 1 title "Cycle per el."


set terminal x11 enhanced
