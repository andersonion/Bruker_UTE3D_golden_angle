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

  DB_MSG(("loadMeth: entered"));

  /* ---- GA one-time defaults (persist across sessions) ----
     Compare ParxRelsParHasValue(...) explicitly to No; also clamp ranges.
  */
  {
    int hv;

    hv = ParxRelsParHasValue("GA_Mode");
    DB_MSG(("loadMeth: Has GA_Mode? %d (curr=%d)", hv, (int)GA_Mode));
    if (hv == No || GA_Mode < GA_Traj_UTE3D || GA_Mode > GA_Traj_LinZ_GA) {
      GA_Mode = GA_Traj_Kronecker;
      DB_MSG(("loadMeth: defaulted GA_Mode -> GA_Traj_Kronecker"));
    }

    hv = ParxRelsParHasValue("GA_UseFibonacci");
    DB_MSG(("loadMeth: Has GA_UseFibonacci? %d (curr=%d)", hv, (int)GA_UseFibonacci));
    if (hv == No || (GA_UseFibonacci != Yes && GA_UseFibonacci != No)) {
      GA_UseFibonacci = No;
      DB_MSG(("loadMeth: defaulted GA_UseFibonacci -> No"));
    }

    hv = ParxRelsParHasValue("GA_NSpokesReq");
    DB_MSG(("loadMeth: Has GA_NSpokesReq? %d (curr=%d)", hv, GA_NSpokesReq));
    if (hv == No || GA_NSpokesReq < 1) {
      GA_NSpokesReq = 10000;
      DB_MSG(("loadMeth: defaulted GA_NSpokesReq -> %d", GA_NSpokesReq));
    }

    hv = ParxRelsParHasValue("GA_FibIndex");
    DB_MSG(("loadMeth: Has GA_FibIndex? %d (curr=%d)", hv, GA_FibIndex));
    if (hv == No || GA_FibIndex < 2 || GA_FibIndex > 45) {
      GA_FibIndex = 19;  /* default F(19)=4181 */
      DB_MSG(("loadMeth: defaulted GA_FibIndex -> %d", GA_FibIndex));
    }

    hv = ParxRelsParHasValue("GA_FibValue");
    DB_MSG(("loadMeth: Has GA_FibValue? %d (curr=%d)", hv, GA_FibValue));
    if (hv == No) {
      GA_FibValue = 0;   /* derived later */
      DB_MSG(("loadMeth: defaulted GA_FibValue -> %d", GA_FibValue));
    }
  }

  /* Derive once so the editor shows consistent values on first open */
  GA_UpdateSpokesRel();

  DB_MSG(("loadMeth: exit with Mode=%d, UseFib=%d, NReq=%d, k=%d, Fk=%d, NEff=%d",
          (int)GA_Mode, (int)GA_UseFibonacci, GA_NSpokesReq, GA_FibIndex, GA_FibValue, GA_NSpokesEff));
}


/* ***************************************************************/
/*		E N D   O F   F I L E				 */
/* ***************************************************************/


