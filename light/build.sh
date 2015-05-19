#!/bin/sh
set -eu

ARDUINO_ROOT=/usr/local/google/home/dek/arduino-1.5.8
ARDUINO_TOOLS=$ARDUINO_ROOT/hardware/tools/avr/bin

$ARDUINO_TOOLS/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard light.cpp -o light.cpp.o 
$ARDUINO_TOOLS/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard -I. Adafruit_NeoPixel.cpp -o Adafruit_NeoPixel.cpp.o 
$ARDUINO_TOOLS/avr-gcc -c -g -Os -w -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/wiring_pulse.c -o wiring_pulse.c.o 
$ARDUINO_TOOLS/avr-gcc -c -g -Os -w -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/wiring.c -o wiring.c.o 
$ARDUINO_TOOLS/avr-gcc -c -g -Os -w -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/hooks.c -o hooks.c.o 
$ARDUINO_TOOLS/avr-gcc -c -g -Os -w -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/wiring_digital.c -o wiring_digital.c.o 
$ARDUINO_TOOLS/avr-gcc -c -g -Os -w -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/WInterrupts.c -o WInterrupts.c.o 
$ARDUINO_TOOLS/avr-gcc -c -g -Os -w -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/wiring_analog.c -o wiring_analog.c.o 
$ARDUINO_TOOLS/avr-gcc -c -g -Os -w -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/wiring_shift.c -o wiring_shift.c.o 
$ARDUINO_TOOLS/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/HardwareSerial1.cpp -o HardwareSerial1.cpp.o 
$ARDUINO_TOOLS/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/CDC.cpp -o CDC.cpp.o 
$ARDUINO_TOOLS/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/HardwareSerial2.cpp -o HardwareSerial2.cpp.o 
$ARDUINO_TOOLS/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/WMath.cpp -o WMath.cpp.o 
$ARDUINO_TOOLS/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/HID.cpp -o HID.cpp.o 
$ARDUINO_TOOLS/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/WString.cpp -o WString.cpp.o 
$ARDUINO_TOOLS/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/Print.cpp -o Print.cpp.o 
$ARDUINO_TOOLS/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/USBCore.cpp -o USBCore.cpp.o 
$ARDUINO_TOOLS/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/IPAddress.cpp -o IPAddress.cpp.o 
$ARDUINO_TOOLS/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/main.cpp -o main.cpp.o 
$ARDUINO_TOOLS/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/HardwareSerial0.cpp -o HardwareSerial0.cpp.o 
$ARDUINO_TOOLS/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/Tone.cpp -o Tone.cpp.o 
$ARDUINO_TOOLS/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/Stream.cpp -o Stream.cpp.o 
$ARDUINO_TOOLS/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/HardwareSerial.cpp -o HardwareSerial.cpp.o 
$ARDUINO_TOOLS/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/abi.cpp -o abi.cpp.o 
$ARDUINO_TOOLS/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/HardwareSerial3.cpp -o HardwareSerial3.cpp.o 
$ARDUINO_TOOLS/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=158 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I$ARDUINO_ROOT/hardware/arduino/avr/cores/arduino -I$ARDUINO_ROOT/hardware/arduino/avr/variants/standard $ARDUINO_ROOT/hardware/arduino/avr/cores/arduino/new.cpp -o new.cpp.o 
$ARDUINO_TOOLS/avr-ar rcs core.a wiring_pulse.c.o 
$ARDUINO_TOOLS/avr-ar rcs core.a wiring.c.o 
$ARDUINO_TOOLS/avr-ar rcs core.a hooks.c.o 
$ARDUINO_TOOLS/avr-ar rcs core.a wiring_digital.c.o 
$ARDUINO_TOOLS/avr-ar rcs core.a WInterrupts.c.o 
$ARDUINO_TOOLS/avr-ar rcs core.a wiring_analog.c.o 
$ARDUINO_TOOLS/avr-ar rcs core.a wiring_shift.c.o 
$ARDUINO_TOOLS/avr-ar rcs core.a HardwareSerial1.cpp.o 
$ARDUINO_TOOLS/avr-ar rcs core.a CDC.cpp.o 
$ARDUINO_TOOLS/avr-ar rcs core.a HardwareSerial2.cpp.o 
$ARDUINO_TOOLS/avr-ar rcs core.a WMath.cpp.o 
$ARDUINO_TOOLS/avr-ar rcs core.a HID.cpp.o 
$ARDUINO_TOOLS/avr-ar rcs core.a WString.cpp.o 
$ARDUINO_TOOLS/avr-ar rcs core.a Print.cpp.o 
$ARDUINO_TOOLS/avr-ar rcs core.a USBCore.cpp.o 
$ARDUINO_TOOLS/avr-ar rcs core.a IPAddress.cpp.o 
$ARDUINO_TOOLS/avr-ar rcs core.a main.cpp.o 
$ARDUINO_TOOLS/avr-ar rcs core.a HardwareSerial0.cpp.o 
$ARDUINO_TOOLS/avr-ar rcs core.a Tone.cpp.o 
$ARDUINO_TOOLS/avr-ar rcs core.a Stream.cpp.o 
$ARDUINO_TOOLS/avr-ar rcs core.a HardwareSerial.cpp.o 
$ARDUINO_TOOLS/avr-ar rcs core.a abi.cpp.o 
$ARDUINO_TOOLS/avr-ar rcs core.a HardwareSerial3.cpp.o 
$ARDUINO_TOOLS/avr-ar rcs core.a new.cpp.o 
$ARDUINO_TOOLS/avr-gcc -w -Os -Wl,--gc-sections -mmcu=atmega328p -o light.cpp.elf light.cpp.o Adafruit_NeoPixel.cpp.o core.a -L/tmp/build5324131886960530268.tmp -lm 
$ARDUINO_TOOLS/avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 light.cpp.elf light.cpp.eep 
$ARDUINO_TOOLS/avr-objcopy -O ihex -R .eeprom light.cpp.elf light.cpp.hex 

$ARDUINO_TOOLS/avrdude -C$ARDUINO_ROOT/hardware/tools/avr/etc/avrdude.conf -v -v -v -v -patmega328p -carduino -P/dev/ttyACM0 -b115200 -D -Uflash:w:light.cpp.hex:i  || rm -f *.d *.o light.cpp.elf light.cpp.eep core.a light.cpp.hex
