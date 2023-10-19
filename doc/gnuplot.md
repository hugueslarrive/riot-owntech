@page gnuplot   measure_transmit to gnuplot

# measure_transmit to gnuplot

The "measure_transmit" thread currently sending lines of space separated
measures over serial or udp port 8888.

You can display a real time plot of this measures with gnuplot.

# transmit over serial

First you need to setup the serial line with
`stty -F /dev/ttyACM0 115200 raw -echo`.

Then you need to cd to gnuplot directory and launch logging with
`cat /dev/ttyACM0 | tee test`.

# transmit over udp port 8888

First you need to set a target ipv6 address in the sock_udp_addr_set()
function call in main.c and issue a `make all flash term`.

Then you need to cd to measure_test directory and launch the server with
`nc -6luk 8888 | tee test`.

# gnuplot

Finally you can launch gnuplot in another terminal with
`gnuplot -c config.gnuplot`. The gnuplot window will ever be reopened
if you close it so you must use Ctrl+c in the terminal to do so.

