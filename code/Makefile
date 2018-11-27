# TAF Makefile
#
# Last update CF 2011/04: include the standard ROOT Makefile.arch
# Last update JB 2011/04/12: ROOTDIR replaced by DTDIR
# Last update JB 2012/10/30: link with Eve lib removed
# Last update JB 2013/06/26: debug flag -g added
# Last update JB 2013/08/20: HistPainter removed and option for large file on LINUX
# Last update JB 2013/11/15: option -Wl removed in shared libs creation
# Last update JB 2014/03/06: option -g removed, -w added to limit warnings
# Last update VR 2014/03/30: BUILD_DIR and co. are directly definied from DTDIR
# Last update VR 2014/03/30: inlude Makefile.arch from ROOTSYS (or given)
# Last update JB 2014/05/13: new BoardReaders classes
# Last update JB 2014/05/14: output specified first in exe creation
# Last update LC 2014/12/15: Remove DMonteCarlo et DHitMonteCarlo
# Last update LC 2014/15/19: Add DMiniVector.h DMiniVector.cxx
# Last update JB 2018/09/06: remove dependencies to tcsh, switch to sh
# Last update JB 2018/11/08: cp .pcm files to the lib directory

# ----------------------------------------
#            Makefile.arch
# This file is available in the ROOT installation folder ($(ROOTSYS)/etc/Makefile.arch)
# It defines compilation flags according to your system and the ROOT version, to compile projects using ROOT
# You should use the Makefile.arch provided with your installation (choice 1a or 1b depending on ROOT VERSION)
# If not available, you can use the given Makefile.arch (choice 2)

include $(ROOTSYS)/etc/Makefile.arch # choice 1a
#include $(ROOTSYS)/test/Makefile.arch # choice 1b (old ROOT versions)
#include ./Makefile.arch # choice 2
# ----------------------------------------
#        SVN revision
# TAF welcome message displays the SVN revision of the code used during the compilation
# The 'svnversion' software must be installed for this functionality, if not, comment both two lines
SVNREV     := -D'SVN_REVISION="$(shell svnversion -n .)"'
CXXFLAGS   += $(SVNREV)
# ----------------------------------------


BUILD_DIR   = $(DTDIR)/bin/bin
OBJ_DIR     = $(DTDIR)/bin/obj
LIB_DIR     = $(DTDIR)/bin/lib


vpath	%.h 			$(DTDIR)/code/include
vpath 	%.$(SrcSuf)		$(OBJ_DIR)
vpath 	%.$(SrcSuf)		$(DTDIR)/code/src
vpath 	%.$(ObjSuf) 		$(OBJ_DIR)
vpath 	%.$(DllSuf) 		$(LIB_DIR)

# Uncomment -g if you need debugger
CXXFLAGS   += -I$(DTDIR)/code/include -DCC_64B -D_FILE_OFFSET_BITS=64 -Wno-unused-variable -Wno-unused-parameter #-w #-std=c++0x # -g # new flag g++<4.7 : -std=c++0x g++>=4.7 -std=c++11 to add std library tools.
LIBS        = $(ROOTLIBS)
GLIBS       = $(ROOTGLIBS)
LIBSALL     = $(ROOTLIBS) -lMinuit -lGeom -lEG -lGui -lGed -lTreePlayer -lMatrix -lPhysics -lMathCore -lSpectrum \
-lTMVA -L/sw/lib #-ltiff

#------------------------------------------------------------------------------

DTHDRS          = TNTBoardReader.h PXIBoardReader.h PXIeBoardReader.h \
		GIGBoardReader.h IMGBoardReader.h BoardReader.h VMEBoardReader.h  MCBoardReader.h BoardReaderIHEP.h \
    AliMIMOSA22RawStreamVASingle.h DecoderM18.h DecoderGeant.h \
    DSession.h DSetup.h \
		DAcq.h DTracker.h DPlane.h DStrip.h \
    DHit.h DTrack.h DLine.h DR3.h DCut.h DAlign.h \
		DEvent.h  DEventMC.h  DParticle.h DGlobalTools.h \
    DPrecAlign.h DPixel.h DLadder.h DMiniVector.h DHelix.h DHelixFitter.h\
    DTrackFitter.h DBeaster.h\
# DXRay2DPdf.h


MHDRS		= MAnalysis.h  MAlign.h MHist.h MRaw.h MRax.h MAlignment.h MMillepede.h MGlobalAlign.h MKalmanFilter.h MLeastChiSquare.h

DTSRCS		= MMain.cxx \
		TNTBoardReader.cxx PXIBoardReader.cxx PXIeBoardReader.cxx \
		GIGBoardReader.cxx IMGBoardReader.cxx BoardReader.cxx VMEBoardReader.cxx MCBoardReader.cxx BoardReaderIHEP.cxx \
    AliMIMOSA22RawStreamVASingle.cxx DecoderM18.cxx DecoderGeant.cxx \
    DSession.cxx DSetup.cxx \
		DAcq.cxx DTracker.cxx  DPlane.cxx DStrip.cxx  \
		DHit.cxx DTrack.cxx DLine.cxx DR3.cxx DCut.cxx DAlign.cxx \
		DEvent.cxx  DEventMC.cxx  DParticle.cxx DGlobalTools.cxx \
		DPrecAlign.cxx DPixel.cxx DLadder.cxx DMiniVector.cxx DHelix.cxx DHelixFitter.cxx \
    DTrackFitter.cxx DBeaster.cxx\
# DXRay2DPdf.cxx

MSRCS		= MPrep.cxx MAnalysis.cxx MPost.cxx MCommands.cxx  MMCGeneration.cxx  MAlign.cxx MHist.cxx MRaw.cxx MRax.cxx \
           MAlignment.cxx MMillepede.cxx MGlobalAlign.cxx MKalmanFilter.cxx MLeastChiSquare.cxx

PROGNAME    = TAF

MOBJS		= $(MSRCS:.cxx=.o)

DTOBJS		= $(DTSRCS:.cxx=.o)

PROGRAM		= $(BUILD_DIR)/$(PROGNAME)

##########TARGETS##########

all:	$(PROGRAM)

$(OBJ_DIR):
		@/bin/sh -cf 'if [ ! -d $(BUILD_DIR) ]; then mkdir -p  $(BUILD_DIR); fi'
		@/bin/sh -cf 'if [ ! -d $(OBJ_DIR) ]; then mkdir -p  $(OBJ_DIR); fi'
		@/bin/sh -cf 'if [ ! -d $(LIB_DIR) ]; then mkdir -p $(LIB_DIR); fi'

libTAF.$(DllSuf): Mdict.$(ObjSuf) $(MOBJS) $(MSRCS) $(MHDRS)
		@echo ""
		@echo " *********************************************"
		@echo " Creating shared lib" libTAF.$(DllSuf)
		@echo " *********************************************"
		$(LD) $(SOFLAGS)   $(LDFLAGS) $(OBJ_DIR)/M*.$(ObjSuf) $(OutPutOpt) $(LIB_DIR)/libTAF.$(DllSuf)
#		$(LD) $(SOFLAGS) -Wl  $(LDFLAGS) $(OBJ_DIR)/M*.$(ObjSuf) $(OutPutOpt) $(LIB_DIR)/libTAF.$(DllSuf)

libDTools.$(DllSuf): DTdict.$(ObjSuf) $(DTOBJS) $(DTSRCS) $(DTHDRS)
		@echo ""
		@echo " *********************************************"
		@echo " Creating shared lib" libDTools.$(DllSuf)
		@echo " *********************************************"
		$(LD) $(SOFLAGS)   $(LDFLAGS) $(OBJ_DIR)/D*.$(ObjSuf) \
    $(OBJ_DIR)/*BoardReader.$(ObjSuf) $(OBJ_DIR)/BoardReaderIHEP.$(ObjSuf) \
    $(OBJ_DIR)/AliMIMOSA22RawStreamVASingle.$(ObjSuf) \
		$(OutPutOpt) $(LIB_DIR)/libDTools.$(DllSuf)


$(PROGRAM):	$(OBJ_DIR)  MMain.$(ObjSuf) libDTools.$(DllSuf) libTAF.$(DllSuf)
		@echo ""
		@echo " *********************************************"
	      	@echo " Creating executable file                     "
		@echo " *********************************************"
		$(LD) $(LDFLAGS)  $(OBJ_DIR)/MMain.$(ObjSuf)  $(LIB_DIR)/lib*  $(LIBSALL)  $(OutPutOpt) $(PROGRAM)
		@echo ""
		@echo " ******************** :-) ********************"
	      	@echo "                $(PROGNAME) built successfully !"

clean:
		@rm -rf **/*~ **/\#* $(DTDIR)/test.dat  $(LIB_DIR) $(OBJ_DIR) $(BUILD_DIR)

tar:		clean
		@tar czvf $(DTDIR)/TAF_`date +%m%d%y`.tar.gz -C $(DTDIR) config code  Scripts

tar-results:	clean
		@tar czvf $(DTDIR)/TAF_results_`date +%m%d%y`.tar.gz -C $(DTDIR) result* Results
#---------------------------------------------------

##########RULES############
.SUFFIXES: .cxx

DTdict.$(SrcSuf): $(DTHDRS) DTLinkDef.h
		@echo ""
		@echo " *********************************************"
	      	@echo " Generating dictionary using  : \n $(DTHDRS) "
		@echo " *********************************************"
		@rootcint -f $(OBJ_DIR)/DTdict.$(SrcSuf) -c -Iinclude/ $(DTHDRS) include/DTLinkDef.h
		@/bin/sh -cf 'if [ -e $(OBJ_DIR)/DTdict_rdict.pcm ]; then cp $(OBJ_DIR)/DTdict_rdict.pcm $(LIB_DIR); fi'

Mdict.$(SrcSuf): $(MHDRS) MLinkDef.h
		@echo ""
		@echo " *********************************************"
		@echo " Generating dictionary using  : \n $(MHDRS) "
		@echo " *********************************************"
		@rootcint -f $(OBJ_DIR)/Mdict.$(SrcSuf) -c -Iinclude/ $(MHDRS) include/MLinkDef.h
		@/bin/sh -cf 'if [ -e $(OBJ_DIR)/Mdict_rdict.pcm ]; then cp $(OBJ_DIR)/Mdict_rdict.pcm $(LIB_DIR); fi'

DTdict.$(ObjSuf): DTdict.$(SrcSuf)
		$(CXX) $(CXXFLAGS) -c $(OBJ_DIR)/DTdict.$(SrcSuf)  -o $(OBJ_DIR)/DTdict.$(ObjSuf)

Mdict.$(ObjSuf): Mdict.$(SrcSuf)
		$(CXX) $(CXXFLAGS) -c $(OBJ_DIR)/Mdict.$(SrcSuf)  -o $(OBJ_DIR)/Mdict.$(ObjSuf)

.$(SrcSuf).$(ObjSuf):
		$(CXX) $(CXXFLAGS) -c $<  -o $(OBJ_DIR)/$@
