#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class    DSession+;
#pragma link C++ class    TNTBoardReader+;
#pragma link C++ class    TNTEvent+;
#pragma link C++ class    TNTPixel+;
#pragma link C++ class    PXIBoardReader+;
#pragma link C++ class    PXIEvent+;
#pragma link C++ class    PXIPixel+;
#pragma link C++ class    PXIeBoardReader+;
#pragma link C++ class    PXIeEvent+;
#pragma link C++ class    PXIePixel+;
#pragma link C++ class    GIGBoardReader+;
#pragma link C++ class    GIGEvent+;
#pragma link C++ class    GIGPixel+;
#pragma link C++ class    GIGMonteCarlo+;
#pragma link C++ class    IMGBoardReader+;
#pragma link C++ class    IMGEvent+;
#pragma link C++ class    IMGPixel+;
#pragma link C++ class    BoardReaderEvent+;
#pragma link C++ class    BoardReaderPixel+;
#pragma link C++ class    VMEBoardReader+;
#pragma link C++ class    MCBoardReader+;
#pragma link C++ class    BoardReaderIHEP+; // 2018/10/09
#pragma link C++ class    AliMIMOSA22RawStreamVASingle+;
#pragma link C++ class    DecoderM18+;
#pragma link C++ class    DecoderGeant+;
#pragma link C++ class    DSetup+;
#pragma link C++ class    DAcq+;
#pragma link C++ class    DTracker+;
#pragma link C++ class    DPlane+;
#pragma link C++ class    DStrip+;
#pragma link C++ class    DTrack+;
#pragma link C++ class    DLine+;
#pragma link C++ class    DHit+;
#pragma link C++ class    DR3+;
#pragma link C++ class    DCut+;
#pragma link C++ class    DAlign+;
#pragma link C++ class    DParticle+;
#pragma link C++ class    DEventHeader+;
#pragma link C++ class    DEvent+;
#pragma link C++ class    DAuthenticPlane+;
#pragma link C++ class    DTransparentPlane+;
#pragma link C++ class    DAuthenticHit+;
#pragma link C++ class    DEventMC+;
#pragma link C++ class    DGlobalTools+;
#pragma link C++ class    DataPoints+;
#pragma link C++ class    DPrecAlign-; // custom streamer
#pragma link C++ class    DPixel+;
//#pragma link C++ class    DMonteCarlo+; // LC : 2014/12/15 : Removed
#pragma link C++ class    DLadder+;
//#pragma link C++ class    DHitMonteCarlo+; // LC : 2014/12/15 : Removed
#pragma link C++ class    MiniVector+; // LC : 2014/12/19
#pragma link C++ class    DBeaster+; // DC : 2017/03/08
#pragma link C++ class    DHelixFitter+; // DC : 2017/07
#pragma link C++ class    DHelix+; // DC : 2017/09
//#pragma link C++ class    BoardReaderIHEP+; // JB, 2018/06/20

#ifdef UseROOFIT
//#pragma link C++ class    DXRay2DPdf+;
#endif

//#pragma link C++ global   gPI;
//#pragma link C++ global   gEULER;
//#pragma link C++ global   gSpeedOfLight;
//#pragma link C++ function gPoly(float,float*,int);
//#pragma link C++ global   gTool;

// and add more CERNLIB functions
//#pragma link C++   function denlan_;

//#pragma link C++ global   tSession;

#endif
