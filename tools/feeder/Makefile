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

APPLICATION = test_v0.9_feader

EXTERNAL_BOARD_DIRS = ../../boards
BOARD ?= stm32f334-disco

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../RIOT

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Additional code parts such as (extensive) debug output and sanity
# checks using assertions. To activate it set environment variable
# DEVELHELP=1, or disable explicitly with DEVELHELP=0.
DEVELHELP ?= 1

# Modules to include:
USEMODULE += xtimer
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += core_idle_thread
# Enable scheduler statistics for ps:
USEMODULE += schedstatistics


# Owntech modules
USEMODULE += hrtim
EXTERNAL_MODULE_DIRS += $(CURDIR)/../../hrtim


include $(RIOTBASE)/Makefile.include
