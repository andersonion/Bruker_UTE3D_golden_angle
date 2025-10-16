#include "method.h"
#include "relProtos.h"
#include <string.h>

void loadMeth(const char *className)
{
  /* ---- Only act when the method editor loads. Do nothing for pdata/reco. ---- */
  if (!className || strcmp(className, "MethodClass") != 0) {
    DB_MSG(("loadMeth: skip for class=%s", className ? className : "(null)"));
    return;
  }

  DB_MSG(("loadMeth: ENTER  Mode=%d  UseFib=%d  NReq=%d  k=%d  Fk=%d  NEff=%d  DefApplied=%d",
          (int)GA_Mode, (int)GA_UseFibonacci, GA_NSpokesReq, GA_FibIndex, GA_FibValue, GA_NSpokesEff, (int)GA_DefaultsApplied));

  /* ---- Apply our defaults exactly once on this node ---- */
  if (ParxRelsParHasValue("GA_DefaultsApplied") == No || GA_DefaultsApplied != Yes) {
      /* Unconditional seeds for the two primary controls */
      GA_Mode         = GA_Traj_Kronecker;   /* default trajectory */
      GA_UseFibonacci = Yes;                 /* default ON */

      /* Seed the rest with sanity checks */
      if (GA_NSpokesReq < 1)                   GA_NSpokesReq = 28777;
      if (GA_FibIndex   < 2 || GA_FibIndex>45) GA_FibIndex   = 17;
      if (GA_FibValue   < 0)                   GA_FibValue   = 0;

      GA_DefaultsApplied = Yes;               /* mark as done so we never reseed */
  }

  /* Derive dependents */
  GA_UpdateSpokesRel();

  /* Optional mirrors to force widgets to show stored values */
  GA_Mode         = GA_Mode;
  GA_UseFibonacci = GA_UseFibonacci;
  GA_FibIndex     = GA_FibIndex;
  GA_NSpokesReq   = GA_NSpokesReq;

  backbone();

  /* If you added GA_RefreshUI + GA_RefreshUIRel, keep this flip to force redraw */
  /* GA_RefreshUI = (GA_RefreshUI == Yes) ? No : Yes; */
  /* GA_RefreshUIRel(); */

  DB_MSG(("loadMeth: EXIT   Mode=%d  UseFib=%d  NReq=%d  k=%d  Fk=%d  NEff=%d  DefApplied=%d",
          (int)GA_Mode, (int)GA_UseFibonacci, GA_NSpokesReq, GA_FibIndex, GA_FibValue, GA_NSpokesEff, (int)GA_DefaultsApplied));
}
