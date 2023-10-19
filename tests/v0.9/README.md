@page v0_9  v0.9 OwnTech board test firmware

# v0.9 OwnTech board test firmware

This is a firmware for testing v0.9 OwnTech board.


# RIOTBASE

First of all you must set the path to the RIOT source tree in the
Makefile before building. Default is `../../RIOT`.

You may revert back to the last known functional commit by issuing

`git checkout c270aadc00cd043f9dfbe2c7459b6fac1aaf9729` in your RIOT
directory and return to master later with `git checkout master`.



# nucleo-f334r8

The nucleo-f334r8 board directory was duplicated here to be able to
modify the ADC configuration. So `EXTERNAL_BOARD_DIRS` is set to
`../../boards` in Makefile.


# gnuplot

The "measure_transmit" thread currently sending lines of 7 space
separated measures over serial.

You can display a real time plot of this measures with gnuplot.

First you need to setup the serial line with
`stty -F /dev/ttyACM0 115200 raw -echo`.

Then you need to cd to gnuplot directory and launch logging with
`cat /dev/ttyACM0 | tee test`.

Finally you can launch gnuplot in another terminal window with
`gnuplot -c config.gnuplot`. The gnuplot window will ever be reopened
if you close it so you must use Ctrl+c in the terminal to do so.


