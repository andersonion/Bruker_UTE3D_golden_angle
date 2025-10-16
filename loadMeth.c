/* loadMeth.c — golden_angle_UTE3D */

#include "method.h"
#include "relProtos.h"
#include <string.h>

void loadMeth(const char *className)
{
  /* Log the class for visibility, but DO NOT return early. */
  DB_MSG(("loadMeth: class=%s", className ? className : "(null)"));

  DB_MSG(("loadMeth: ENTER  Mode=%d  UseFib=%d  NReq=%d  k=%d  Fk=%d  NEff=%d  DefApplied=%d",
          (int)GA_Mode, (int)GA_UseFibonacci, GA_NSpokesReq, GA_FibIndex,
          GA_FibValue, GA_NSpokesEff, (int)GA_DefaultsApplied));

  /* Always hard-wire online recon OFF in the method context */
  RecoOnline = No;

  /* ---- Apply defaults exactly once per node ---- */
  if (ParxRelsParHasValue("GA_DefaultsApplied") == No || GA_DefaultsApplied != Yes) {
      GA_Mode         = GA_Traj_Kronecker;   /* default trajectory */
      GA_UseFibonacci = Yes;                 /* default ON */
      if (GA_NSpokesReq < 1)                   GA_NSpokesReq = 10000;
      if (GA_FibIndex   < 2 || GA_FibIndex>45) GA_FibIndex   = 19;   /* F(19)=4181 */
      if (GA_FibValue   < 0)                   GA_FibValue   = 0;
      GA_DefaultsApplied = Yes;               /* mark done; never reseed this node */
      DB_MSG(("loadMeth: seeded defaults once"));
  }

  /* ---- Self-heal: if a reopen shows type defaults, restore our intended defaults ---- */
  if ((int)GA_Mode == 0 && (int)GA_UseFibonacci == 0) {
      DB_MSG(("loadMeth: self-heal Mode/UseFib (saw type defaults 0/0)"));
      GA_Mode         = GA_Traj_Kronecker;
      GA_UseFibonacci = Yes;
  }

  /* Derive dependents */
  GA_UpdateSpokesRel();

  /* Mirror stored values back to widgets so the card shows reality on revisit */
  GA_Mode         = GA_Mode;
  GA_UseFibonacci = GA_UseFibonacci;
  GA_FibIndex     = GA_FibIndex;
  GA_NSpokesReq   = GA_NSpokesReq;

  backbone();

  DB_MSG(("loadMeth: EXIT   Mode=%d  UseFib=%d  NReq=%d  k=%d  Fk=%d  NEff=%d  DefApplied=%d",
          (int)GA_Mode, (int)GA_UseFibonacci, GA_NSpokesReq, GA_FibIndex,
          GA_FibValue, GA_NSpokesEff, (int)GA_DefaultsApplied));
}
