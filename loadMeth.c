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


	/* Only seed if we've never done it on this node */
	if (ParxRelsParHasValue("GA_DefaultsApplied") == No || GA_DefaultsApplied != Yes) {
	
		/* seed MODE + FIB toggle ONCE */
		if (ParxRelsParHasValue("GA_Mode") == No ||
			GA_Mode < GA_Traj_UTE3D || GA_Mode > GA_Traj_LinZ_GA)
			GA_Mode = GA_Traj_Kronecker;
	
		if (ParxRelsParHasValue("GA_UseFibonacci") == No ||
			(GA_UseFibonacci != Yes && GA_UseFibonacci != No))
			GA_UseFibonacci = Yes;
	
		/* seed the rest ONLY if unset/invalid */
		if (ParxRelsParHasValue("GA_NSpokesReq") == No || GA_NSpokesReq < 1)
			GA_NSpokesReq = 10000;
	
		if (ParxRelsParHasValue("GA_FibIndex") == No || GA_FibIndex < 2 || GA_FibIndex > 45)
			GA_FibIndex = 17;  /* F(19)=4181 */
	
		if (ParxRelsParHasValue("GA_FibValue") == No)
			GA_FibValue = 0;   /* derived */
	
		GA_DefaultsApplied = Yes;  /* <- write the “done once” flag so we don’t reseed */
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
