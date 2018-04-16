/***************************************************************************//**
 * SVN File:    $Id$
 *
 * Project:     MLeastChiSquare applications
 *
 * @class:      MLeastChiSquare
 *
 * @brief:      
 *
 * Description: Particle/sensor/layer info	
 *
 *
 * @createdby:  LIU Qingyuan <liuqingyuan678@gmail.com> at 2016-05-20 11:19:00
 * @copyright:  (c)2016 IPHC & HEPG - Shandong University. All Rights Reserved.
 *
 * @lastchange: $Revision$
 *              $Author$
 *              $Date$
 *
 *******************************************************************************/
//
//_____________ constants ________________________________
Double_t Gmass = 139.57e-3;//0.511e-3;//139.57e-3;//105.658e-3;
Double_t Gp = 1.;//450e-3;//120;
Double_t SensorRes = 10;//3.5um
// material related
Double_t OneOverCosL = 1./cos(10.*TMath::Pi()/180); // lambda correction for materials

//
//_______________Pseudo-Efficiency related________________
// Noise density
Double_t frequency = 170e3; //Hz, at R = 73mm on one wire out of 40 wires
Double_t timeRes = 190e-6; // sec, integration time
Double_t rRef = 77.88e3;//Sense wire! 73.0e3;// um, R = 73 mm
Double_t length1st = 780e3;//current inner chamber// SPT: 380e3; // um
Double_t RHitsBall = sqrt(rRef*rRef+length1st/2*length1st/2);
Double_t noiseDens = frequency*40.*timeRes /(2*TMath::Pi()*RHitsBall*length1st);// nHits um^-2 at the side of 1st wire, length of the wire ~ 380mm
noiseDens = noiseDens*(RHitsBall*RHitsBall)/(rRef*rRef); // nHits um^-2 at the centre of the 1st wire during the readout period.
// Sensor Eff
Double_t SensorEff = 0.999;//

//--------------------Layers----------------------------
enum {kZPos, kTrackingLayer, kThickness, kXOverX0, kSigmaLayer,kNProperty};
// ============ 3 planes Telescope ==============
Double_t Layers_3Pl[][kNProperty] = {
     {  0,      0,      50,     OneOverCosL*0.000534188,    999}, // um
     {  31500,  0,      2200,   OneOverCosL*0.010400000,    999}, // um
     {  72580,  1,      50,     OneOverCosL*0.000534188,    SensorRes},
     {  72610,  0,      10,     OneOverCosL*3.33333e-05,    999},
     {  72665,  0,      100,    OneOverCosL*0.000357143,    999},
     {  72740,  0,      50,     OneOverCosL*0.00056243,     999},
     {  72770,  0,      10,     OneOverCosL*3.33333e-05,    999},
     {  73030,  0,      500,    OneOverCosL*0.0021097,      999},
     {  86160,  1,      50,     OneOverCosL*0.000534188,    SensorRes},
     {  86190,  0,      10,     OneOverCosL*3.33333e-05,    999},
     {  86245,  0,      100,    OneOverCosL*0.000357143,    999},
     {  86320,  0,      50,     OneOverCosL*0.00056243,     999},
     {  86350,  0,      10,     OneOverCosL*3.33333e-05,    999},
     {  86610,  0,      500,    OneOverCosL*0.0021097,      999},
     {  99500,  1,      50,     OneOverCosL*0.000534188,    SensorRes},
     {  99530,  0,      10,     OneOverCosL*3.33333e-05,    999},
     {  99585,  0,      100,    OneOverCosL*0.000357143,    999},
     {  99660,  0,      50,     OneOverCosL*0.00056243,     999},
     {  99690,  0,      10,     OneOverCosL*3.33333e-05,    999},
     {  99950,  0,      500,    OneOverCosL*0.0021097,      999}
     ///{  72610-700,  0,      10,     5*3.33333e-05,    999},
     ///{  72665-700,  0,      100,    0.000357143,    999},
     ///{  72740-700,  0,      50,     0.00056243,     999},
     ///{  72770-700,  0,      10,     5*3.33333e-05,    999},
     ///{  73030-700,  0,      500,    0.0021097,      999},
     ///{  72580,  1,      50,     0.000534188,    10.},
     ///{  86190-700,  0,      10,     5*3.33333e-05,    999},
     ///{  86245-700,  0,      100,    0.000357143,    999},
     ///{  86320-700,  0,      50,     0.00056243,     999},
     ///{  86350-700,  0,      10,     5*3.33333e-05,    999},
     ///{  86610-700,  0,      500,    0.0021097,      999},
     ///{  86160,  1,      50,     0.000534188,    10.},
     ///{  99530-700,  0,      10,     5*3.33333e-05,    999},
     ///{  99585-700,  0,      100,    0.000357143,    999},
     ///{  99660-700,  0,      50,     0.00056243,     999},
     ///{  99690-700,  0,      10,     5*3.33333e-05,    999},
     ///{  99950-700,  0,      500,    0.0021097,      999}
     ///{  99500,  1,      50,     0.000534188,    10.},
};
// Double_t Layers_3Pl[][kNProperty] = {
//      {      0,   0, 50, 5.34188034188034242e-04, SensorRes},
//      {72.58e3,   1, 50, 5.34188034188034242e-04, SensorRes},
//      {72.61e3,   0, 10, 3.33333333333333349e-05, SensorRes},
//      {72.64e3,   0, 50, 1.74216027874564460e-04, SensorRes},
//      {86.16e3,   1, 50, 5.34188034188034242e-04, SensorRes},
//      {86.19e3,   0, 10, 3.33333333333333349e-05, SensorRes},
//      {86.22e3,   0, 50, 1.74216027874564460e-04, SensorRes},
//      {99.50e3,   1, 50, 5.34188034188034242e-04, SensorRes},
//      {99.53e3,   0, 10, 3.33333333333333349e-05, SensorRes},
//      {99.56e3,   0, 50, 1.74216027874564460e-04, SensorRes}
// };
Int_t nLayers_3Pl = sizeof(Layers_3Pl)/sizeof(Layers_3Pl[0]);

// ============ 4 planes Telescope ==============
Double_t Layers_4Pl[][kNProperty] = {
     {  0,      0,      50,     OneOverCosL*0.000534188,    999},
     {  31500,  0,      2200,   OneOverCosL*0.010400000,    999},
     {  72580,  1,      50,     OneOverCosL*0.000534188,    SensorRes},
     {  72610,  0,      10,     OneOverCosL*3.33333e-05,    999},
     {  72665,  0,      100,    OneOverCosL*0.000357143,    999},
     {  72740,  0,      50,     OneOverCosL*0.00056243,     999},
     {  72770,  0,      10,     OneOverCosL*3.33333e-05,    999},
     {  73030,  0,      500,    OneOverCosL*0.0021097,      999},
     {  73290,  0,      10,     OneOverCosL*3.33333e-05,    999},
     {  73320,  0,      50,     OneOverCosL*0.00056243,     999},
     {  73395,  0,      100,    OneOverCosL*0.000357143,    999},
     {  73450,  0,      10,     OneOverCosL*3.33333e-05,    999},
     {  73480,  1,      50,     OneOverCosL*0.000534188,    SensorRes},
     //{  98600,  1,      50,     OneOverCosL*0.000534188,    SensorRes},
     //{  98630,  0,      10,     OneOverCosL*3.33333e-05,    999},
     //{  98685,  0,      100,    OneOverCosL*0.000357143,    999},
     //{  98760,  0,      50,     OneOverCosL*0.00056243,     999},
     //{  98790,  0,      10,     OneOverCosL*3.33333e-05,    999},
     //{  99050,  0,      500,    OneOverCosL*0.0021097,      999},
     //{  99310,  0,      10,     OneOverCosL*3.33333e-05,    999},
     //{  99340,  0,      50,     OneOverCosL*0.00056243,     999},
     //{  99415,  0,      100,    OneOverCosL*0.000357143,    999},
     //{  99470,  0,      10,     OneOverCosL*3.33333e-05,    999},
     //{  99500,  1,      50,     OneOverCosL*0.000534188,    SensorRes}

     {  86160,  1,      50,     OneOverCosL*0.000534188,    SensorRes},
     {  86190,  0,      10,     OneOverCosL*3.33333e-05,    999},
     {  86245,  0,      100,    OneOverCosL*0.000357143,    999},
     {  86320,  0,      50,     OneOverCosL*0.00056243,     999},
     {  86350,  0,      10,     OneOverCosL*3.33333e-05,    999},
     {  86610,  0,      500,    OneOverCosL*0.0021097,      999},
     {  99500,  1,      50,     OneOverCosL*0.000534188,    SensorRes},
     {  99530,  0,      10,     OneOverCosL*3.33333e-05,    999},
     {  99585,  0,      100,    OneOverCosL*0.000357143,    999},
     {  99660,  0,      50,     OneOverCosL*0.00056243,     999},
     {  99690,  0,      10,     OneOverCosL*3.33333e-05,    999},
     {  99950,  0,      500,    OneOverCosL*0.0021097,      999}
/*
     {      0,   0, 50, 5.34188034188034242e-04, SensorRes},
     {72.58e3,   1, 50, 5.34188034188034242e-04, SensorRes},
     {72.61e3,   0, 10, 3.33333333333333349e-05, SensorRes},
     {72.64e3,   0, 50, 1.74216027874564460e-04, SensorRes},
     {74.52e3,   1, 50, 5.34188034188034242e-04, SensorRes},
     {74.55e3,   0, 10, 3.33333333333333349e-05, SensorRes},
     {74.58e3,   0, 50, 1.74216027874564460e-04, SensorRes},
     {86.16e3,   1, 50, 5.34188034188034242e-04, SensorRes},
     {86.19e3,   0, 10, 3.33333333333333349e-05, SensorRes},
     {86.22e3,   0, 50, 1.74216027874564460e-04, SensorRes},
     {99.50e3,   1, 50, 5.34188034188034242e-04, SensorRes},
     {99.53e3,   0, 10, 3.33333333333333349e-05, SensorRes},
     {99.56e3,   0, 50, 1.74216027874564460e-04, SensorRes}
*/
};
Int_t nLayers_4Pl = sizeof(Layers_4Pl)/sizeof(Layers_4Pl[0]);
