ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)


CXX           = g++
CXXFLAGS      = -g -Wall -fPIC -Wno-deprecated

NGLIBS         = $(ROOTGLIBS) 
NGLIBS         += -lMinuit  -lFoam 

CXXFLAGS      += $(ROOTCFLAGS)
CXX           += -I./
LIBS           = $(ROOTLIBS) 

GLIBS          = $(filter-out -lNew, $(NGLIBS))

CXX	      += -I./lib/
OUTLIB	      = ./lib/
.SUFFIXES: .cc,.C
.PREFIXES: ./lib/

#----------------------------------------------------#

main: macro.cc lib/Spatial_Resolution_BeamEScan.o
	$(CXX) $(CXXFLAGS) -o SpatialResolutionApp $(OUTLIB)/*.o  $(GLIBS) $<

#Main code:
lib/Spatial_Resolution_BeamEScan.o:
	$(CXX) $(CXXFLAGS) -c -I. -o $(OUTLIB)Spatial_Resolution_BeamEScan.o $<
lib/Spatial_Resolution_BeamEScan.o: src/Spatial_Resolution_BeamEScan.cc

clean:
	rm -f src/*~
	rm -f *~
	rm -f $(OUTLIB)*.o
	rm -f SpatialResolutionApp

#----------------------------------------------------#
