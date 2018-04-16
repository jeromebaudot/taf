#! /bin/sh
set -x #echo on

# Usage: copyConfig oldRunNumber newRunNumber
 
# Generate a new configuration file (.cfg) from an old one,
#  simply copying and changing the run number.

# version 2016/11/29

if [ $# -le 1 ]

then
echo "Usage: source copyConfig <oldRunNb> <newRunNb> [configDir]"

else


configDir="config"

if [ $# -ge 2 ]
then
configDir=$3
else
configDir="config"
fi

cd $DTDIR/$configDir
oldRunNumber=$1
newRunNumber=$2

oldFile=run${oldRunNumber}.cfg
newFile=run${newRunNumber}.cfg

echo "Will generate new config in $configDir: $newFile from $oldFile"

#cp $oldFile $newFile
#sed -i s/$oldRunNumber/$newRunNumber/g  $newFile
# For Mac-OS
sed s/$oldRunNumber/$newRunNumber/g > $newFile

cd -

echo "done"

fi

set +x # echo off
