#!/bin/bash
#RUNNAMECORE='PCB19_B4'
RUNNAMECORE='PCB19_FIX_B4'
VBB=0
MX='AC_Amp'
#declare -a HV_LIST=(0 1 2 3 5 8)
declare -a HV_LIST=(3)
declare -a MX_LIST=('AC_Amp' 'DC_Amp' 'DC_SF')
echo "Analysis lounched"
source ./Scripts/thistaf.sh


for mx_val in  "${MX_LIST[@]}"
do
  MX=$mx_val
  for value in  "${HV_LIST[@]}"
  do
  	HV=$value
        echo "\n\n=========================================================="
  	echo "Running for MX = $MX , HV = $HV"
        echo "==========================================================\n\n"
        cd ./Results/${RUNNAMECORE}_VBB${VBB}_HV${HV}_${MX}
	root -q -b ../../code/macros/drawShape.C
	gs -dBATCH -dNOPAUSE -sDEVICE=pdfwrite -dPDFSETTINGS=/printer -sOutputFile=run6Pl1_ClCharge.pdf run6Pl1_ClCharge_tmp*
        cd -
  done
done

