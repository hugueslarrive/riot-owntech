@page getting_started OwnTech getting started guide

# OwnTech getting started guide

# Architectures

RIOT OS supports many hardware architectures like stm32, avr, mips,
xtensa, msp430, etc.

In addition to these architectures, it offers the possibility of
compiling a project in the form of a 32-bit elf binary that can be run
under linux for testing ("native" architecture).

As current PCs are of 64-bit architecture, it is necessary to activate
multiarch-support on Debian / Ubuntu systems:

    dpkg --add-architecture i386

This command does not return anything but we can check the supported
architectures. The `dpkg --print-architecture` returns the main
architecture of the system and the `dpkg --print-foreign-architectures`
returns the other active architectures enabled by dpkg --add-architecture.

Once the architecture is activated, it is necessary to update the
package database:

    apt-get update

Now we can install an i386 architecture package by adding "`:i386`"
after its name in an apt-get command.


# Dependencies

## Build-essential and native architecture (i386)

    apt-get install libc6-dev-i386 libc6-dbg:i386 build-essential \
    pkg-config uml-utilities bridge-utils git unzip


## For STM32

    apt-get install openocd gcc-arm-none-eabi


# Repositories

    git clone https://gitlab.laas.fr/owntech/riot-owntech
    git clone https://github.com/RIOT-OS/RIOT.git


# Tests

## native arch

    cd RIOT/examples/default
    make
    Building application "default" for "native" with MCU "native".

    "make" -C /home/hugues/RIOT/boards/native
    "make" -C /home/hugues/RIOT/boards/native/drivers
    "make" -C /home/hugues/RIOT/core
    "make" -C /home/hugues/RIOT/cpu/native
    "make" -C /home/hugues/RIOT/cpu/native/netdev_tap
    "make" -C /home/hugues/RIOT/cpu/native/periph
    "make" -C /home/hugues/RIOT/cpu/native/stdio_native
    "make" -C /home/hugues/RIOT/cpu/native/vfs
    "make" -C /home/hugues/RIOT/drivers
    "make" -C /home/hugues/RIOT/drivers/netdev_eth
    "make" -C /home/hugues/RIOT/drivers/periph_common
    "make" -C /home/hugues/RIOT/drivers/saul
    "make" -C /home/hugues/RIOT/drivers/saul/init_devs
    "make" -C /home/hugues/RIOT/sys
    "make" -C /home/hugues/RIOT/sys/auto_init
    "make" -C /home/hugues/RIOT/sys/fmt
    "make" -C /home/hugues/RIOT/sys/iolist
    "make" -C /home/hugues/RIOT/sys/net/gnrc
    "make" -C /home/hugues/RIOT/sys/net/gnrc/netapi
    "make" -C /home/hugues/RIOT/sys/net/gnrc/netif
    "make" -C /home/hugues/RIOT/sys/net/gnrc/netif/ethernet
    "make" -C /home/hugues/RIOT/sys/net/gnrc/netif/hdr
    "make" -C /home/hugues/RIOT/sys/net/gnrc/netif/init_devs
    "make" -C /home/hugues/RIOT/sys/net/gnrc/netreg
    "make" -C /home/hugues/RIOT/sys/net/gnrc/pkt
    "make" -C /home/hugues/RIOT/sys/net/gnrc/pktbuf
    "make" -C /home/hugues/RIOT/sys/net/gnrc/pktbuf_static
    "make" -C /home/hugues/RIOT/sys/net/gnrc/pktdump
    "make" -C /home/hugues/RIOT/sys/net/link_layer/l2util
    "make" -C /home/hugues/RIOT/sys/net/netif
    "make" -C /home/hugues/RIOT/sys/od
    "make" -C /home/hugues/RIOT/sys/phydat
    "make" -C /home/hugues/RIOT/sys/ps
    "make" -C /home/hugues/RIOT/sys/saul_reg
    "make" -C /home/hugues/RIOT/sys/shell
    "make" -C /home/hugues/RIOT/sys/shell/commands
       text	   data	    bss	    dec	    hex	filename
      91657	   1288	  72088	 165033	  284a9	/home/hugues/RIOT/examples/defaultt/bin/native/default.elf

So "native" toolchain is working.

## stm32

    cd ../../../riot-owntech/tests_v0.9
    make
    [...]

You should end up with something like:

     text      data	    bss	    dec	    hex	filename
    24488	    168	   4272	  28928	   7100	/home/hugues/owntech/firmware/riot-owntech/tests_v0.9/bin/nucleo-f334r8/test_v0.9.elf
    
So "arm-none-eabi" toolchain is working.

# GNU Make based RIOT build system

Very easy: `make all` to build the project, `make flash` to upload it
into the board flash memory, `make term` to launch pyterm so:

    make all flash term

You will end up with the RIOT shell:

    Done flashing
    /home/hugues/owntech/firmware/RIOT/dist/tools/pyterm/pyterm -p "/dev/ttyACM0" -b "115200"  
    Twisted not available, please install it if you want to use pyterm's JSON capabilities
    2020-10-24 02:20:19,211 # Connect to serial port /dev/ttyACM0
    Welcome to pyterm!
    Type '/exit' to exit.
    help
    2020-10-24 02:20:40,622 # ?help

As you can see pyterm is a pretty-buggy python script but it work good anyway :)

# Now it's time to play !

    2020-10-24 02:20:40,622 # help
    > help
    2020-10-24 02:20:43,504 #  help
    2020-10-24 02:20:43,506 # Command              Description
    2020-10-24 02:20:43,510 # ---------------------------------------
    2020-10-24 02:20:43,514 # legset               set inverter leg duty cycle
    2020-10-24 02:20:43,519 # sinadd               add legs to sin50hz thread
    2020-10-24 02:20:43,523 # sinrm                remove legs from sin50hz thread
    2020-10-24 02:20:43,528 # getysc               call sin50hz_get_yscale()
    2020-10-24 02:20:43,533 # setysc               call sin50hz_set_yscale(uint16_t scale)
    2020-10-24 02:20:43,539 # setshift             call sin50hz_set_shift(uint16_t shift)
    2020-10-24 02:20:43,545 # sin_bench            start a sine wave with sine osicllating amplitude
    2020-10-24 02:20:43,550 # daq_meas             acquire a measure from a leg sensor
    2020-10-24 02:20:43,554 # reboot               Reboot the node
    2020-10-24 02:20:43,558 # version              Prints current RIOT_VERSION
    2020-10-24 02:20:43,564 # pm                   interact with layered PM subsystem
    2020-10-24 02:20:43,569 # ps                   Prints information about running threads.
    > legset
    2020-10-24 02:51:21,402 #  legset
    2020-10-24 02:51:21,405 # usage: legset <dev> <val> <pha>
    2020-10-24 02:51:21,409 # 	dev: device by number between 1 and 2
    2020-10-24 02:51:21,410 # 	val: duty cycle
    2020-10-24 02:51:21,412 # 	pha: phase shift
    2020-10-24 02:51:21,412 # 

The `help` riot shell command give a list of available commands with a
short description and you must input the command without any argument to
display it's usage.

TODO: add phase shift in the legset short description.

    > sinadd
    2020-10-24 03:12:54,886 #  sinadd
    2020-10-24 03:12:54,889 # usage: sinadd <leg> <leg> [leg]
    2020-10-24 03:12:54,893 # 	dev: device by number between 1 and 2

TODO: s/dev/leg/

    > sinadd 1 2
    2020-10-24 03:15:21,791 #  sinadd 1 2

You will end up with a 50Hz sine wave between Vin1 and Vin2 with
(0.8 x Vout) amplitude. In this example "in" and "out" are reversed, if
you are wondering why not Vhigh and Vlow as usual ask Jean.

Now with randoms used power adapter and AC you can supply 50Hz
synchronous motor from 60Hz US power gride to tune up your
european Hammond L122 organ for example !

I said "randoms" but they must be appropriate: you can count aprox.
100 VA/Kg for toroidals transformers and the power adapter must have an
output voltage greater than 1 / 0.8 of that of transformer secondary's
peak voltage (RMS voltage usually given x 1.41). 2x is a good option. A
24 volts power adapter for a 12-230v transformer for example.

So you need to adjust the gain (0.8 by default) to feet your needs:

gain = 12 * 1.41 / 24 = 0.7

    > getysc
    2020-10-24 03:39:41,062 #  getysc
    2020-10-24 03:39:41,063 # 18432

18432 / 0.8 * 0.7 = 16128

    > setysc 16128
    2020-10-24 04:22:32,548 #  setysc 16128

There is some power losses and as we use a power transformer upside
down (unfavorable case) that need to be adjusted with a multimeter and
can be up to 18432.

Now you can edit `main.c` and add the following code to the `main()`
function:

```C
/** - Add power script here **/
sin50hz_add2p(1, 2);
sin50hz_set_yscale(16128);
```

Then launch `make all flash` to upload your valuable firmware to the
board.

