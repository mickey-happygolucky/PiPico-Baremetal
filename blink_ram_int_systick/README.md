# blink sample

This is the simple led blinking program that run on RAM.

## build

```txt
$ make
```

## run

1. setup the pico sdk
1. run openocd
1. run gdb-multiarch
1. load to RAM on Raspberrypi Pico
1. do the `c(continue)`

Do the command in shell as follows:

```txt
$ cd ${PICO_SDK_PATH}/../openocd
$ src/openocd -f interface/picoprobe.cfg -f target/rp2040.cfg -s tcl
```

In other shell.

```txt
$ gdb-multiarch -x pico.gdbinit led.elf
(gdb) c
```
