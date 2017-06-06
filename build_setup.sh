#!/bin/bash
#******************************************************************#
# Author     : Nishal
# Date       : 5th June 2017
# File       : build_setup.sh
# Version    : 0.1 
# Description: This script does the following thing
#              1)setup for the system to work when called with INITIAL
#              2)git clone of opensource repo when called with UPDATE
#              3)build and flash of application 
#              
#******************************************************************#

#Export environment variable
source setenv.sh

if [ "$#" -ne  "0" -a "$1" == "INITIAL" ]; then
    echo "<SCRIPT_LOG> setup with install called.."
    cd $DW_RIOT_ROOT/..
    pwd
    ./setup.sh $1
    cd -
elif [ "$#" -ne  "0" -a "$1" == "UPDATE" ]; then
    echo "<SCRIPT_LOG> setup with update called.."
    cd $DW_RIOT_ROOT/..
    pwd
    ./setup.sh $1
    cd -
fi
#Clean the bins
if [ -d "$DW_RIOT_ROOT/examples/dw1000_rtls_arm/bin" ]; then
    sudo rm -rf $DW_RIOT_ROOT/examples/dw1000_rtls_arm/bin
fi

#Build application for RTLS Ranging application
make -C $RIOT_ROOT/examples/dw1000_rtls_arm BOARD=$DW_HW_PLATFORM

