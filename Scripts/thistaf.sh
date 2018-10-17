# Source this script to set up :
#   - the TAF environment variable used in Makefile, code, scripts and macros : DTDIR
#   - others environment variables (PATH, LD_LIBRARY_PATH)
#
# Conveniently alias like this can be defined in .bashrc:
#   alias sourceTAF=". /opt/taf/trunk/scripts/thistaf.sh"
#
# This script is for the bash like shells
#
# Author: Valerian.REITHINGER@ipnl.in2p3.fr , 07/01/2014
# Adapted from "thisroot.sh" from CERN ROOT software, by Fons Rademakers, 18/8/2006
# Modifications:
#  17/10/2018, JB, new variable ROOT_INCLUDE_PATH

##################################################
#                CONFIGURATION
#  Configuration for script behavior
##################################################
# change directory to DTDIR when source this script ?
#GoToDTDIR=TRUE #Comment this line to disable

# Print info on terminal (environment variables) ?
PrintInfos=TRUE #Comment this line to disable

##################################################
#         OPTIONAL CONFIGURATION
#  Needed if a warning message is displayed
#  Following values are ignored if not needed
##################################################
# CERN ROOT software location
# If you can run 'root' or 'root-config' from your terminal, skip this optionnal configuration step
#   ROOT is normally ready to use after sourcing the 'thisroot.sh' script in it's bin folder (ROOT recommendation)
#     (you can add this line to your ~/.bashrc : source /where/root/is/bin/thisroot.sh to do it automatically )
#     Then, some environment variables are set : ROOTSYS PATH LD_LIBRARY_PATH MANPATH DYLD_LIBRARY_PATH PYTHONPATH SHLIB_PATH ...
#     Those env. var. are needed to run root (PATH) and to compile TAF (LD_LIBRARY_PATH, ...)
#   This script will detect if this step is made
#     If YES, no ROOT configuration is made, and following lines are ignored (ROOTLOCATION)
#     If NO, the 'thisroot.sh' script is executed from $ROOTLOCATION/bin/thisroot.sh

#ROOTLOCATION=/libcern/root/5.34.15/debian-6.0.8-i386/ # exemple for IPHC sbgat576 computer
#ROOTLOCATION=/libcern/root/5.34.18/sl6.3-x86_64/ # exemple for sbguiX computer
#ROOTLOCATION=/cern/2011/root.5.28.00.gcc.4.1.2 # exemple for IPHC sbgat576 computer
#ROOTLOCATION=/Users/jeromeb/Library/Root/root # exemple for a lambda computer
#ROOTLOCATION=""

##################################################
#                  WORK
#          (should not be edited)
##################################################
# Function used to delete "old" ($2) paths from environment variables given ($1), result in $newpath
drop_from_path()
{
   # Assert that we got enough arguments
   if test $# -ne 2 ; then
      echo "drop_from_path: needs 2 arguments"
      return 1
   fi

   p=$1
   drop=$2

   newpath=`echo $p | sed -e "s;:${drop}:;:;g" \
                          -e "s;:${drop};;g"   \
                          -e "s;${drop}:;;g"   \
                          -e "s;${drop};;g"`
}

# Check prerequisites : cern ROOT configuration
if [ -z "${ROOTSYS}" ] ; then #if ROOTSYS is not defined
   echo "<INFO> ROOTSYS is not defined, trying to configure ROOT ..."
   if [ -z "${ROOTLOCATION}" ] ; then # if ROOTLOCATION is not defined (see top of this file)
      echo "<ERROR> ROOTLOCATION is not configured in thistaf.sh, please edit this script"
      return 1
   else # ROOTLOCATION is defined
      if [ -f $ROOTLOCATION/bin/thisroot.sh ]; then # if thisroot.sh is finded
         echo "<INFO> running $ROOTLOCATION/bin/thisroot.sh to configure ROOT"
         source $ROOTLOCATION/bin/thisroot.sh
      else # thisroot.sh is NOT finded
         echo "<ERROR> Can't find thisroot.sh in $ROOTLOCATION/bin, please check ROOTLOCATION in thistaf.sh"
         return 1
      fi
   fi
fi

# If DTDIR set before, backup it to clean environment variables later
if [ -n "${DTDIR}" ] ; then
   old_dtdir=${DTDIR}
   echo "<WARNING> $old_dtdir taf version settings are curently used and will be replaced!"
fi


# If this script is direclty executed, without arguments (this is not the normal way !)
if [ "x${BASH_ARGV[0]}" = "x" ]; then
    echo "<WARNING> this is not the normal way to configure taf !"
    # If this script is not called from the proper path
    if [ ! -f Scripts/thistaf.sh ]; then
       echo "<ERROR> must $ cd where/taf/is before calling $ source Scripts/thistaf.sh"
       DTDIR=;  export DTDIR
       return 1
    fi
    # If this script is called from the proper path
    DTDIR="$PWD"; export DTDIR

# If this script is called with argument, ie via $ source /opt/taf/scripts/thistaf.sh (normal way)
else
    thistaf=$(dirname ${BASH_ARGV[0]}) #get the directory of this file from the last argument of source or "."
    DTDIR=$(cd ${thistaf}/..;pwd); export DTDIR #remove the "Scripts/thistaf.sh" part of argument to get the main path of TAF
fi


# If a previous $DTDIR was set before calling this script : clean entries
if [ -n "${old_dtdir}" ] ; then
   echo "<INFO> $old_dtdir taf version settings are cleaned"
   # clean PATH
   if [ -n "${PATH}" ]; then
      drop_from_path "$PATH" ${old_dtdir}/bin/bin;
      drop_from_path "$newpath" ${old_dtdir}/Scripts;
      PATH=$newpath
   fi
   # clean LD_LIBRARY_PATH
   if [ -n "${LD_LIBRARY_PATH}" ]; then
      drop_from_path "$LD_LIBRARY_PATH" ${old_dtdir}/bin/lib;
      LD_LIBRARY_PATH=$newpath
   fi
fi

# set $PATH
if [ -z "${PATH}" ]; then
   PATH=$DTDIR/bin/bin;
   PATH=$DTDIR/Scripts:$PATH;
   export PATH
else
   PATH=$DTDIR/bin/bin:$PATH;
   PATH=$DTDIR/Scripts:$PATH;
   export PATH
fi

# set $LD_LIBRARY_PATH
if [ -z "${LD_LIBRARY_PATH}" ]; then
   LD_LIBRARY_PATH=$DTDIR/bin/lib;
   export LD_LIBRARY_PATH
else
   LD_LIBRARY_PATH=$DTDIR/bin/lib:$LD_LIBRARY_PATH;
   export LD_LIBRARY_PATH
fi

# set $ROOT_INCLUDE_PATH
if [ -z "${ROOT_INCLUDE_PATH}" ]; then
   ROOT_INCLUDE_PATH=$DTDIR/code/include;
   export ROOT_INCLUDE_PATH
else
   ROOT_INCLUDE_PATH=$DTDIR/code/include:$ROOT_INCLUDE_PATH;
   export ROOT_INCLUDE_PATH
fi

export TMVASYS=$ROOTSYS/tmva/

# Print infos on terminal : environment variables, ...
if [ -n "${PrintInfos}" ] ; then
	echo "<INFO> TAF settings are set for: $DTDIR"
	echo ""
	echo "<INFO> Environment variables :"
	echo " DTDIR           = $DTDIR"
	echo " ROOTSYS         = $ROOTSYS"
  echo " ROOT_INCLUDE_PATH = $ROOT_INCLUDE_PATH"
	echo " LD_LIBRARY_PATH = $LD_LIBRARY_PATH"
        echo " TMVASYS         = $TMVASYS"
	echo ""
	echo "<INFO> Documentation files (index.hmtl, taf_shortDoc.pdf and maf_doc.pdf) sit in DTDIR/doc"
	echo "<INFO> To compile : run 'maketaf'"
	echo "<INFO> To lauch TAF, run 'TAF' or 'taf'"
	echo ""
fi


# go to DTDIR
if [ -n "${GoToDTDIR}" ] ; then
	cd $DTDIR;
	echo "<INFO> Current directory changed to TAF directory !"
fi


# alias
alias taf='TAF'
alias cdtaf='cd $DTDIR/code'
alias kitaf='killall TAF'
alias killtaf='killall TAF'


unset old_dtdir
unset thistaf
unset GoToDTDIR
unset PrintInfos
