@page riot_contrib  OwnTech contributions to the Riot project

# OwnTech contributions to the Riot project

# nucleo-f334r8 missing features


## dma

PRs that was merged into RIOT master:
- https://github.com/RIOT-OS/RIOT/pull/14764 cpu/stm32/periph/dma: add
support for STM32F3
- https://github.com/RIOT-OS/RIOT/pull/14734 boards/nucleo-f334r8: dma
support added

And also:
- https://github.com/RIOT-OS/RIOT/pull/14763 cpu/stm32/vectors/vectors_f3:
a small fix for STM32F334x8
- https://github.com/RIOT-OS/RIOT/pull/14767 boards/common/blxxxpill:
DMA feature added
- https://github.com/RIOT-OS/RIOT/pull/14765 boards/nucleo-f303k8:
DMA feature added


## adc

PRs that was merged into RIOT master :
- https://github.com/RIOT-OS/RIOT/pull/14860 STM32F3 familly ADC
implementation
- https://github.com/RIOT-OS/RIOT/pull/14849 cpu/stm32/periph/adc:
remove MAX_ADC_SPEED from implementations where it is not used
- https://github.com/RIOT-OS/RIOT/pull/14846 Declare adc_config[] like
others peripherals configuration arrays
- https://github.com/RIOT-OS/RIOT/pull/14861 boards/nucleo-f334r8: add
ADC feature

And also:
- https://github.com/RIOT-OS/RIOT/pull/14863 boards/nucleo-f302r8: add
ADC feature
- https://github.com/RIOT-OS/RIOT/pull/14862 boards/nucleo-f303k8: add
ADC feature
- https://github.com/RIOT-OS/RIOT/pull/14864 boards/nucleo-f303re: add
ADC feature
- https://github.com/RIOT-OS/RIOT/pull/14865 boards/nucleo-f303ze: add
ADC feature


# RIOT missing features


## hrtim

I have writed a 1200+ lines basic periph_hrtim driver implementation
submitted in https://github.com/RIOT-OS/RIOT/pull/15165 that was not
merged because this peripheral is too specific to STM32.

I also started a discussion on the periph_pwm API that is unable to
support advanced timer features:
https://github.com/RIOT-OS/RIOT/issues/15172.

For now I have backported the implementation in the `/hrtim` and
`/tests_v0.9/tests/periph_hrtim` directories.

## Advanced ADC support

RIOT actually provide basic ADC support but work on an advenced one is
in progress: https://github.com/RIOT-OS/RIOT/pull/13247/commits.

The owntech board v0.9 use differential output isolators for voltage
measurements that is not supported by the RIOT periph_adc API so
negatives inputs are "manually" initialized and DIFSEL register is set
prior to adc_init call.

The calibration procedure is different for differential channels as I
authored F3's ADC support, I have added DIFSEL check for differential
mode calibration on the sidelines of the maintenance PR:
https://github.com/RIOT-OS/RIOT/pull/15119 cpu/stm32/periph/adc_f3: add
ADC3 and ADC4 management and that was accepted as I said "it doesn't
bother" ! This must be take into account in case of code porting on
another stm32 familly (G4 ?).


# Other Riot contributions

Merged:
- https://github.com/RIOT-OS/RIOT/pull/14461: Fix bad assertion in
cpu/stm32/periph/pwm.c
- https://github.com/RIOT-OS/RIOT/pull/14482: cpu/stm32/periph/pwm: some
bugfixes...

Unmerged:
- https://github.com/RIOT-OS/RIOT/pull/14768: tests/periph_spi: add
support for printing SPI clock rates on stm32

# Riot wiki
- https://github.com/RIOT-OS/RIOT/wiki/Build-RIOT-on-WSL-%28Windows-Subsystem-for-Linux%29

