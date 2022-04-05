#!/bin/bash
RUNNAMECORE='PCB14_A4'
VBB=0
MX='AC_Amp'
declare -a HV_LIST=(0 1 2 3 5 8)
echo "Analysis lounched"

for value in  "${HV_LIST[@]}"
do
	HV=$value
	echo "Running for HV = $HV"

done


