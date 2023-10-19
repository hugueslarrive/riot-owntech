EXTERNAL_BOARD_DIRS ?= ../../
BOARD ?= nucleo-f334r8

RIOTBASE ?= ../../../RIOT

include $(RIOTBASE)/tests/Makefile.tests_common

USEMODULE += hrtim
EXTERNAL_MODULE_DIRS += $(CURDIR)/../../modules/hrtim

#~ FEATURES_REQUIRED = periph_hrtim

USEMODULE += xtimer
USEMODULE += shell

include $(RIOTBASE)/Makefile.include
