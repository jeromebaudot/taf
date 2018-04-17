#! /bin/sh

# RUN NUMBER / N files / PATH


#----RUN NUMBER
#Mimosa_Number=14
if [ $1 -ge 100000 ]
then Mimosa_Number=`expr $1 / 10000`
else Mimosa_Number=`expr $1 / 1000`
fi
#echo $Mimosa_Number

if [ $1 -ge 100000 ]  
then Run_Number=`expr $1 - $[10000*$Mimosa_Number]`
else Run_Number=`expr $1 - $[1000*$Mimosa_Number]`
fi
#echo $Run_Number
#525

#Mimosa_Number=14
#----FILES NUMBERS
# start index
BEGIN=0
#number of files 
END=$2


#----DIRECTORY WHERE THE links will be created:
if [ -z $DTDIR ] ; then echo "DTDIR not set... run MAF-config" ; exit; fi
Raw_data_Path=$DTDIR
Raw_data_Path2=datRAW

#----DIRECTORY WHERE THE RAW DATA ARE:
#Raw_data_Path3=/rawcmos4/dataRAW/cern_may02/tape2/
#Raw_data_Path3=/rawcmos6a/data2/2004/mimosa5/cern/datRAW
#Raw_data_Path3=/rawcmos1/data/2004/mimosa9/cern/datRAW
#Raw_data_Path3=/rawcmos3b/data/mi9_cern_raw_08_2004
#Raw_data_Path3=/rawcmos6c/data/cern_mi9_august_2004/datRAW
#Raw_data_Path3=/rawcmos7/data/mimosa9_april_2005_testbeam_desy
#Raw_data_Path3=/rawcmos8/data/old/
#Raw_data_Path3=/rawcmos8/data/mimosa8_desy_raw_092005_dir01
#Raw_data_Path3=/rawcmos8a/data/mimostar2_desy_raw_112005_dir01
#Raw_data_Path3=/rawcmos9/data/mimostar2_november_2005_testbeam_desy/
#Raw_data_Path3=/rawcmos9/data/mimostar2_mai_2006_calibration_vme/ 
#Raw_data_Path3=/rawcmos18a/data/mimosa30-32_cern_06_2012/cern_mi32_06_2012/merge/
#Raw_data_Path3=/rawcmos18a/data/mimosa30-32_cern_06_2012/cern_mi30_06_2012/merge/
Raw_data_Path3=$3

Run_Mimosa=${Mimosa_Number}${Run_Number}
Run_Mimosa_complete=${Mimosa_Number}${Run_Number}

if [ ! -d $Raw_data_Path/$Raw_data_Path2/$Run_Mimosa ] ; 
then  mkdir -p $Raw_data_Path/$Raw_data_Path2/$Run_Mimosa;
fi

# cd $Raw_data_Path/$Raw_data_Path2/
# mkdir $Run_Mimosa


#----------------------------------------------------------------
#NUM=10
for File_Number in `seq $BEGIN $END`
do
test -f  ${Raw_data_Path3}/${Run_Mimosa}/RUN_${Run_Mimosa_complete}_${File_Number}.rz
if [ $? -eq 1 ] ; then echo "${Raw_data_Path3}/${Run_Mimosa}/RUN_${Run_Mimosa_complete}_${File_Number}.rz does not exist" ; exit ; fi


# echo ${File_Number} ${NUM} 
if  (( ${File_Number} < 10 ))
then
#echo "test1" ${File_Number} ${NUM} 
   \ln -sf ${Raw_data_Path3}/${Run_Mimosa}/RUN_${Run_Mimosa_complete}_${File_Number}.rz  $Raw_data_Path/$Raw_data_Path2/${Run_Mimosa}/${Run_Number}_00${File_Number}.raw 
else

#echo "test2" ${File_Number} ${NUM} 
    \ln -sf ${Raw_data_Path3}/${Run_Mimosa}/RUN_${Run_Mimosa_complete}_${File_Number}.rz  $Raw_data_Path/$Raw_data_Path2/${Run_Mimosa}/${Run_Number}_0${File_Number}.raw 

fi

done
#-------------------------------------------------------------

ls $Raw_data_Path/$Raw_data_Path2/ 
ls -Lash $Raw_data_Path/$Raw_data_Path2/$Run_Mimosa

# cd $Raw_data_Path
