#!/bin/bash
#******************************************************************#
# Author     : Nishal
# Date       : 29th May 2017
# File       : setup.sh
# Version    : 0.1 
# Description: This script does the setup for the system to work 
#              with DW1000 examples
#              
#******************************************************************#
#Initialising the necessary variables required for this script
DIR=$(pwd)
OPENOCD_DIR=openocd-code

RIOT_BRANCH=2017.01-branch
PATCH_FILE=riot-dw.patch

echo ""
echo "<SCRIPT_LOG> Current directory $DIR"

if [ "$#" -ne  "0" -a "$1" == "INITIAL" ]; then
    echo "<SCRIPT_LOG> Setting up the dependencies for project..."
    #Install dependent librarires
    echo "<SCRIPT_LOG> Installing dependent librarires..."
    sudo apt-get install lib32z1 lib32ncurses5 lib32bz2-1.0

    #Install GCC ARM Embedded tool chain
    echo "<SCRIPT_LOG> Installing GCC ARM Embedded tool chain..."
    sudo add-apt-repository ppa:team-gcc-arm-embedded/ppa
    sudo apt-get update
    sudo apt-get install gcc-arm-embedded

    #Install OpenOCD (On-Chip Debugger)
    echo "<SCRIPT_LOG> Installing OpenOCD..."
    sudo apt-get install build-essential autoconf automake libtool libusb-dev libusb-1.0-0-dev libhidapi-dev

    #check if data is cloned already.
    if [ -d "$DIR/tools/$OPENOCD_DIR" ]; then
	echo "<SCRIPT_LOG> $OPENOCD_DIR already exists."
    else
	mkdir tools
	cd tools
 	echo "<SCRIPT_LOG> cloning openOCD to $OPENOCD_DIR directory."
	git clone https://git.code.sf.net/p/openocd/code $OPENOCD_DIR
	cd $OPENOCD_DIR
	./bootstrap
	./configure
	make
	sudo make install
	cd -
        cd ..
    fi
    openocd --version
else
    echo "<SCRIPT_LOG> Setting up the dependencies for project not required..."
fi

#Clone RIOT repo if INITIAL or UPDATE
if [ "$#" -ne  "0" ]; then
if [ "$1" == "INITIAL" -o "$1" == "UPDATE" ]; then

    #clone the RIOT repo and respective branch
    echo "<SCRIPT_LOG> Cloning open source RIOT repository..."
    if [ -d "$RIOT_ROOT" ]; then
        echo "<SCRIPT_LOG> $RIOT_ROOT already exists..."
	rm -rf $RIOT_ROOT
    fi
    echo "<SCRIPT_LOG> Cloning RIOT code to $RIOT_ROOT directory."
    git clone https://github.com/RIOT-OS/RIOT.git $RIOT_ROOT
    cd $RIOT_ROOT
    git checkout $RIOT_BRANCH
    cd -

    #Create symlinks
    echo "<SCRIPT_LOG> Creating symbolic links..."
    sudo ln -s $DW_RIOT_ROOT/boards/nrf52dk/dw1000 $RIOT_ROOT/boards/nrf52dk/dw1000
    sudo ln -s $DW_RIOT_ROOT/boards/nucleo-f401/dw1000 $RIOT_ROOT/boards/nucleo-f401/dw1000
    sudo ln -s $DW_RIOT_ROOT/boards/samr21-xpro/dw1000 $RIOT_ROOT/boards/samr21-xpro/dw1000
    sudo ln -s $DW_RIOT_ROOT/drivers/dw1000 $RIOT_ROOT/drivers/dw1000
    sudo ln -s $DW_RIOT_ROOT/drivers/include/dw1000 $RIOT_ROOT/drivers/include/dw1000
    sudo ln -s $DW_RIOT_ROOT/sys/dw1000_common $RIOT_ROOT/sys/dw1000_common
    sudo ln -s $DW_RIOT_ROOT/sys/include/dw1000_common $RIOT_ROOT/sys/include/dw1000_common
    sudo ln -s $DW_RIOT_ROOT/examples/dw1000_rtls_arm $RIOT_ROOT/examples/dw1000_rtls_arm
    sudo ln -s $DW_RIOT_ROOT/doc/doxygen/html $RIOT_ROOT/doc/doxygen/html 
    sync

    #Apply a patch for the makefile changes.
    echo "<SCRIPT_LOG> Applying patch $PATCH_FILE for the common files..."
    if [ -f $DIR/$PATCH_FILE ]; then
        echo "<SCRIPT_LOG> $DIR/$PATCH_FILE file exists"
        echo "$(ls -l)"
        patch -p0 -b < $PATCH_FILE
    else
        echo "<SCRIPT_LOG> $DIR/$PATCH_FILE not found"
    fi
fi
fi
