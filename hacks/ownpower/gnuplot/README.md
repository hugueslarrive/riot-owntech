@page ownpower  ownpower/gnuplot - sending measures to gnuplot over serial
# ownpower/gnuplot - sending measures to gnuplot over serial

The "measure_transmit" thread currently sending lines of space separated
measures over serial.

You can display a real time plot of this measures with gnuplot.

First you need to setup the serial line with
`stty -F /dev/ttyACM0 115200 raw -echo`.

Then you need to cd to measure_test directory and launch logging with
`cat /dev/ttyACM0 | tee test`.

Finally you can launch gnuplot in another terminal with
`gnuplot -c config.gnuplot`. The gnuplot window will ever be reopened
if you close it so you must use Ctrl+c in the terminal to do so.
