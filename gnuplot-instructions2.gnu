set terminal png size 1600,1000 enhanced font 'Verdana,10'
# Line width of the axes
set border linewidth 1.5


# Axes label
set xlabel 'distance'
set ylabel 'synd\_weight'

# Axes ranges
set xrange [1200:1450]
set yrange [0:40000]

set title "Syndrom weight frequency (1 block, N tries)"

# Each bar is half the (visual) width of its x-range.
set boxwidth 1 absolute
set style fill solid 1.0 noborder
set nokey 

Gauss(x,mu,sigma) = 1./(sigma*sqrt(2*pi)) * exp( -(x-mu)**2 / (2*sigma**2) )

# Plot
plot './dat/2.dat' using 1:2 smooth frequency with boxes

# Print the Gaussian ???
# Gauss(x,1324,1000)
