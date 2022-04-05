#!/bin/bash
source  Scripts/thistaf.sh
RUNNAMECORE='PCB10_B4_10M'
#RUNNAMECORE='PCB3_A4_10M'
VBB=0
MX='AC_Amp'
declare -a HV_LIST=(05 1 15)
declare -a MX_LIST=('AC_Amp' 'DC_Amp' 'DC_SF')
echo "Analysis lounched"
 source ./Scripts/thistaf.sh


for mx_val in  "${MX_LIST[@]}"
do
  MX=$mx_val
  for value in  "${HV_LIST[@]}"
  do
  	HV=$value
  	echo "Running for MX = $MX , HV = $HV"
  	TAF -run 1 -cfg ./config/ce65abc_noise.cfg -datafile  ${RUNNAMECORE}_HV0_VBB${HV}_noise.root code/macros/batch_noise.C -q -b
  	TAF -run 1 -cfg ./config/ce65abc_extnoise.cfg -datafile  ${RUNNAMECORE}_HV0_VBB${HV}_fe.root  code/macros/batchraw_${MX}.C -q -b
  	root savepng.C -q 
  	root plotnoise.C -q 
  	mv  ./Results/1 ./Results/${RUNNAMECORE}_VBB${VBB}_HV${HV}_${MX}
  done
done

