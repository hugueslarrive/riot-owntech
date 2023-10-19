@page mqtt nucleo-f746zg to thingsboard MQTT communication

# nucleo-f746zg to thingsboard MQTT communication

# RIOTBASE

First of all you must set the path to the RIOT source tree in the
Makefile before building. Default is `../../../RIOT`.

You may revert back to the last known functional commit by issuing

`git checkout 46fc1f2ae678a859b06ab3654d6b5344b759ed39` in your RIOT
directory and return to master later with `git checkout master`.


# Acquisition chain

This firmware provide a test of the following acquisition chain:
```
 ----------------        ---------------             ----------------------------------------------
|   Old OwnTech  | SPI  | nucleo-f746zg | Ethernet  |           Ubuntu 20.04 computer              |
| 3 phases board |----->| with OwnTech  |---------->| RSMB ---> mosquitto_[s|p]ub ---> ThingsBoard |
|   MCP3208 ADC  |      | RIOT firmware |  MQTT-SN  |      MQTT                   MQTT             |
 ----------------        ---------------             ----------------------------------------------
```

## 3 phases board internal wiring
<!-- @cond COMMENT --><script><!-- @endcond -->
_
<!-- @cond COMMENT --></script><!-- @endcond -->

You must power up the board by the high side terminals with an
external 15 to 24v power supply and connect J4-1 to J4-3 to power up the
feeder.

You must also cross-link isolators EN1 to SS pins:
- U11-5 (SS) to U10-7 (wired to AN3 on my board)
- U10-5 (SS1) to U11-7 (wired to AN2 on my board)


## SPI wiring
<!-- @cond COMMENT --><script><!-- @endcond -->
_
<!-- @cond COMMENT --></script><!-- @endcond -->
| Signal    | 3 phases board    | nucleo-f746zg |
|:--------- |:----------------- |:------------- |
| GND       | GND               | GND           |
| VCC_3V3   | J3-4              | CN8-7         |
| MISO      | J3-6              | CN10-9        |
| MOSI      | J3-7              | CN9-5         |
| SCK       | J3-10             | CN9-10        |
| CS1       | SS                | CN9-8         |
| CS2       | SS1               | CN9-6         |

## Thingsboard
<!-- @cond COMMENT --><script><!-- @endcond -->
_
<!-- @cond COMMENT --></script><!-- @endcond -->

Assuming Thingsboard is already installed on your computer, you 
have to setup a device with "OT_TEST_TOKEN" as access token.


## Mosquitto.rsmb MQTT-SN to MQTT bridge
<!-- @cond COMMENT --><script><!-- @endcond -->
_
<!-- @cond COMMENT --></script><!-- @endcond -->

First identify your ethernet interface name (eno2 in my case) with
`ifconfig` then assign it a site-local address with 
`sudo ip a a fec0:affe::1/64 dev eno2`

RIOT allready provide mosquitto.rsmb in its sources tree so you don't
need to download or install it. Just launch 
`RSMB_CFG=$(pwd)/mosquitto_rsmb/config.cnf make mosquitto_rsmb` from
the project directory.

Since there seems to be no way to pass the token through the rsmb bridge
I use it as a topic in the MQTT-SN communication which allows to
identify multiple clients.

## Mosquitto.rsmb to Thingsboard MQTT link
<!-- @cond COMMENT --><script><!-- @endcond -->
_
<!-- @cond COMMENT --></script><!-- @endcond -->

Now you have to link Mosquitto.rsmb to Thingsboard. The easyest way to
do it is to install the mosquitto-clients package
(`sudo apt install mosquitto-clients`) which provide the mosquitto_sub
and mosquitto_pub commands. The mosquitto_pub command hase a `-l` option
to read messages from stdin, sending a separate message for each line so
we can just pipe the output of mosquitto_sub to it:
`mosquitto_sub -h "localhost" -p 1886 -t "OT_TEST_TOKEN" | mosquitto_pub -d -h "localhost" -p 1883 -t "v1/devices/me/telemetry" -u "OT_TEST_TOKEN" -l`

You can also launch mosquitto_sub in another terminal to watch the
communication.

## Nucleo-f746zg
<!-- @cond COMMENT --><script><!-- @endcond -->
_
<!-- @cond COMMENT --></script><!-- @endcond -->

The board must be connected to your computer by USB and by Ethernet.

Flash and run the firmware with `make all flash term`.

The firmware configure automatically its network interface then starts
to publish data after aproximatively 3 seconds. There is no output on
the serial console except an amazing OwnTech banner that I made 
sunday.

