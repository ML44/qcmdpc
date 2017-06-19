set terminal png size 1600,1000 enhanced font 'Verdana,10'
# Line width of the axes
set border linewidth 1.5


# Axes label
set xlabel 'distance'
set ylabel 'synd\_weight'

# Axes ranges
set xrange [0:2401]
set yrange [s-5:s+2]

set title mytitle

set palette model RGB defined (0 "black", 1 "red", 2 "blue", 3 "green", 4 "pink")
unset colorbox
set nokey 

lim0 = s0
lim1 = s1

set arrow 1 from 0,s0 to 2401,s0 nohead filled back lw 0 lc rgb "black" 
set arrow 10 from 0,lim0 to 2401,lim0 nohead filled back lw 2 lc rgb "black" 
set arrow 2 from 0,s1 to 2401,s1 nohead filled back lw 0 lc rgb "red" 
set arrow 20 from 0,lim1 to 2401,lim1 nohead filled back lw 2 lc rgb "red" 
set arrow 3 from 0,s to 2401,s nohead  filled back lt 0 lw 0 lc rgb "black"


# Plot
plot './dat/1.dat' using 1:2:3 pt 7 ps 0.5 palette