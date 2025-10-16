#include "method.h"
#include "relProtos.h"
#include <string.h>

void loadMeth(const char *className)
{
  /* Only act for the method editor; do nothing for pdata/reco editors. */
  if (!className || strcmp(className, "MethodClass") != 0) {
    DB_MSG(("loadMeth: skip for class=%s", className ? className : "(null)"));
    return;
  }

  DB_MSG(("loadMeth: ENTER  Mode=%d  UseFib=%d  NReq=%d  k=%d  Fk=%d  NEff=%d  DefApplied=%d",
          (int)GA_Mode, (int)GA_UseFibonacci, GA_NSpokesReq, GA_FibIndex, GA_FibValue, GA_NSpokesEff, (int)GA_DefaultsApplied));

  /* ---- ALWAYS hard-wire online recon OFF in the method editor ---- */
  RecoOnline = No;

  /* ---- Apply GA defaults exactly once on this node ---- */
  if (ParxRelsParHasValue("GA_DefaultsApplied") == No || GA_DefaultsApplied != Yes) {
      /* Unconditional seeds for the two primary controls (do NOT guard these) */
      GA_Mode         = GA_Traj_Kronecker;   /* default: Kronecker (recommended) */
      GA_UseFibonacci = Yes;                 /* default: ON */

      /* Sanity for the rest */
      if (GA_NSpokesReq < 1)                   GA_NSpokesReq = 10000;
      if (GA_FibIndex   < 2 || GA_FibIndex>45) GA_FibIndex   = 19;   /* F(19)=4181 */
      if (GA_FibValue   < 0)                   GA_FibValue   = 0;    /* derived placeholder */

      GA_DefaultsApplied = Yes;  /* mark done so we never reseed this node again */
  }

  /* Derive dependents (and keep everything coherent) */
  GA_UpdateSpokesRel();

  /* Mirror stored values back into widgets so the card shows reality on reopen */
  GA_Mode         = GA_Mode;
  GA_UseFibonacci = GA_UseFibonacci;
  GA_FibIndex     = GA_FibIndex;
  GA_NSpokesReq   = GA_NSpokesReq;

  backbone();

  /* If you added GA_RefreshUI + GA_RefreshUIRel, you may keep this: */
  /* GA_RefreshUI = (GA_RefreshUI == Yes) ? No : Yes; */
  /* GA_RefreshUIRel(); */

  DB_MSG(("loadMeth: EXIT   Mode=%d  UseFib=%d  NReq=%d  k=%d  Fk=%d  NEff=%d  DefApplied=%d",
          (int)GA_Mode, (int)GA_UseFibonacci, GA_NSpokesReq, GA_FibIndex, GA_FibValue, GA_NSpokesEff, (int)GA_DefaultsApplied));
}
