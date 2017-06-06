#!/bin/bash
#******************************************************************#
# Author     : Nishal
# Date       : 5th June 2017
# File       : build.sh
# Version    : 0.1 
# Description: This script initializes the path as enviroment 
#              variables for the following
#              1) RIOT_ROOT(open source)
#              2) DW_RIOT_ROOT(DW specific)
#              3) DW_HW_PLATFORM(default HW platform)
#              
#******************************************************************#

#Get the real path of the script
BASEDIR=$(dirname $(readlink -f ${BASH_SOURCE[0]}))

#Export the Environment variables.
RIOT_ROOT=$BASEDIR/RIOT
DW_RIOT_ROOT=$BASEDIR/DW1000

#Set HW platform here. Default is Nordic nrf5280
DW_HW_PLATFORM=nrf52dk

#export RIOT_ROOT
#export DW_RIOT_ROOT
#export DW_HW_PLATFORM

BASH_FILE=~/.bashrc

# Function to add the path variables to the bashrc file
add_env_path()
{
    path_exported=0
    eval bash_search_str="$1"
    eval bashrc_append="$2"

    #check if path is exported in bash
    cat $BASH_FILE | \
    { while read line; do
        if [[ $line == *$bash_search_str* ]]; then
            #check if the path exported is same as current path
            if [ "$line" ==  "$bashrc_append" ]; then
	            path_exported=1
            else
	            path_exported=1
                echo "$line was set before."
                #Update the new path
                echo "Updating $bashrc_append to the $BASH_FILE"
                sed -i "s%$line%$bashrc_append%g" $BASH_FILE
            fi
            break
        fi
    done

    #Append the path if not present in the file
    if [ $path_exported -eq 0 ]; then
        echo "Appending $bashrc_append to the $BASH_FILE"
        echo $bashrc_append >> $BASH_FILE
    fi
    }
}

#check if RIOT_ROOT is exported in bashrc
bashrc_txt="export RIOT_ROOT=$RIOT_ROOT"
search_txt="export RIOT_ROOT"
add_env_path "\${search_txt}" "\${bashrc_txt}"


#check if DW_RIOT_ROOT is exported in bashrc
bashrc_txt="export DW_RIOT_ROOT=$DW_RIOT_ROOT"
search_txt="export DW_RIOT_ROOT"
add_env_path "\${search_txt}" "\${bashrc_txt}"

#check if DW_HW_PLATFORM is exported in bashrc
bashrc_txt="export DW_HW_PLATFORM=$DW_HW_PLATFORM"
search_txt="export DW_HW_PLATFORM"
add_env_path "\${search_txt}" "\${bashrc_txt}"


