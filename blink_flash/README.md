# blink sample on flash

This is the sample led blinking program that run on the flash.

# Prerequirement

## elf2uf2

This program needs the elf2uf2 to write into the flash.

To get the elf2uf2 do the things as follows.

1. Setup the pico sdk
1. Copy the elf2uf2 from SDK into ~/bin
1. Add the ~/bin to the PATH environment.

for examples.

Setup the pico sdk.

```txt
$ wget https://raw.githubusercontent.com/raspberrypi/pico-setup/master/pico_setup.sh
$ chmod +x ./pico_setup.sh
$ ./pico_setup.sh
```

Copy the elf2uf2 from SDK into ~/bin

```txt
$ mkdir ~/bin
$ cp ${PICO_EXAMPLES_PATH}/build/elf2uf2/elf2uf2 ~/bin
```

Add the ~/bin to the PATH environment.

```txt
$ echo 'export PATH=${HOME}/bin:${PATH}' >> ~/.bashrc
$ source ~/.bashrc
```

## Boot Stage2

This program needs the second stage bootloader(Boot Stage2) to load the program from the flash.

Copy the pre-compiled Boot Stage2 from SDK to under the boot2 directory.

```txt
$ mkdir boot2
$ cp ${PICO_EXAMPLES_PATH}/build/pico-sdk/src/rp2_common/boot_stage2/bs2_default_padded_checksummed.S ./boot2
```


# Build and Flash

## Build

Run make to build the program.

```txt
$ make
```

## Flash the program to the Pico

For flashing the program do as follows.

1. Press the MODESEL button and connect Pico to the PC via a USB cable.
1. Then wait for mounting the /media/${USER}/RPI-RP2.
1. Copy the led.uf2 to the /media/${USER}/RPI-RP2.
