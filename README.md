# beaglebone-black-glitcher

pru application that waits for a LOW logic signal on P9_27 and outputs a pulse on P9_30 of a beaglebone black
pulse delay and width set from userspace using included program `glitch-param-set`

adapted from https://docs.beagleboard.org/latest/books/pru-cookbook.

## installation
install image from https://forum.beagleboard.org/t/debian-11-x-bullseye-monthly-snapshot-2023-10-07/31280

```bash
debian@BeagleBone:~$ cd /opt/source
debian@BeagleBone:/opt/source$ git clone https://github.com/resinbeard/beaglebone-black-glitcher.git
debian@BeagleBone:/opt/source$ cd beaglebone-black-glitcher
debian@BeagleBone:/opt/source/beaglebone-black-glitcher$ sudo ./install.sh
debian@BeagleBone:/opt/source/beaglebone-black-glitcher$ cd glitch
debian@BeagleBone:/opt/source/beaglebone-black-glitcher/glitch$ source setup.sh
debian@BeagleBone:/opt/source/beaglebone-black-glitcher/glitch$ make
```

## usage
```bash
debian@BeagleBone:/opt/source/beaglebone-black-glitcher/glitch$ sudo ./glitch-param-set --delay 100 --width 4000
Using /dev/mem.
pulse_delay_cycles: 100, pulse_width_cycles: 4000
munmap succeeded
debian@BeagleBone:/opt/source/beaglebone-black-glitcher/glitch$
```

