#! /bin/sh
#
# Create symbolic links for PXI rawdata files,
#  from an old run number to a new one.
#
# Usage: source makeSynchroLink.sh <dir> <oldRunNb> <newRunNb>
#

if [ $# -ne 3 ]

then
echo "Usage: source makeSynchroLink.sh <dir> <oldRunNb> <newRunNb>\n"

else

cd $1
ln -sf $2/RUN_$2.bin         $3/RUN_$3.bin
ln -sf $2/RUN_$2.bin.all.inf $3/RUN_$3.bin.all.inf
ln -sf $2/RUN_$2.bin.inf     $3/RUN_$3.bin.inf
ln -sf $2/RUN_$2.par         $3/RUN_$3.par
cd -

ls -l $1

fi