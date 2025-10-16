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


#include "method.h"


void loadMeth(const char *className)
{
  DB_MSG(( "-->UTE3D:loadMeth( %s )", className ));
/* ---- GA one-time defaults (persist across sessions) ---- */
if (!ParxRelsParHasValue("GA_Mode"))         GA_Mode         = GA_Traj_Kronecker;
if (!ParxRelsParHasValue("GA_UseFibonacci")) GA_UseFibonacci = No;
if (!ParxRelsParHasValue("GA_NSpokesReq"))   GA_NSpokesReq   = 10000;
if (!ParxRelsParHasValue("GA_FibIndex"))     GA_FibIndex     = 19;   /* F(19)=4181 */
if (!ParxRelsParHasValue("GA_FibValue"))     GA_FibValue     = 0;
/* Optional: default for the ordering toggle if you added it */
if (ParxRelsParHasValue("GA_GradFriendly") == No) {
    /* nothing — leave untouched if you didn’t declare it */
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


