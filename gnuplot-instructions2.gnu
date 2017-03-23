set terminal png size 1600,1000 enhanced font 'Verdana,10'
# Line width of the axes
set border linewidth 1.5

# Axes label
set xlabel 'distance'
set ylabel 'synd\_weight / 2'

# Axes ranges
set xrange [625:700]
set yrange [0:0.04]

set title "Syndrom weight frequency (1 block, N tries)"

# Each bar is half the (visual) width of its x-range.
set boxwidth 1 absolute
set style data linespoints 
#set style fill transparent solid 0.5 noborder
set nokey 

stats './dat/2.dat' using 3
stats './dat/2.dat' using 2

Gauss(x,mu,sigma) = 1./(sigma*sqrt(2*pi)) * exp( -(x-mu)**2 / (2*sigma**2) )
#m = STATS_mean
#s = STATS_stddev


# Plot
plot './dat/2.dat' using 1:2 pt 7 , \
     './dat/2.dat' using 1:3 pt 7 
#     Gauss(x,m,s)
#show variables all


# Print the Gaussian ???
# Gauss(x,1324,1000)
