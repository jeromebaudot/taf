#! /bin/sh
#set -x #echo on

# Usage: copyConfig oldRunNumber newRunNumber
 
# Generate a new configuration file (.cfg) from an old one,
#  simply copying and changing the run number.

if [ $# -ne 4 ]

then
echo "Usage: source copyConfig <oldRunNb> <newRunNb> <oldNoiseRunNb> <newNoiseRunNb>\n"

else

cd $DTDIR/config
oldRunNumber=$1
newRunNumber=$2
oldNoiseRunNumber=$3
newNoiseRunNumber=$4

oldFile=run${oldRunNumber}.cfg
newFile=run${newRunNumber}.cfg

echo "Will generate new config $newFile from $oldFile\n"

cp $oldFile $newFile
sed -i s/$oldRunNumber/$newRunNumber/g $newFile
sed -i s/$oldNoiseRunNumber/$newNoiseRunNumber/g $newFile
#$cmd

cd -

echo "done"

fi

set +x # echo off
