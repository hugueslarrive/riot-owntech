#
# Copyright (c) 2020-2021 Centre National de la Recherche Scientifique
#
# CNRS, établissement public à caractère scientifique et technologique,
# dont le siège est situé 3 rue Michel-Ange, 75794 Paris cedex 16.
#
#            Luiz Villa - Projet OwnTech <owntech@laas.fr>
#        Laboratoire d'analyse et d'architecture des systèmes
#               LAAS-CNRS - 7, avenue du Colonel Roche
#                 BP 54200 - 31031 Toulouse cedex 4
#
# SPDX-License-Identifier: CECILL-C
#
# @author   Hugues Larrive <hugues.larrive@laas.fr>
#

APPLICATION = test_adc_g4

#~ EXTERNAL_BOARD_DIRS = ../../boards
BOARD ?= nucleo-g474re

# permit modules to include config files
INCLUDES += -I$(CURDIR)

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../RIOT
#~ RIOTBASE ?= /home/hugues/riot-forks/hrtim/RIOT

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Additional code parts such as (extensive) debug output and sanity
# checks using assertions. To activate it set environment variable
# DEVELHELP=1, or disable explicitly with DEVELHELP=0.
DEVELHELP ?= 1

#~ FEATURES_REQUIRED += periph_adc

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
EXTERNAL_MODULE_DIRS += $(CURDIR)/../../modules/adc_g4
USEMODULE += adc_g4

#~ EXTERNAL_MODULE_DIRS += $(CURDIR)/../../modules/daq
#~ USEMODULE += daq

#~ EXTERNAL_MODULE_DIRS += $(CURDIR)/../../modules/hrtim
#~ USEMODULE += hrtim

#~ EXTERNAL_MODULE_DIRS += $(CURDIR)/../../modules/leg
#~ USEMODULE += leg

#~ EXTERNAL_MODULE_DIRS += $(CURDIR)/../../modules/ot_shell_commands
#~ USEMODULE += ot_shell_commands

#~ EXTERNAL_MODULE_DIRS += $(CURDIR)/../../modules/sin
#~ USEMODULE += sin

#~ EXTERNAL_MODULE_DIRS += $(CURDIR)/../../tests/test_daq
#~ USEMODULE += test_daq

#~ EXTERNAL_MODULE_DIRS += $(CURDIR)/../../tests/test_pwm
#~ USEMODULE += test_pwm

include $(RIOTBASE)/Makefile.include
