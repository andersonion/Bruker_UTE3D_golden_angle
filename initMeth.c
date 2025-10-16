/* ***************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/UTE3D/initMeth.c,v $
 *
 * Copyright (c) 2009
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: initMeth.c,v 1.31 2013/05/22 10:55:23 sako Exp $
 *
 * ***************************************************************/

static const char resid[] = "$Id: initMeth.c,v 1.31 2013/05/22 10:55:23 sako Exp $ (C) 2009 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE       0
#define DB_LINE_NR	0




#include "method.h"
/* ---- local GA helpers (initMeth.c) ---- */
static int fib_by_index(int k){
  if (k <= 1) return 1;
  int a=1,b=1; for (int i=2;i<=k;++i){ int t=a+b; a=b; b=t; }
  return b;
}



void initMeth()
/*:=MPE=:=======================================================*/
{


	/* ---- GA derive (runs every time; does not overwrite inputs) ---- */
	if (GA_NSpokesReq < 1) GA_NSpokesReq = 1;
	if (GA_FibIndex   < 2) GA_FibIndex   = 2;
	
	if (GA_UseFibonacci == Yes) {
	  GA_FibValue   = fib_by_index(GA_FibIndex);  /* make sure fib_by_index is defined above */
	  GA_NSpokesEff = GA_FibValue;
	} else {
	  GA_FibValue   = 0;
	  GA_NSpokesEff = GA_NSpokesReq;
	}



  int dimRange[2]   = {3, 3};
  int lowMat[3]     = {16, 16, 16};
  int upMat[3]      = {512, 512, 512};
  int defaultMat[3] = {128, 128, 128};

  DB_MSG(( "--> initMeth" ));

  /* which version of toolboxes should be active */  
  PTB_VersionRequirement( Yes,20100101,"");


  /* initialize nucleus */  
  STB_InitNuclei(1);

  /* initialize rf pulse parameters */  
  STB_InitRFPulse("ExcPulse1",      // name of pulse structure
                  "ExcPulse1Enum",  // name of pulse list parameter
                  "ExcPulse1Ampl",  // name of pulse amplitude parameter
                  "ExcPulse1Shape", // name of pulse shape (for calc. pulses)
                  0,                // used for excitation
                  "bp.exc",         // default shape
                  3e5,              // default bandwidth
                  5.0);             // default pulse angle

  // check valid range for this specific pulse 
  ExcPulse1Range();
  
  if(ParxRelsParHasValue("PVM_DeriveGains") == No)
    PVM_DeriveGains = Yes; 

  /* initialize digitizer parameter */
  EffSWhRange(); 
  STB_InitDigPars();

  /* encoding group */
  STB_InitEncoding(3, dimRange, lowMat, upMat, defaultMat);  

  /* initialize geometry parameters */
  STB_InitImageGeometry();
  
  /* geometry editor */
  GTB_GeoObjFreezePos0("PVM_SliceGeoObj", No);
  GTB_GeoObjFreezePos1("PVM_SliceGeoObj", No);
  GTB_GeoObjFreezePos2("PVM_SliceGeoObj", No);
  PTB_SetGeoObjShape("PVM_SliceGeoObj", Ellipsoid_In_GobjShape);

  /* not a csi experiment */
  PTB_SetSpectrocopyDims( 0, 0 );

  /* initialize method specific parameters */
  ParxRelsMakeNonEditable("NPro");
  ProUnderRange();
  ReadGradLim = 100.0;

  if(ParxRelsParHasValue("YesNoMinEchoTime") == 0)
    YesNoMinEchoTime = Yes;

  if(ParxRelsParHasValue("MinimumUnderSampling") == 0)
    MinimumUnderSampling = Yes;
 
  if(ParxRelsParHasValue("GradSync") == 0)
    GradSync = No;

  if(!ParxRelsParHasValue("RecoOnline"))
    RecoOnline = Yes;
  
  if(!ParxRelsParHasValue("B0Corr"))
    B0Corr=Yes;  

  if(!ParxRelsParHasValue("RampCompYN"))
    RampCompYN=Yes;

  if(!ParxRelsParHasValue("Spoiling"))
    Spoiling = 1.0;
  
  SlewRateRange();
  
  if(CFG_InstrumentType() == Avance_III)
    GradRes = MAX_OF(0.005,CFG_GradientShapeResolution());
  else
    GradRes = 0.010;
    
  /* initialize standard imaging parameters NA TR TE */
  RepetitionTimeRange();
  AveragesRange();
  EchoTimeRange();
  if(!ParxRelsParHasValue("PVM_NRepetitions"))
    PVM_NRepetitions = 1;
  
  /* Initialization of trajectory module */
  STB_InitTraj(1, 1, 1, 1, No, 2, Yes, 0, NULL, No, 0,
    "GradShape", NULL, NULL,
    "GradAmpR","GradAmpP","GradAmpS");

  /* Initialisation of modules */
  STB_InitTriggerModule();
  STB_InitFovSatModule();
  STB_InitFatSupModule();
  STB_InitMagTransModule();
  STB_InitDummyScans(1000.0);
  STB_InitFreqDriftCorr();

  /* initialize mapshim parameter class */
  STB_InitMapShim();

  ParxRelsMakeNonEditable("PVM_MinEchoTime,PVM_AntiAlias,RampPoints");

  /* 
   * Once all parameters have initial values, the backbone is called
   * to assure they are consistent 
   */
  
  backbone();
 
DB_MSG(("initMeth: Mode=%d  UseFib=%d  NReq=%d  k=%d  Fk=%d  NEff=%d  DefApplied=%d",
        (int)GA_Mode, (int)GA_UseFibonacci, GA_NSpokesReq, GA_FibIndex, GA_FibValue, GA_NSpokesEff, (int)GA_DefaultsApplied));

  DB_MSG(( "<-- initMeth" ));

}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/


