/* ***************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/UTE3D/parsLayout.h,v $
 *
 * Copyright (c) 2009
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: parsLayout.h,v 1.22 2013/06/20 15:18:16 sako Exp $
 *
 * ***************************************************************/

/* ***************************************************************/
/*	PARAMETER CLASS	               			       	 */
/* ***************************************************************/


/*--------------------------------------------------------------*
 * Definition of the PV class...
 *--------------------------------------------------------------*/

parclass
{
  ExcPulse1Enum;
  ExcPulse1;
  ExcPulse1Ampl;
} 
attributes
{
  display_name "RF Pulses";
} RF_Pulses;

parclass
{
  PVM_EffSWh;
  PVM_AcquisitionTime;
  Spoiling;
  RampCompYN;
  RampPoints;
  VarRampTimeYN;
  EffSlewRate;
  RampTime;
  GradSync;
  GradShape;
  GradRes;
  PVM_MinFov;
  DigitizerPars;
  RecoOnline;
}
attributes
{
  display_name "Sequence Details";
} Sequence_Details;

parclass
{
  DummyScans_Parameters;
  PVM_FreqDriftYN;
  PVM_TriggerModule;
  Trigger_Parameters;
  PVM_FovSatOnOff;
  Fov_Sat_Parameters;
  PVM_FatSupOnOff;
  Fat_Sup_Parameters;
  PVM_MagTransOnOff;
  Magn_Transfer_Parameters;
}Preparation;

parclass
{
  Method;
  YesNoMinEchoTime;
  PVM_EchoTime;
  PVM_MinEchoTime;
  PVM_RepetitionTime;
  NPro;
  ProUndersampling;
  MinimumUnderSampling;
  PVM_NAverages;
  PVM_NRepetitions;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  PVM_DeriveGains;
  RF_Pulses;
  Nuclei;
  Encoding;
  Sequence_Details;
  ImageGeometry;
  Preparation;
  TrajParameters;
  MapShim;
} MethodClass;

parclass
{
  B0Corr;
  TrajRecoParameters;
}
attributes
{
  display_name "Reconstruction Options";
}MethodRecoGroup;

/*--------------------------------------------------------------*
 * Conflicts
 *--------------------------------------------------------------*/
conflicts
{
  PVM_EchoTime;
  PVM_RepetitionTime;
  PVM_Fov;
  ProUndersampling;
};

/* ***************************************************************/
/*	E N D   O F   F I L E					 */
/* ***************************************************************/

