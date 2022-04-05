#!/bin/bash
#RUNNAMECORE='PCB19_B4'
#RUNNAMECORE='PCB19_FIX_B4'
#RUNNAMECORE='PCB10_B4_10M'
RUNNAMECORE='PCB3_A4_10M'
VBB=0
MX='AC_Amp'
declare -a HV_LIST=(0 1 2 3 4 5 7 10 15)
#declare -a HV_LIST=(0 1 2 3)
#declare -a HV_LIST=(3)
#declare -a MX_LIST=('DC_Amp' 'DC_SF')
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
  	TAF -run 6 -cfg ./config/ce65abc_noise.cfg -datafile ${RUNNAMECORE}_HV3_VBB0_noise.root code/macros/batch_noise.C -q -b
  	TAF -run 6 -cfg ./config/ce65abc_dsf.cfg -datafile ${RUNNAMECORE}_HV${HV}_VBB${VBB}_fe.root  code/macros/batchanalysis_${MX}.C -q -b >> logs/analysis_${RUNNAMECORE}_VBB${VBB}_HV${HV}_${MX}.log
        mv datDSF/run6_01.root datDSF/${RUNNAMECORE}_VBB${VBB}_HV${HV}_${MX}.root
  	mv ./results_ana_M651/run6* ./Results/${RUNNAMECORE}_VBB${VBB}_HV${HV}_${MX}
  done
done

