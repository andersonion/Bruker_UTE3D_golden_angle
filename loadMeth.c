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
  (void)className;

  /* ---- Apply our two defaults once per protocol ---- */
  if (ParxRelsParHasValue("GA_DefaultsApplied") == No || GA_DefaultsApplied != Yes) {
    GA_Mode          = GA_Traj_Kronecker;   /* default */
    GA_UseFibonacci  = Yes;                 /* default ON */

    /* seed other knobs sensibly only if unset */
    if (ParxRelsParHasValue("GA_NSpokesReq") == No || GA_NSpokesReq < 1) GA_NSpokesReq = 10000;
    if (ParxRelsParHasValue("GA_FibIndex")   == No || GA_FibIndex   < 2 || GA_FibIndex > 45) GA_FibIndex = 19;
    if (ParxRelsParHasValue("GA_FibValue")   == No) GA_FibValue = 0;

    GA_DefaultsApplied = Yes;               /* <-- persists with the protocol */
  }

  /* Derive dependents so the editor shows correct values */
  GA_UpdateSpokesRel();

  /* Mirror current stored values back to widgets so the card displays them */
  GA_Mode         = GA_Mode;
  GA_UseFibonacci = GA_UseFibonacci;
  GA_FibIndex     = GA_FibIndex;
  GA_NSpokesReq   = GA_NSpokesReq;

  backbone();
}
);  /* ensure any dependent arrays are up-to-date in the UI */
}

/* ***************************************************************/
/*		E N D   O F   F I L E				 */
/* ***************************************************************/


