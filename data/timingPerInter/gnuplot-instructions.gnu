s = 3.045
e = 0.02

mytitle = 'Average number of rounds for decryption depending on the size of the intersection of the spectrums of h and e (80 bit security parameters, 10^8 samples, only considering values appearing in > 10000 samples)'

set terminal png size 1600,1000 enhanced font 'Verdana,10'
# Line width of the axes
set border linewidth 1.5

# Axes label
set xlabel 'cardinal of the intersection'
set ylabel 'average number of rounds'

# Axes ranges
set xrange [100:400]
set yrange [s-2*e:s+2*e]

set title mytitle

set palette model RGB defined (0 "white", 9999 "white", 10000 "black", 100000 "black")
unset colorbox
set nokey 

# Plot
plot './out.dat' using 1:2:3 pt 7 ps 0.5 palette
