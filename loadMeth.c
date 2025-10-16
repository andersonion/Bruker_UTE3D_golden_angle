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
  (void)className;  /* silence unused parameter warning */

  /* ---- GA one-time defaults (persist across sessions) ----
     Note: ParxRelsParHasValue returns Yes/No (not booleans), so compare to No.
     Also clamp to sane ranges if values are uninitialized.
  */
  {
    int hv;

    hv = ParxRelsParHasValue("GA_Mode");
    if (hv == No || GA_Mode < GA_Traj_UTE3D || GA_Mode > GA_Traj_LinZ_GA) {
      GA_Mode = GA_Traj_Kronecker;
    }

    hv = ParxRelsParHasValue("GA_UseFibonacci");
    if (hv == No || (GA_UseFibonacci != Yes && GA_UseFibonacci != No)) {
      GA_UseFibonacci = No;
    }

    hv = ParxRelsParHasValue("GA_NSpokesReq");
    if (hv == No || GA_NSpokesReq < 1) {
      GA_NSpokesReq = 10000;
    }

    hv = ParxRelsParHasValue("GA_FibIndex");
    if (hv == No || GA_FibIndex < 2 || GA_FibIndex > 45) {
      GA_FibIndex = 19;  /* default F(19) = 4181 */
    }

    hv = ParxRelsParHasValue("GA_FibValue");
    if (hv == No) {
      GA_FibValue = 0;   /* derived later */
    }

    /* Derive once so the UI shows consistent values on first open */
    GA_UpdateSpokesRel();
  }
}


/* ***************************************************************/
/*		E N D   O F   F I L E				 */
/* ***************************************************************/


