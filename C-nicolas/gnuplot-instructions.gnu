s = 3.0405
e = 0.0005

mytitle = 'Average number of rounds for decryption for each distance in the spectrum (80 bit security parameters, 10^8 tries)'

set terminal png size 1600,1000 enhanced font 'Verdana,10'
# Line width of the axes
set border linewidth 1.5

# Axes label
set xlabel 'distance'
set ylabel 'average number of rounds'

# Axes ranges
set xrange [0:2401]
set yrange [s-3*e:s+2*e]

set title mytitle

set palette model RGB defined (0 "black", 1 "red", 2 "blue", 3 "green", 4 "brown")
unset colorbox
set nokey 

# Plot
plot './out5.csv' using 1:2:3 pt 7 ps 0.5 palette
