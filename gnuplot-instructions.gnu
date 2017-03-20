set terminal png size 1200,600 enhanced font 'Verdana,10'
# Line width of the axes
set border linewidth 1.5


# Axes label
set xlabel 'distance'
set ylabel 'synd\_weight'

# Axes ranges
set xrange [0:2401]
set yrange [1319:1325]

set title "Average syndrom weight per distance (1 block, 10^6 tries)"

set palette model RGB defined (0 "black", 1 "red", 2 "blue", 3 "green", 4 "pink")
unset colorbox
set nokey 

# Plot
plot './dat/1.dat' using 1:2:3 pt 7 ps 0.5 palette