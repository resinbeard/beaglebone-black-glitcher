#!/bin/bash
#
export TARGET=glitch.pru0
echo TARGET=$TARGET

# Configure the PRU pins based on which Beagle is running
machine=$(awk '{print $NF}' /proc/device-tree/model)
echo -n $machine
if [ $machine = "Black" ]; then
    echo " Found"
else
    echo " Not Found"
    pins=""
fi

config-pin P9_27 pruin
config-pin -q P9_27

config-pin P9_30 pruout
config-pin -q P9_30
