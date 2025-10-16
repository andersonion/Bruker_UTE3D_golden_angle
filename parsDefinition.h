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

/* ================================================================
   ---- Trajectory Selection ----
   ================================================================ */

/* GA_Mode is now a typed enum parameter (see parsTypes.h) */
GA_TrajMode parameter
{
  display_name "Trajectory Mode";
  short_description "Select projection ordering: packaged UTE3D, Kronecker (recommended), or linear-in-z golden angle.";
  relations backbone;
} GA_Mode;


YesNo parameter
{
  display_name "Force Fibonacci Spoke Count";
  short_description "Round requested spokes up to the nearest Fibonacci number.";
  relations GA_UpdateSpokesRel;
} GA_UseFibonacci;

int parameter
{
  display_name "Requested Number of Spokes";
  short_description "Total number of projection spokes to acquire (all frames combined).";
  format "%d";
  minimum 1;
  maximum 100000000;
  relations GA_UpdateSpokesRel;
} GA_NSpokesReq;


int parameter
{
  display_name "Effective Number of Spokes";
  short_description "Derived number of spokes after applying the Fibonacci constraint.";
  format "%d";
  minimum 1;
  maximum 100000000;
  relations backbone;
} GA_NSpokesEff;


YesNo parameter
{
  display_name "Gradient-Friendly Order";
  short_description "Traverse directions in a near-neighbor order to reduce gradient jumps (minimize slew/eddy).";
  relations backbone;
} GA_GradFriendly;

/* ================================================================
   ---- Fibonacci Target (optional when GA_UseFibonacci==Yes) ----
   ================================================================ */

int parameter
{
  display_name "Fibonacci Index (k)";
  short_description "When 'Force Fibonacci' is Yes, set the target Fibonacci index k so that N_spokes = F(k).";
  format "%d";
  minimum 2;
  maximum 45;        /* F(45) ~ 1,134,903,170: plenty */
  relations GA_UpdateSpokesRel;
} GA_FibIndex;

int parameter
{
  display_name "Fibonacci Value F(k) (derived)";
  short_description "Derived Fibonacci number for the selected index k.";
  format "%d";
  minimum 1;
  maximum 2000000000;
  relations backbone;     /* read-only display; computed in relations/init */
} GA_FibValue;

/* ---- Internal: one-time GA defaults applied? (persistent) ---- */
YesNo parameter
{
  display_name "GA Defaults Applied (internal)";
  short_description "Internal flag so defaults run only once per protocol.";
  relations backbone;
} GA_DefaultsApplied;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/


