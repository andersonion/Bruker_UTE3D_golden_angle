/* ***************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/UTE3D/backbone.c,v $
 *
 * Copyright (c) 2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: backbone.c,v 1.24 2013/08/29 12:31:35 sako Exp $
 *
 * ***************************************************************/


static const char resid[] = "$Id: backbone.c,v 1.24 2013/08/29 12:31:35 sako Exp $ (C) 2011 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

#include "method.h"


void backbone(void)
{

  double minFov[3];
  double readGradRatio;
  double minte;
  int dim;

  DB_MSG(("-->backbone"));
  

  STB_UpdateNuclei(No);
  
  dim = PTB_GetSpatDim();

  /* calculate limits for read phase and slice */
  
  ReadGradLim     = 100.0;
 
  /* encoding */
  PVM_AntiAlias[0] = PVM_AntiAlias[1] = PVM_AntiAlias[2] = 1.0;
  STB_UpdateEncodingSpecial(3, 3, Yes);

  minFov[0] = FreqEncodingLimits( PVM_EncMatrix[0]/2,
				  PVM_AntiAlias,
				  &readGradRatio,
				  PVM_GradCalConst);

  /* constraint to isotropic Fov */
  minFov[2] = minFov[1] = minFov[0];

  /* update geometry parameters */
  UpdateGeometry(minFov);
 
  /* update excitation pulse */   
  UpdateRFPulses();
          
  /* calculation of ReadGrad, PhaseGrad and SliceGrad */
  FreqEncodingGradients(PVM_Fov[0],PVM_Fov[1],PVM_Fov[2],PVM_GradCalConst);

  if(VarRampTimeYN == Yes)
  {
    double rampT, maxGrad;

    rampT=CFG_GradientRampTime();
    maxGrad=MAX_OF(MAX_OF(ReadGrad,PhaseGrad),SliceGrad);
    RampTime = rampT * maxGrad / EffSlewRate;
    ParxRelsShowInEditor("EffSlewRate");
  }
  else
  {
    RampTime = CFG_GradientRampTime();
    ParxRelsHideInEditor("EffSlewRate");
  }  

  ParxRelsMakeNonEditable("RampTime");

  if(RampCompYN==Yes)
  {
    double points;

    points= RampTime/(2.0*PVM_DigDw);
    RampPoints = (int) (0.5 + points);
    RampCompTime = RampPoints * PVM_DigDw;
  }
  else
  {
    RampPoints = 0;
    RampCompTime = 0.0;
  }
  
 /* setting number of radial projections */
  SetNPro();

  /* handling of modules */
  double spoilerThick = MIN_OF(MIN_OF(PVM_SpatResol[0]*PVM_EncZf[0],PVM_SpatResol[1]*PVM_EncZf[1]),PVM_SpatResol[2]*PVM_EncZf[2]); 
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, spoilerThick);
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);
  STB_UpdateMagTransModule(PVM_DeriveGains);
  STB_UpdateTriggerModule();
  if(PVM_TriggerModule == On)
    PVM_TriggerMode = per_PhaseStep;


  /* update sequence timing */
  UpdateEchoTime(&minte);
  UpdateRepetitionTime();
  STB_UpdateDummyScans(PVM_RepetitionTime);

  /* update mapshim parameter class */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");

  SetBaseLevelParam();

  /* update trajectory module */
  double gradAmp[3]={ReadGrad, PhaseGrad, SliceGrad};
  double nPoints = PVM_DigNp+RampPoints; 

  STB_UpdateTraj(PVM_SpatResol, PVM_EncZf, PVM_Fov, 3, NPro, nPoints, 0, PVM_DigDw, gradAmp,
                 PtrType3x3 PVM_SPackArrGradOrient[0], PVM_TriggerModule, GradSync, 0.5, 0.0, GradRes, gradAmp); 
  UpdateAdjustments();

  SetRecoParam();
  
  DB_MSG(("<--backbone"));
  return;
}

/*-------------------------------------------------------
 * local utility routines to simplify the backbone 
 *------------------------------------------------------*/

void UpdateEchoTime(double *const minte)
{  
  
  DB_MSG(("-->UpdateEchoTime"));

  *minte = 
    ExcPulse1.Length/2  +
    0.006;             /* time for ADC_INIT */

  PVM_MinEchoTime = *minte;

  EchoTimeRange();
   
  DB_MSG(("<--UpdateEchoTime"));
  return;
}


void UpdateRepetitionTime(void)
{
  double TotalTime, shapeTime;
  double riseT, mindur;

  DB_MSG(("-->UpdateRepetitionTime"));

  /* adapt timing for sufficient spoiling */
  SpoilDur = 1e5*Spoiling/(ReadGrad*PVM_GradCalConst*PVM_SpatResol[0]);
  riseT = CFG_GradientRiseTime();
  mindur = PVM_DigEndDelOpt - 2.0 * riseT - SpoilDur;
  if(mindur>0)
  {
    SpoilDur= PVM_DigEndDelOpt - 2.0 * riseT - mindur;
    Spoiling = 1e-5*SpoilDur*ReadGrad*PVM_GradCalConst*PVM_SpatResol[0];
  }

 
  shapeTime = RampTime + 2*riseT + PVM_AcquisitionTime + RampCompTime + SpoilDur;

  PVM_MinRepetitionTime = 
      PVM_FovSatModuleTime                             +
      PVM_MagTransModuleTime                           +
      PVM_FatSupModuleTime                             +
      0.04                                             +
      CFG_AmplifierEnable()                            +      
      ExcPulse1.Length/2                               +
      PVM_EchoTime                                     +
      shapeTime;
  

  PVM_RepetitionTime=MAX_OF(PVM_MinRepetitionTime,PVM_RepetitionTime);


  TotalTime = PVM_RepetitionTime 
              * NPro
              * PVM_NAverages
              * PVM_NRepetitions;
    
  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  DB_MSG(("<--UpdateRepetitionTime"));
  return;
}

void UpdateGeometry(double *minFov)
{

  PVM_SPackArrSliceOffset[0] = PVM_SPackArrPhase1Offset[0] = PVM_SPackArrReadOffset[0] = 0.0;

  ParxRelsMakeNonEditable("PVM_SPackArrReadOffset");
  ParxRelsMakeNonEditable("PVM_SPackArrPhase1Offset");
  ParxRelsMakeNonEditable("PVM_SPackArrSliceOffset");
  ParxRelsMakeNonEditable("PVM_SPackArrReadOrient");
 
  STB_UpdateImageGeometry(3, PVM_Matrix, minFov);
  
}

double FreqEncodingLimits( int dpoints, 
			   double *antiAlias, 
			   double *const readGradRatio,
			   double gradCalConst) 
{
  double minFovRead = 0.0;


  DB_MSG(("-->FreqEncodingLimits"));

  STB_UpdateDigPars(&PVM_EffSWh,dpoints,antiAlias,&PVM_AcquisitionTime);

  *readGradRatio = 1.0;

  minFovRead     = MRT_MinReadFov(PVM_EffSWh,
				  *readGradRatio,
				  ReadGradLim,
				  100,
				  gradCalConst);


  DB_MSG(("<--FreqEncodingLimits"));

  return minFovRead;
}


void FreqEncodingGradients(double readFov,
			   double phaseFov,
                           double sliceFov,
			   double gradCalConst)
{
  DB_MSG(("-->FreqEncodingGradients"));

  ReadGrad     = MRT_ReadGrad(PVM_EffSWh,readFov,gradCalConst);
  PhaseGrad    = MRT_ReadGrad(PVM_EffSWh,phaseFov,gradCalConst);
  SliceGrad    = MRT_ReadGrad(PVM_EffSWh,sliceFov,gradCalConst);

  if(((ReadGrad - ReadGradLim) > 1.0e-3) || ((PhaseGrad -100)>1.0e-3) || ((SliceGrad -100)>1.0e-3))
  {

    UT_ReportError("FreqEncodingGradients: "
		   "Gradient exceeds its limits\n");

  }

  ReadGrad = MIN_OF(ReadGrad,ReadGradLim);
  PhaseGrad = MIN_OF(PhaseGrad,100);
  SliceGrad = MIN_OF(SliceGrad,100);

  DB_MSG(("<--FreqEncodingGradients"));
  return;
}

/*--------------------------------------------------------
 * Routine to update RF pulse parameters
 *-------------------------------------------------------*/

void UpdateRFPulses(void)
{

  /* Updates all parameters that belong to ExcPulse1 pulse structure
     (as initialized by STB_InitRFPulse see initMeth.c)
  */

  STB_UpdateRFPulse("ExcPulse1",1,PVM_DeriveGains,Conventional);


  if(PVM_DeriveGains==Yes)
  {
    ParxRelsHideInEditor("ExcPulse1Ampl");
  }
  else
  {
    ParxRelsShowInEditor("ExcPulse1Ampl");
  }

  ParxRelsShowInFile("ExcPulse1Ampl");

  DB_MSG(("<--UpdateRFPulses"));

  return;
}


/*==============================================================
 * specific functions for radial imaging 
 *==============================================================*/

/* calculate number of projections */

void CalcNPro(void)
{
  int i=0; 
  double pi = M_PI;
  int size = PVM_EncMatrix[0];
  int n_phi=0;
  int n_theta=0;
  
  n_theta = (int) ((pi * size)/(2*ProUndersampling));
  for(i=0;i<n_theta;i++)
  {
    n_phi += (int) (pi*size*sin(pi*i/n_theta));
  }
  NPro = n_phi;
  DB_MSG(("NPro = %d", NPro));
}

void SetNPro(void)
{
  
  if (MinimumUnderSampling == Yes)
  {
    ProUndersampling = 1.0;
    ParxRelsMakeNonEditable("ProUndersampling");
  }
  else
  {
    ParxRelsMakeEditable("ProUndersampling");
  }

  /* initial calculation of NPro */
  CalcNPro();
  
  /* limit number of projections */
  int maxPro = PTB_MaxArraySize();

  if(NPro > maxPro)
  { 
    /* adapt undersampling to maximum array size */
    ProUndersampling *= (double)NPro / (double)maxPro;
    CalcNPro();
  }

  /* apply memory restrictions */
  GO_online_reco = RecoOnline;
  
  if (RecoOnline == Yes)
  {
    RecoRegridNSetDefaults = Off;
    
    ProUndersampling *= RecoOptimiseMemory(PVM_Matrix[0], NPro, PVM_EncMatrix[0]/2+RampPoints, PVM_EncNReceivers, &RecoRegridNOver);
    if ((ProUndersampling < 1.0) || (ProUndersampling > 10.0))
    {
      ProUndersampling=MAX_OF(MIN_OF(ProUndersampling,10.0),1.0);
      UT_ReportError("Reco memory optimisation failed! Try smaller matrix size.");
    }

    CalcNPro(); 

    DB_MSG(("ProUnderSampling = %.4f, NPro = %d", ProUndersampling, NPro));
  }
}

/* calculate radial projections */
int SetProj3D( double *r,
	       double *p,
	       double *s,
	       const double gr,
	       const double gp,
	       const double gs)
{
  int i,j, k=0;
  double pi,angle;
  int size = PVM_EncMatrix[0];
  int n_phi=0;
  int n_theta=0;

  pi=M_PI;
  angle = 2 * pi;
  n_theta = (int) ((pi * size)/(2*ProUndersampling));
  
  for(j=0; j<n_theta;j++)
  {
    //theta = (int) (pi*j/n_theta);
    n_phi = (int) (pi*size*sin(pi*j/n_theta));
    
    for(i=0; i<n_phi; i++)
    {
      r[i+k]=sin(pi*j/n_theta)*cos(angle*i/n_phi)*gr;
      p[i+k]=sin(pi*j/n_theta)*sin(angle*i/n_phi)*gp;
      s[i+k]=cos(pi*j/n_theta)*gs; 
    }
    k += i;
  }
  return 0;
}


void UpdateAdjustments(void)
{
  DB_MSG(("-->UpdateAdjustments"));
  
  PTB_ClearAdjustments();
  STB_TrajAppendAdjustment("");
  PTB_AppendConfiguredAdjustment(per_scan, RCVR);
  
  DB_MSG(("<--UpdateAdjustments"));
}
