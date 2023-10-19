APPLICATION ?= owntech

# OwnTech specific board configuration
EXTERNAL_BOARD_DIRS = ../../boards
BOARD = nucleo-f334r8

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../RIOT

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# DEVELHELP enabled by default for all tests, set 0 to disable
DEVELHELP ?= 1

FEATURES_REQUIRED += periph_spi

# Modules to include:
USEMODULE += xtimer
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += core_idle_thread
# Uncomment this to enable scheduler statistics for ps:
USEMODULE += schedstatistics

include $(RIOTBASE)/Makefile.include
