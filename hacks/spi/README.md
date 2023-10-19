@page spi nucleo-f746zg to nucleo-f334r8 SPI communication

# nucleo-f746zg to nucleo-f334r8 SPI communication

RIOTBASE is set to `../../../RIOT` in `f334r8/Makefile`.

RIOT HEAD commit is a1bde635d0222206cf553d153237004ee5ae5544.

You may revert back to this commit by issuing

`git checkout a1bde635d0222206cf553d153237004ee5ae5544` in the RIOT
directory and return to master later with `git checkout master`.


# Abstract

During this hackathon I tried to link the nucleo-f746zg to the
nucleo-f334r8 by SPI with f746 as master and f3 as slave.

The first difficulty was to tell openocd which stlink it should connect
to.

The second difficulty encountered was that riot does not implement the
SPI slave mode.


# Tell openocd which stlink it should connect to

When you connect two boards simultaneously to your computer, the usual
`make all flash term` try to flash to the first stlink connected and
launch pyterm on the first USB serial port (ttyACM0) so that does not
work as is for the second board connected.

You set the `PORT_LINUX=/dev/ttyACM1` environement variable which will
be used for pyterm but it has no effect on openocd.

It is necessary to add the stlink's serial number to the openocd
configuration file, you can retrieve it like this :

    hugues@Latitude5400:~/owntech/firmware/riot-owntech$ ls -l /dev/serial/by-id/
    total 0
    lrwxrwxrwx 1 root root 13 juil. 20 23:09 usb-STMicroelectronics_STM32_STLink_0668FF545454885087241526-if02 -> ../../ttyACM0
    lrwxrwxrwx 1 root root 13 juil. 21 00:18 usb-STMicroelectronics_STM32_STLink_066AFF343039564157223543-if02 -> ../../ttyACM1

So if the f334r8 was the firstly connected (ttyACM0) and the f746zg the
second (ttyACM) you must set `hla_serial "0668FF545454885087241526"` in
`f334r3/stm32f3.cfg` and `hla_serial "066AFF343039564157223543"` in
`f746zg/stm32f7.cfg`. I found these two files in
`RIOT/boards/common/stm32/dist/` directory.

Then you can use the OPENOCD_CONFIG environement variable in your
make command line thats indicate to openocd which board to talk to.


# SPI wiring
| SPI  | f746zg | f334r8 |
|:---- |:------ |:------ |
| SCK  | D13    | D13    |
| MISO | D12    | D12    |
| MOSI | D11    | D11    |
| CS   | D10    | D10    |


# Test

`cd` to `hackathon-2/f334r8` directory then `OPENOCD_CONFIG=stm32f3.cfg make all flash term`.

In another terminal `cd` to `../RIOT/tests/periph_spi` directory then
`EXTERNAL_BOARD_DIRS=../../../riot-owntech/boards/ BOARD=nucleo-f746zg PORT_LINUX=/dev/ttyACM1 OPENOCD_CONFIG=../../../riot-owntech/hackathon-2/f746zg/stm32f7.cfg make all flash term`. Sorry :p

In f746zg's RIOT shell:

    init 0 0 0 3 14
    SPI_DEV(0) initialized: mode: 0, clk: 0, cs_port: 3, cs_pin: 14
    send hello
    Sent bytes
       0    1    2    3    4 
      0x68 0x65 0x6c 0x6c 0x6f
        h    e    l    l    o 

    Received bytes
      0    1    2    3    4 
      0x72 0x65 0x61 0x64 0x79
        r    e    a    d    y 

In f334r8's:

     Received bytes
       0    1    2    3    4 
      0x68 0x65 0x6c 0x6c 0x6f
        h    e    l    l    o 
    
    "hello" received

It work !
