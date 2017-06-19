set terminal png size 1600,1000 enhanced font 'Verdana,10'
# Line width of the axes
set border linewidth 1.5

stats './dat/3_0.dat' using 1 name "sw0"
stats './dat/3_1.dat' using 1 name "sw1"


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

#stats './dat/2.dat' using 5

Gauss(x,mu,sigma) = 1./(sigma*sqrt(2*pi)) * exp( -(x-mu)**2 / (2*sigma**2) )
sw_theo = 661.9173794122203
sw_tot = (sw0_records*sw0_mean + sw1_records*sw1_mean) / (sw0_records + sw1_records)

set arrow from sw0_mean,graph(0,0) to sw0_mean,graph(1,1) nohead lt rgb "red"
set arrow from sw1_mean,graph(0,0) to sw1_mean,graph(1,1) nohead lt rgb "blue"
set arrow from sw_tot,graph(0,0) to sw_tot,graph(1,1) nohead lt rgb "green"

# Plot
plot './dat/2.dat' using 1:2 pt 7 lt rgb "red" , \
    './dat/2.dat' using 1:3 pt 7  lt rgb "blue", \
    './dat/2.dat' using 1:($2+$3) pt 7  lt rgb "green", \
#     Gauss(x,sw0_mean,sw0_stddev) , \
#     Gauss(x,sw1_mean,sw1_stddev)
#show variables all


# Print the Gaussian ???
# Gauss(x,1324,1000)
