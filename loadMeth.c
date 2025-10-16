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
* ---- GA one-time defaults (persist across sessions) ----
   We explicitly test BOTH "no value yet" AND invalid ranges.
   This avoids the 2/1/0/1 garbage you saw.
*/
{
  int hv;

  hv = ParxRelsParHasValue("GA_Mode");
  if (hv == No || GA_Mode < GA_Traj_UTE3D || GA_Mode > GA_Traj_LinZ_GA) {
    GA_Mode = GA_Traj_Kronecker;
    DB_MSG(("GA default: GA_Mode := GA_Traj_Kronecker"));
  }

  hv = ParxRelsParHasValue("GA_UseFibonacci");
  if (hv == No || (GA_UseFibonacci != Yes && GA_UseFibonacci != No)) {
    GA_UseFibonacci = No;
    DB_MSG(("GA default: GA_UseFibonacci := No"));
  }

  hv = ParxRelsParHasValue("GA_NSpokesReq");
  if (hv == No || GA_NSpokesReq < 1) {
    GA_NSpokesReq = 10000;
    DB_MSG(("GA default: GA_NSpokesReq := %d", GA_NSpokesReq));
  }

  hv = ParxRelsParHasValue("GA_FibIndex");
  if (hv == No || GA_FibIndex < 2 || GA_FibIndex > 45) {
    GA_FibIndex = 19;  /* F(19)=4181 (handy) */
    DB_MSG(("GA default: GA_FibIndex := %d", GA_FibIndex));
  }

  hv = ParxRelsParHasValue("GA_FibValue");
  if (hv == No) {
    GA_FibValue = 0;   /* derived later */
    DB_MSG(("GA default: GA_FibValue := %d", GA_FibValue));
  }

  /* derive once so the editor shows the correct effective spokes on first open */
  GA_UpdateSpokesRel();
}


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


