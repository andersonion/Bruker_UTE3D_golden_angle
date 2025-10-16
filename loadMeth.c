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

void loadMeth(void)
{
  /* One-time defaults (persist). Only set if PV has no stored value. */
  if (ParxRelsParHasValue("GA_Mode")         == No) GA_Mode         = GA_Traj_Kronecker;
  if (ParxRelsParHasValue("GA_UseFibonacci") == No) GA_UseFibonacci = Yes;   /* default ON */
  if (ParxRelsParHasValue("GA_NSpokesReq")   == No) GA_NSpokesReq   = 10000;
  if (ParxRelsParHasValue("GA_FibIndex")     == No) GA_FibIndex     = 17;    /* F(19)=4181 */
  if (ParxRelsParHasValue("GA_FibValue")     == No) GA_FibValue     = 0;

  /* derive once so initial open shows consistent values */
  GA_UpdateSpokesRel();
}


/* ***************************************************************/
/*		E N D   O F   F I L E				 */
/* ***************************************************************/


