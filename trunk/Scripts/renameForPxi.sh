#! /bin/sh

# dir / old name / new name
 
# Generate links with correct names for PXI files
# to be read in synchronization with IMG files

dir=$1
oldName=$2
newName=$3

echo "Will generate link from $dir/$oldName to $dir/$newName\n"

ln -fs $dir/$oldName/RUN_$oldName.bin $dir/$newName/RUN_$newName.bin
ln -fs $dir/$oldName/RUN_$oldName.bin.all.inf $dir/$newName/RUN_$newName.bin.all.inf
ln -fs $dir/$oldName/RUN_$oldName.bin.inf $dir/$newName/RUN_$newName.bin.inf
ln -fs $dir/$oldName/RUN_$oldName.par $dir/$newName/RUN_$newName.par

ls -l $dir/$newName
