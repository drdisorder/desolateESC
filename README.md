# desolateESC
ESC FW experiment 

license: CC BY-NC-SA (https://creativecommons.org/licenses/by-nc-sa/2.0/)
### BY Attribution
You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
### NC NonCommercial
You may not use the material for commercial purposes.
### SA ShareAlike
If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.

## use desolateESC on your own risk!
- there is no guarantee that it will not destroy your ESC.
- the target ESC's must be programmed directly on the MCU. you will need to erase all FW that was there before, means you will loose you warranty for sure. 
- there might be no way back.

Currently supported MCU's:
- STM32F051

Currently supported targets:
- Wraith32 35A
- Maverick ESC
- KISS24A ESC (now also when flashed over the bootloader)

Currently working state:
- Dshot with any bitrate (auto bitrate) tested with 600 and 1200
- Dshot telemetry workes on the desk
- ADC stuff like temperature, voltage and if on the HW the current. whilst the current reading is not very accurate
- desk tests with different motors run fine up to a phase switch speed of 50khz (500rpm on a 2pole motor)

Todo:
- perhaps blheli and maverick bootloader support, if not encrypted.
- it has to be tested a lot