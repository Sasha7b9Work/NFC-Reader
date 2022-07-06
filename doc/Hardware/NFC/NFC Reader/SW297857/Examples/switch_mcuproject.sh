#!/bin/bash
#
# (c) NXP Semiconductors
#  _   _            _   _  __     _____     _ _ _ _
# | \ | |          | \ | |/ _|   |  __ \   | | (_) |
# |  \| |_  ___ __ |  \| | |_ ___| |__) |__| | |_| |__
# | . ` \ \/ / '_ \| . ` |  _/ __|  _  // _` | | | '_ \
# | |\  |>  <| |_) | |\  | || (__| | \ \ (_| | | | |_) |
# |_| \_/_/\_\ .__/|_| \_|_| \___|_|  \_\__,_|_|_|_.__/
#            | |
#            |_|
#
# Helper Script to switch MCU Type for examples
#
# e.g. To switch Examples and Compliance APPs to PN7462AU:
#
#       cd Examples
#       ./switch_mcuproject.sh _PN74xxxx_cproject
#
#       cd ../ComplianceApp
#       ../Examples/switch_mcuproject.sh ../Examples_PN74xxxx_cproject
#

if [ -z $1 ]
then
    echo "Usage $0 _Template_Project"
    exit
fi

if [ ! -f $1 ]
then
    echo "'$1' does not exist."
    exit
fi

for i in Nfcrdlib*;
do
    sed s/THE_PROJECT_NAME/${i}_mcux/g $1 > $i/mcux/.cproject
done
