#!/bin/bash

function clearVar
{
  unset CFGFILES
  unset RUNS
  unset MACROFILES
  unset ARGS
  unset LOGFILE
}

function deleteTAFunusedFiles
{
  find . -name "PXIeError_run*.txt" -exec rm -v {} \; 
  find . -name "PXIeMsg_run*.txt" -exec -v rm {} \;
  find . -name "eta_run*.root" -exec rm -v {} \;
  find . -name "eta*.root" -exec rm -v {} \;
  find . -name "errors_run*.txt" -exec rm -v {} \;
  find . -name "msg_run*.txt" -exec rm -v {} \;
}
##############################
#       Precaution
##############################
echo "You are running the batch file exemple located in TAF $TAFMAINDIR/Scripts"
echo "If it's that you want, comment first lines"
echo "If no, copy/paste this file and RENAME it ! (cause Script dir. is in the PATH !"
return 0


##############################
#       configuration
##############################
#***********************
#   Work Mode
#***********************
# ONLY_TEST="1"
ONLY_TEST="0"

#***********************
#   Work Mode
#***********************
# DeleteTAFUnusedFiles="0"
DeleteTAFUnusedFiles="1"

#***********************
#   execution path
#***********************
EXEC_PATH="/home/valerian/research/projects.sync/pivi/experiments/2011.12.11-HIT/analyses/2014.07-Manuscrit_with_new_TAF/02-Clustering";



#***********************
#    config files
#***********************
icfg=0;
#***********************
CFGFILES_PATH=$EXEC_PATH
#CFGFILES_PATH="/home/valerian/research/projects.sync/pivi/experiments/2011.12.11-HIT/analyses/2014.07-Manuscrit_with_new_TAF/000-tests"
#CFGFILES_PATH=""

CFGFILES[icfg]="HIT1_noise26169_Min1-2max14um.cfg";icfg=$(($icfg+1));
# CFGFILES[icfg]="HIT1_noise26169_Min1-2max27um.cfg";icfg=$(($icfg+1));
# CFGFILES[icfg]="HIT1_noise26169_Min1-2max40um.cfg";icfg=$(($icfg+1));
# CFGFILES[icfg]="HIT1_noise26169_Min1-2max53um.cfg";icfg=$(($icfg+1));
# CFGFILES[icfg]="HIT1_noise26169_Min1-2max66um.cfg";icfg=$(($icfg+1));
# CFGFILES[icfg]="HIT1_noise26169_Min1-2max79um.cfg";icfg=$(($icfg+1));
CFGFILES[icfg]="HIT1_noise26169_Min1-2max92um.cfg";icfg=$(($icfg+1));
CFGFILES[icfg]="HIT1_noise26169_Min1-2max105um.cfg";icfg=$(($icfg+1));
CFGFILES[icfg]="HIT1_noise26169_Min1-2max118um.cfg";icfg=$(($icfg+1));
CFGFILES[icfg]="HIT1_noise26169_Min1-2max131um.cfg";icfg=$(($icfg+1));
CFGFILES[icfg]="HIT1_noise26169_Min1-2max144um.cfg";icfg=$(($icfg+1));


#***********************
#        runs
#***********************
irun=0;
#***********************
#RSB8
# RUNS[irun]=1;irun=$(($irun+1)); #Align rsb8 p200Mev
RUNS[irun]=31;irun=$(($irun+1)); #IVI rsb8 C350Mev eudet2

# RunNbFIRST=1;
# RunNbLAST=10;
# iRunNb=$RunNbFIRST;
# while [ "$iRunNb" != $(($RunNbLAST +1)) ]
# do
# RUNS[irun]=$iRunNb;irun=$(($irun+1));
# iRunNb=$(($iRunNb+1));
# done


#***********************
#       macros
#***********************
imac=0;
#***********************
MACROS_PATH=$EXEC_PATH
#MACROS_PATH=$EXEC_PATH
#MACROS_PATH="/home/valerian/research/projects.sync/pivi/experiments/2011.12.11-HIT/analyses/2014.07-Manuscrit_with_new_TAF/000-tests"

MACROFILES[imac]="macroMRax.C";imac=$(($imac+1));
#MACROFILES[imac]="macroMRax2.C";imac=$(($imac+1));

#***********************
#  executable and args
#***********************
iarg=0;
#***********************
EXEC_NAME="TAF";

ARGS[iarg]="-guix";iarg=$(($iarg+1));
ARGS[iarg]="-b";iarg=$(($iarg+1));
ARGS[iarg]="-q";iarg=$(($iarg+1));
ARGS[iarg]="-l";iarg=$(($iarg+1));

#***********************
#  logs paths
#***********************
OUTPUTLOGS_PATH="$EXEC_PATH/logs";
#OUTPUTLOGS_PATH="/home/valerian/research/projects.sync/pivi/experiments/2011.12.11-HIT/analyses/2014.07-Manuscrit_with_new_TAF/000-tests"


##############################
#    end of configuration
##############################






CFGFILES_NB=${#CFGFILES[*]};
RUNS_NB=${#RUNS[*]};
MACROFILES_NB=${#MACROFILES[*]};
JOBS_NB=$(($CFGFILES_NB * $RUNS_NB * $MACROFILES_NB ))

PREVIOUSPWD=`pwd`;
STARTDATE=$(date);
STARTTIME=$(date +"%s");

echo "####################################################################################"
echo "#                           batch TAF script                                        "
echo "####################################################################################"
echo ""
echo "############################################"
echo "#          CONFIGURATION                   #"
echo "############################################"
echo "#"
echo "# host         : $HOSTNAME";
echo "# start        : $STARTDATE";
echo "# EXEC_PATH    : $EXEC_PATH";
if [ -d $EXEC_PATH ]; then
  echo "#    * exists, cd to it"
  cd $EXEC_PATH
else
  echo "#    * DO NOT EXISTS ! Quit"
  return 0
fi
echo "# executable   : $EXEC_NAME";
EXEC_FILE=`which $EXEC_NAME`;
if [ "$EXEC_FILE" = "" ]; then
  echo "#    * DO NOT EXISTS ! Quit"
  return 0
fi
echo "# arguments    : ${ARGS[*]}";
echo "# Jobs         : $JOBS_NB";
echo "# config path  : $CFGFILES_PATH";
if [ ! -d $CFGFILES_PATH ]; then
  echo "#    * DO NOT EXISTS ! Quit"
  return 0
fi
echo "# config files : $CFGFILES_NB";
for i in ${!CFGFILES[@]}  ; do
  echo "#    * ${CFGFILES[i]}";
  if [ ! -f $CFGFILES_PATH/${CFGFILES[i]} ]; then
    echo "#    * DO NOT EXISTS ! Quit"
    return 0
  fi
done;
echo "# runs         : $RUNS_NB";
for i in ${!RUNS[@]}      ; do echo "#    * ${RUNS[i]}"; done;
echo "# macros path  : $MACROS_PATH";
if [ ! -d $MACROS_PATH ]; then
  echo "#    * DO NOT EXISTS ! Quit"
  return 0
fi
echo "# macro files  : $MACROFILES_NB";
for i in ${!MACROFILES[@]}; do 
  echo "#    * ${MACROFILES[i]}"; 
  if [ ! -f $MACROS_PATH/${MACROFILES[i]} ]; then
    echo "#    * DO NOT EXISTS ! Quit"
    return 0
  fi
done;
echo "# logs path    : $OUTPUTLOGS_PATH";
if [ -d $OUTPUTLOGS_PATH ]; then
echo "#    * exists, OK"
else
mkdir -p $OUTPUTLOGS_PATH
echo "#    * created "
fi
echo "************************"
echo "# ALL tests passed OK !"
echo "************************"
echo "############################################"
echo ""

if [ "$ONLY_TEST" = "1" ]; then
  echo "Only test mode, now quit"
  clearVar
  return 0
fi


echo "############################################"
echo "#           JOBS STRAT                     #"
echo "############################################"
echo "# start on     : $STARTDATE";
echo "############################################"
echo ""
ijob=0
for irun in ${!RUNS[@]}; do
    for icfg in ${!CFGFILES[@]}; do    
        for imac in ${!MACROFILES[@]}; do
        
        CURRENTDATE=$(date);
        JOBSTARTTIME=$(date +"%s");
        diff=$(($JOBSTARTTIME - $STARTTIME))
        ijob=$(($ijob + 1));
        LOGFILE[ijob]="logs_RUN-${RUNS[irun]}_CFG-${CFGFILES[icfg]}_MACRO-${MACROFILES[imac]}.log"
        COMMAND="$EXEC_NAME ${ARGS[*]} -run ${RUNS[irun]} -cfg $CFGFILES_PATH/${CFGFILES[icfg]} $MACROS_PATH/${MACROFILES[imac]}  >$OUTPUTLOGS_PATH/${LOGFILE[ijob]} -suffix _RUN${RUNS[irun]}_CFG${CFGFILES[icfg]} 2>&1"
        echo "********************************"
        echo "*   Job $ijob/$JOBS_NB starts  *"
        echo "********************************"
        echo "* started on $CURRENTDATE"
        echo "* elapsed time from first job: $(($diff / 60)):$(($diff % 60))"
        echo "********************************"
        echo "* RUN     [$(($irun+1))/$RUNS_NB]: ${RUNS[irun]}"
        echo "* config  [$(($icfg+1))/$CFGFILES_NB]: ${CFGFILES[icfg]}"
        echo "* macro   [$(($imac+1))/$MACROFILES_NB]: ${MACROFILES[imac]}"
        echo "********************************"
        echo "* log file: ${LOGFILE[ijob]}"
        echo "* Command : $COMMAND"
        echo "********************************"
        echo "working ..."
        eval $COMMAND;

        CURRENTDATE=$(date);
        JOBFINISHTIME=$(date +"%s");
        diff=$(($JOBFINISHTIME - $JOBSTARTTIME))
        JOBSTIMES[ijob]="$(($diff  / 60)):$(($diff  % 60))"
        diff2=$(($JOBFINISHTIME - $STARTTIME))        
        echo "********************************"
        echo "* finished on $CURRENTDATE"
        echo "* elapsed time for this job: ${JOBSTIMES[ijob]}"
        echo "* elapsed time from 1st job: $(($diff2 / 60)):$(($diff2 % 60))"
        echo "* last 5 lines of log files:"
        echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
        tail -5 $OUTPUTLOGS_PATH/${LOGFILE[ijob]}
        echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
        echo "*******************************"
        echo ""        
        done
    done 
done
echo "############################################"
echo "#          JOBS FINISHED                   #"
echo "############################################"
FINISHDATE=$(date);
FINISHTIME=$(date +"%s");
diff=$(($FINISHTIME - $STARTTIME))

if [ "$DeleteTAFUnusedFiles" = "1" ]; 
then
  echo "#################################"
  echo "# Delete TAF unused files";
  echo "#################################"
  deleteTAFunusedFiles
  echo "#################################"
  echo "#"
fi

echo "#################################"
echo "# jobs details:";
echo "#################################"
ijob=1
while [ "$ijob" != "$(($JOBS_NB +1))" ]
do
  echo "######################"
  echo "#       job: $ijob"
  echo "######################"
  echo "#  * duration: ${JOBSTIMES[ijob]}"
  echo "#  * log file: ${LOGFILE[ijob]}"
  echo "#  * 5 last lines:"
  echo "#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
  tail -5 $OUTPUTLOGS_PATH/${LOGFILE[ijob]}
  echo "#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
  echo "#"
  ijob=$(($ijob + 1));
done
 ijob=$(($ijob - 1));
echo "#################################"
echo "#"

echo "#################################"
echo "# jobs summary";
echo "#################################"
echo "# started on      : $STARTDATE";
echo "# finished  on    : $FINISHDATE";
echo "# processing time : $(($diff / 60)):$(($diff % 60))"
echo "# processed jobs  : $ijob/$JOBS_NB";
echo "# jobs time       :";
ijob=1
while [ "$ijob" != "$(($JOBS_NB +1))" ]
do
  echo "#  * job: $ijob ${JOBSTIMES[ijob]}"
  ijob=$(($ijob + 1));
done
echo "#################################"
echo "#"

cd $PREVIOUSPWD;
clearVar
echo "############################################"
return 0