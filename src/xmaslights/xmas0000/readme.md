# XMas Lights

## Description

* XMas Lights is a hardware/software solution to enable programmable lightchains
* Based on Z-Uno controller https://z-uno.z-wave.me/
* Two USB-connected lightchains support
* Z-Wave based remote control

## Development environment

### Toolchain

* [Arduino IDE](https://www.microsoft.com/de-de/p/arduino-ide/9nblggh4rsd8?activetab=pivot:overviewtab) is needed to flash firmware
* [Visual Studio Code](https://code.visualstudio.com/download) is recommended for the development and debug processes
* [Microsoft Build Tools](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=BuildTools&rel=16) should be installed on windows to support C compilation/debug process

### Simulation

* Rename local *.ino file to *.cpp file
* Uncomment #define DEBUG_CONSOLE and comment #undef DEBUG_CONSOLE to run simulation
* Console windows should display simulation

## Known issues

* Z-Uno should be manually restarted (press "Reset" button once) after power is lost

## Versions

* 0.01 / 30.01.2020: Initial version
