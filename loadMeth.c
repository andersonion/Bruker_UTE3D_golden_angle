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
  /* no GA defaults here; defaults are handled once in initMeth via GA_DefaultsApplied */
  /* Ensure derived values are up-to-date the first time the editor appears */
	GA_UpdateSpokesRel();
	backbone();

}


/* ***************************************************************/
/*		E N D   O F   F I L E				 */
/* ***************************************************************/


