@page ownpower nucleo-f303k8 on OwnWall v4.3 board retrofit

# nucleo-f303k8 on OwnWall v4.3 board retrofit

This is a firmware for use a nucleo-f303k8 stm32 instead of a nano avr
on the OwnWall v4.3 board. As the stm32 have 3.3v io pins you need to
make small modifications to the board. See `3v3mod.jpg`.

# RIOTBASE

First of all you must set the path to the RIOT source tree in the
Makefile before building. Default is `../../../RIOT`.

You may revert back to the last known functional commit by issuing

`git checkout 46fc1f2ae678a859b06ab3654d6b5344b759ed39` in your RIOT
directory and return to master later with `git checkout master`.

# Correction

A correction algorithm was implemented in `correction.c` and
`correction.h` files. The goal is to use the board to transfert energy
from 24v battery to 12v battery.

The correction function is called every 160µs by the measure thread.
First it does safety checks (24v battery sufficiantly charged, 12v
battery is not full, the current is going in the right direction, the
current isn't too high, and there is no overheat), then it try to
startup the converter with a computed duty-cycle (VL/VH) then it track
the maximum charging voltage or current and finally it set the pwm
duty-cycle.


