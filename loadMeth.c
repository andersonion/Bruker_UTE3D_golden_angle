/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/UTE3D/loadMeth.c,v $
 *
 * Copyright (c) 2009-2010
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: loadMeth.c,v 1.3 2011/05/04 10:54:35 wemch Exp $
 *
 ****************************************************************/

static const char resid[] = "$Id: loadMeth.c,v 1.3 2011/05/04 10:54:35 wemch Exp $ (C) 2009-2010 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

#include "relProtos.h"
#include "method.h"


void loadMeth(const char *className)
{
  DB_MSG(( "-->UTE3D:loadMeth( %s )", className ));
/* ---- GA one-time defaults (persist across sessions) ----
   IMPORTANT: ParxRelsParHasValue returns Yes/No, so test '== No'
*/
if (ParxRelsParHasValue("GA_Mode")         == No) GA_Mode         = GA_Traj_Kronecker;
if (ParxRelsParHasValue("GA_UseFibonacci") == No) GA_UseFibonacci = No;
if (ParxRelsParHasValue("GA_NSpokesReq")   == No) GA_NSpokesReq   = 10000;
if (ParxRelsParHasValue("GA_FibIndex")     == No) GA_FibIndex     = 19;   /* default F(19)=4181 */
if (ParxRelsParHasValue("GA_FibValue")     == No) GA_FibValue     = 0;

/* Optionally derive once so the UI looks right on first open */
#ifdef GA_UpdateSpokesRel
GA_UpdateSpokesRel();   /* requires relProtos.h; otherwise omit and let InitMeth derive */
#endif



  if (0 != className)
  {
      if (0 == strcmp( className, "MethodClass"))
      {
          initMeth();
      }
      else if (0 == strcmp(className, "MethodRecoGroup"))
      {
          DB_MSG(("...responding to loadMeth call for MethodRecoGroup."));
          SetRecoParam();
      }
  }
  else
  {
    DB_MSG(( "...ignoring loadMeth call - I don't know this class" ));
  }

  DB_MSG(( "<--UTE3D:loadMeth( %s )", className ));
}

/* ***************************************************************/
/*		E N D   O F   F I L E				 */
/* ***************************************************************/


