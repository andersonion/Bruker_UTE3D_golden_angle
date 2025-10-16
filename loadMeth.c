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

#define DEBUG		1
#define DB_MODULE	1
#define DB_LINE_NR	1
/* loadMeth.c — golden_angle_UTE3D */

#include "method.h"
#include "relProtos.h"

void loadMeth(const char *className)
{
  (void)className;
  DB_MSG(("loadMeth: ENTER  Mode=%d  UseFib=%d  NReq=%d  k=%d  Fk=%d  NEff=%d  DefApplied=%d",
          (int)GA_Mode, (int)GA_UseFibonacci, GA_NSpokesReq, GA_FibIndex, GA_FibValue, GA_NSpokesEff, (int)GA_DefaultsApplied));


	/* ---- Apply our defaults exactly once on this node ---- */
	if (ParxRelsParHasValue("GA_DefaultsApplied") == No || GA_DefaultsApplied != Yes) {
		/* Unconditional seeds (do NOT guard these) */
		GA_Mode         = GA_Traj_Kronecker;   /* default trajectory */
		GA_UseFibonacci = Yes;                 /* default ON */
	
		/* Seed the rest with sanity checks (these guards are OK) */
		if (GA_NSpokesReq < 1)                   GA_NSpokesReq = 28733; // Mimicking our defaults on regular UTE3D
		if (GA_FibIndex   < 2 || GA_FibIndex>17) GA_FibIndex   = 19;
		/* GA_FibValue is derived; set zero to be safe */
		if (GA_FibValue < 0)                     GA_FibValue   = 0;
	
		GA_DefaultsApplied = Yes;               /* mark as done so we never reseed */
	}



  /* derive dependents so the editor shows consistent values */
  GA_UpdateSpokesRel();

  /* mirror stored values back into widgets so the card displays them on revisit */
  GA_Mode         = GA_Mode;
  GA_UseFibonacci = GA_UseFibonacci;
  GA_FibIndex     = GA_FibIndex;
  GA_NSpokesReq   = GA_NSpokesReq;

  /* ensure dependent arrays/tables are refreshed */
  /* Force the editor to display the currently stored values */
GA_RefreshUI = (GA_RefreshUI == Yes) ? No : Yes;   /* flip the hidden knob */
GA_RefreshUIRel();

  backbone();

      DB_MSG(("loadMeth: EXIT   Mode=%d  UseFib=%d  NReq=%d  k=%d  Fk=%d  NEff=%d  DefApplied=%d",
          (int)GA_Mode, (int)GA_UseFibonacci, GA_NSpokesReq, GA_FibIndex, GA_FibValue, GA_NSpokesEff, (int)GA_DefaultsApplied));

}
