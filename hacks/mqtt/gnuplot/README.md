@page mqtt  mqtt/gnuplot - sending measures to gnuplot over UDP
# mqtt/gnuplot - sending measures to gnuplot over UDP

The "measure_transmit" thread currently sending lines of 8 space separated
measures in udp on port 8888.

You can display a real time plot of this measures with gnuplot.

First you need to set a target ipv6 address in the sock_udp_addr_set()
function call in `main.c` and issue a `make all flash term`.

Then you need to cd to gnuplot directory and launch the server with
`nc -6luk 8888 | tee test`.

Finally you can launch gnuplot in another terminal with
`gnuplot -c config.gnuplot`. The gnuplot window will ever be reopened
if you close it so you must use Ctrl+c in the terminal to do so.

