set terminal png size 1600,1000 enhanced font 'Verdana,10'
# Line width of the axes
set border linewidth 1.5


# Axes label
set xlabel 'distance'
set ylabel 'synd\_weight'

# Axes ranges
set xrange [0:2401]
set yrange [1318:1326]

set title "Average syndrom weight per distance (1 block, N tries)"

set palette model RGB defined (0 "black", 1 "red", 2 "blue", 3 "green", 4 "pink")
unset colorbox
set nokey 

s0 = 1324
delta = 0.6
s(i) = s0 - i*delta

#set arrow from 0,s(0) to 2400,s(0) nohead lt rgb "red"
#set arrow from 0,s(1) to 2400,s(1) nohead lt rgb "red"
#set arrow from 0,s(2) to 2400,s(2) nohead lt rgb "red"


# Plot
plot './dat/1.dat' using 1:2:3 pt 7 ps 0.5 palette