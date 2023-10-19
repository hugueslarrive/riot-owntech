#
# Copyright (c) 2020 LAAS-CNRS
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Author:	Hugues Larrive <hugues.larrive@laas.fr>
#

APPLICATION = test_mqtt

# OwnTech specific board configuration
EXTERNAL_BOARD_DIRS = ../../boards
BOARD = nucleo-f746zg

# permit modules to include config files
INCLUDES += -I$(CURDIR)

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../RIOT

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# DEVELHELP enabled by default for all tests, set 0 to disable
DEVELHELP ?= 1

FEATURES_REQUIRED += periph_pwm
FEATURES_REQUIRED += periph_spi
FEATURES_REQUIRED += periph_dma

# Modules to include:
USEMODULE += xtimer
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += core_idle_thread
# Uncomment this to enable scheduler statistics for ps:
USEMODULE += schedstatistics

# For UDP Client/Server
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc_ipv6_default
USEMODULE += gnrc_icmpv6_echo
USEMODULE += gnrc_sock_udp

# Include MQTT-SN
USEMODULE += asymcute

# Owntech modules
EXTERNAL_MODULE_DIRS += $(CURDIR)/../../modules/daq
USEMODULE += daq

EXTERNAL_MODULE_DIRS += $(CURDIR)/../../modules/ot_shell_commands
USEMODULE += ot_shell_commands

EXTERNAL_MODULE_DIRS += $(CURDIR)/../../modules/ot_pwm
USEMODULE += ot_pwm

EXTERNAL_MODULE_DIRS += $(CURDIR)/pwm
USEMODULE += pwm

EXTERNAL_MODULE_DIRS += $(CURDIR)/mqttsn
USEMODULE += mqttsn


include $(RIOTBASE)/Makefile.include
