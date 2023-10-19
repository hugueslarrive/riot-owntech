@page tools An stm32f334 discovery board based feeder (5 to 10v boost) and a sine wave C array generator

# An stm32f334 discovery board based feeder (5 to 10v boost) and a sine wave C array generator

# /tools/feeder

An HRTIM use case which use the stm32f334-disco board buck-boost
converter and a small isolated 5V USB SMPS to provide 9-10V to the
OwnTech v0.9 feeder input. GNDref is isolated from GNDD so you must
never use your computer to supply the feeder when it is connected to
the nucleo st-link.


# /tools/singen

This tool allow you to generate the lookup table used by sin50Hz.

First you need to edit `singen_simple.c` and set `SAMPLE_FREQ`,
`SIN_FREQ`, and `TIMER_PERIOD` constants. 

Then you can run
`gcc -o singen_simple singen_simple.c -lm && ./singen_simple`

This will generate a set of varibles that you can copy-paste in
`sin/sin50hz.c`.

If you change the pwm or sample frequency, you need to regenerate them
and also modify the `LEG_RES` and `LEG_FREQ` values in `leg/leg.h`. You
can use the riot shell command `init <LEG_FREQ>` in `tests/periph_hrtim`
to determine `LEG_RES` / `TIMER_PERIOD` value.
