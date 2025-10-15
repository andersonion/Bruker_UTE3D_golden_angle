/* ***************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/UTE3D/parsRelations.c,v $
 *
 * Copyright (c) 2009
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: parsRelations.c,v 1.20 2013/06/14 14:02:41 sako Exp $
 *
 * ***************************************************************/


static const char resid[] = "$Id: parsRelations.c,v 1.20 2013/06/14 14:02:41 sako Exp $ (C) 2009 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

#include "method.h"



/*-----------------------------------------------
 * Range checking and default relation routines
 *----------------------------------------------*/


/*===============================================================
 * ExcPulse1EnumRel
 * Relation of ExcPulse1Enum (a dynamic enmueration parameter which
 * allows to select one of the existing  pulses)
 *===============================================================*/

void ExcPulse1EnumRel(void)
{
  DB_MSG(("-->ExcPulse1EnumRel"));

  UT_SetRequest("ExcPulse1Enum");
  backbone();
  
  DB_MSG(("<--ExcPulse1EnumRel"));                                       
}

/*===============================================================
 * ExcPulse1AmplRel
 * Relation of ExcPulseAmpl 
 * This parameter is used in the setup parameter card to adjust
 * the RF pulse amplitude manually
 *===============================================================*/

void ExcPulse1AmplRel(void)
{
  DB_MSG(("-->ExcPulse1AmplRel"));
  UT_SetRequest("ExcPulse1Ampl");
  HandleRFPulseAmplitude();
  DB_MSG(("-->ExcPulse1AmplRel"));
}

void HandleRFPulseAmplitude(void)
{
  DB_MSG(("-->HandleRFPulseAmplitude"));

  STB_UpdateRFShapeAmplitude("ExcPulse1Ampl",No);
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]");

  DB_MSG(("<--HandleRFPulseAmplitude"));
}


/* ===================================================================
 * Relation of ExcPulse
 * 
 * All pulses of type PVM_RF_PULSE must have relations like this.
 * However, if you clone this funtion for a different pulse parameter
 * remember to replace the param name in the call to UT_SetRequest!
 *
 * IMPORTANT: this function should not be invoked in the backbone!
 ====================================================================*/

void ExcPulse1Rel(void)
{
  DB_MSG(("-->ExcPulse1Rel"));
  
  /*
   * Tell the request handling system that the parameter
   * ExcPulse has been edited 
   */
  
  UT_SetRequest("ExcPulse1");
  
  /* Check the values of ExcPulse */
  
  ExcPulse1Range();
  
  /* 
   * call the backbone; further handling will take place there
   * (by means of STB_UpdateRFPulse)  
   */
  
  backbone();
  
  DB_MSG(("<--ExcPulse1Rel"));
}

void ExcPulse1Range(void)
{
  DB_MSG(("-->ExcPulse1Range"));
  
  // range checker fields to be controlled may be
  // .Length  
  // .Bandwidth
  // .Flipangle
  // .Calculated
  // .Sharpness
  // .Flatness   
 double dval=ExcPulse1.Flipangle;
  
 ExcPulse1.Flipangle = MIN_OF(90.0,MAX_OF(dval,1.0));

 DB_MSG(("<--ExcPulseRange")); 
}


void EffSWhRange(void)
{
  DB_MSG(("-->EffSWhRange"));

  if(!ParxRelsParHasValue("PVM_EffSWh"))
  {
    PVM_EffSWh=100000;
  }
  else
  {
    double dval;
    /* constrain */
    dval = PVM_EffSWh;
    PVM_EffSWh=MAX_OF(MIN_OF(1000000.0,dval),2000.0);
  }

  DB_MSG(("<--EffSWhRange"));
}

void EffSWhRel(void)
{
  DB_MSG(("-->EffSWhRel"));

  EffSWhRange();
  backbone();

  DB_MSG(("<--EffSWhRel"));
}


/*--------------------------------------------------------
 *
 * single parameter range checkers and relations
 *
 *-------------------------------------------------------*/


void RepetitionTimeRange(void)
{
  
  DB_MSG(("-->RepetitionTimeRange"));

  if(ParxRelsParHasValue("PVM_RepetitionTime")==No)
  {
    if (CFG_InstrumentType() == Avance_II)
      PVM_RepetitionTime = 5.5;
    else
      PVM_RepetitionTime = 4.0;   
  }
  else
  {
    PVM_RepetitionTime = MAX_OF(1e-3,PVM_RepetitionTime);
  }

  
  DB_MSG(("<--RepetitionTimeRange"));
  return;
}

void RepetitionTimeRel(void)
{
  DB_MSG(("-->RepetitionTimeRel"));

  RepetitionTimeRange();
  backbone();

  DB_MSG(("<--RepetitionTimeRel"));
  return;
}

void AveragesRange(void)
{
  
  DB_MSG(("-->AveragesRange"));

  if(ParxRelsParHasValue("PVM_NAverages")==No)
  {
    PVM_NAverages = 1;
  }
  else
  {
    PVM_NAverages = MAX_OF(1,PVM_NAverages);
  }
  
  DB_MSG(("<--AveragesRange"));
  return;
}

void AveragesRel(void)
{
  DB_MSG(("-->AveragesRel"));

  AveragesRange();
  backbone();

  DB_MSG(("<--AveragesRel"));
  return;
}



void EchoTimeRange(void)
{
  
  DB_MSG(("-->EchoTimeRange"));

  if(YesNoMinEchoTime == Yes)
  {
    PVM_EchoTime = PVM_MinEchoTime;
    ParxRelsMakeNonEditable("PVM_EchoTime");
  }
  else
  {
    PVM_EchoTime = MAX_OF(PVM_MinEchoTime,PVM_EchoTime);
    ParxRelsMakeEditable("PVM_EchoTime");
  }
  
  DB_MSG(("<--EchoTimeRange"));
  return;
}

void EchoTimeRel(void)
{
  DB_MSG(("-->EchoTimeRel"));

  EchoTimeRange();
  backbone();

  DB_MSG(("<--EchoTimeRel"));
  return;
}

/*==============================================================
 * method specific range checkers and relations
 *==============================================================*/


void ProUnderRel(void)
{
  DB_MSG(("-->ProUnderRel"));

  ProUnderRange();
  backbone();

  DB_MSG(("<--ProUnderRel"));
  return;
}

void ProUnderRange(void)
{
  DB_MSG(("-->ProUnderRange"));

  if(!ParxRelsParHasValue("ProUndersampling"))
  {
    ProUndersampling = 1.0;
  }
  else
  {
    ProUndersampling = MAX_OF(MIN_OF(10.0,ProUndersampling),1.0);
  }

  DB_MSG(("<--ProUnderRange"));
  return;
}

/* relations for var. ramp time */

void SlewRateRel(void)
{
  DB_MSG(("-->SlewRateRel"));

  SlewRateRange();
  backbone();

  DB_MSG(("<--SlewRateRel"));
}

void SlewRateRange(void)
{
  DB_MSG(("-->SlewRateRange"));

  if(ParxRelsParHasValue("EffSlewRate") == 0)
  {
    EffSlewRate = 100.0;
  }
  else
  {
    EffSlewRate = MIN_OF(MAX_OF(10.0,EffSlewRate),100.0);
  }
  
  DB_MSG(("<--SlewRateRange"));
}
