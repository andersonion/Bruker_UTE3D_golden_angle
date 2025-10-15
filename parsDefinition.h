/* ***************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/UTE3D/parsDefinition.h,v $
 *
 * Copyright (c) 2009
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: parsDefinition.h,v 1.17 2013/08/29 12:31:35 sako Exp $
 *
 * ***************************************************************/



/****************************************************************/
/* INCLUDE FILES						*/
/****************************************************************/

double parameter {editable false;}GradRes;
double parameter {editable false;}ReadGradLim; 
double parameter {editable false;}ReadGrad;
double parameter {editable false;}PhaseGrad;
double parameter {editable false;}SliceGrad;
double parameter {editable false;}RampCompTime;
int    parameter {editable false;}RampPoints;
double parameter GradAmpR[];
double parameter GradAmpP[];
double parameter GradAmpS[]; 
double parameter GradShape[];

int parameter 
{
  relations backbone;
}GradShapeSize;

PV_PULSE_LIST parameter
{
  display_name "Excitation Pulse Shape";
  relations    ExcPulse1EnumRel;
}ExcPulse1Enum;


PVM_RF_PULSE parameter
{
  display_name "Excitation Pulse";
  relations    ExcPulse1Rel;
}ExcPulse1;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "RF Pulse Amplitude";
  relations ExcPulse1AmplRel;
}ExcPulse1Ampl;

double parameter
{
  relations backbone;
}ExcPulse1Shape[];

int parameter
{
  display_name "Projections";
  short_description "Total number of projections.";
  format "%d";
  relations backbone;
}NPro;

double parameter
{
  display_name "Polar Undersampling";
  format "%.2f";
  relations ProUnderRel;
}ProUndersampling;

YesNo parameter
{
  display_name "Minimize Undersampling";
  short_description "Use minimum undersampling factor.";
  relations backbone;
} MinimumUnderSampling;

YesNo parameter
{
  display_name "Minimize TE";
  relations EchoTimeRel;
}YesNoMinEchoTime;

YesNo parameter
{
  display_name "Gradient Synchronization";
  relations backbone;
}GradSync;

YesNo parameter
{
  display_name "Online Reconstruction";
  short_description "Switch off to enable larger matrices.";
  relations backbone;
} RecoOnline;

YesNo parameter 
{
  display_name "B0 Correction";
}B0Corr;

double parameter
{
  display_name "Gradient Ramp Time.";
  relations backbone;
  units "ms";
  format "%.3f";
}RampTime;

YesNo parameter
{
  display_name "Ramp Sampling Compensation";
  short_description "Compensation of resolution loss caused by ramp sampling.";
  relations backbone;
}RampCompYN;

YesNo parameter
{
  display_name "Variable Ramp Time";
  short_description "Allows to change the gradient ramp time based on the slew rate.";
  relations backbone;
}VarRampTimeYN;

double parameter
{
  display_name "Effective Slew Rate";
  relations SlewRateRel;
  units "%";
  format "%f";
}EffSlewRate;

double parameter
{
  display_name "Spoiling";
  short_description "Readout gradient spoiling after FID acquisition.";
  units "{cycles}/{pixel}";
  format "%.1f";
  minimum 0.0;
  maximum 100.0;
  relations backbone;
} Spoiling;

double parameter SpoilDur;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/


