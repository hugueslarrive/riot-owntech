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

APPLICATION = ownpower

BOARD ?= nucleo-f303k8

# permit modules to include config files
INCLUDES += -I$(CURDIR)

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../RIOT

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Additional code parts such as (extensive) debug output and sanity
# checks using assertions. To activate it set environment variable
# DEVELHELP=1, or disable explicitly with DEVELHELP=0.
DEVELHELP ?= 1

FEATURES_REQUIRED += periph_pwm
FEATURES_REQUIRED += periph_spi

# Do not use DMA as it is inefficient for small (3 bytes) transferts
#FEATURES_REQUIRED += periph_dma

# Modules to include:
USEMODULE += xtimer
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += core_idle_thread
# Enable scheduler statistics for ps:
USEMODULE += schedstatistics

# Owntech modules
EXTERNAL_MODULE_DIRS += $(CURDIR)/../../modules/daq
USEMODULE += daq

EXTERNAL_MODULE_DIRS += $(CURDIR)/../../modules/ot_shell_commands
USEMODULE += ot_shell_commands

EXTERNAL_MODULE_DIRS += $(CURDIR)/pwm
USEMODULE += pwm

include $(RIOTBASE)/Makefile.include
