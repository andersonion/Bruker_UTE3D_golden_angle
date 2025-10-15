/* ***************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/UTE3D/RecoRelations.c,v $
 *
 * Copyright (c) 2009 - 2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: RecoRelations.c,v 1.21.2.1 2014/08/19 14:12:14 sako Exp $
 *
 * ***************************************************************/

const char * methods_ute3d_reco_relations_id = "$Id: RecoRelations.c,v 1.21.2.1 2014/08/19 14:12:14 sako Exp $ (C) 2009 - 2011 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0



#include "method.h"


void SetRecoParam( void )
{
  DB_MSG(("-->SetRecoParam"));

  /* set up user reco */
  ATB_InitUserModeReco(
    ACQ_dim,
    ACQ_dim,
    PVM_EncMatrix,
    PVM_Matrix,
    PVM_AntiAlias,
    NULL,
    NI,
    ACQ_obj_order,
    1,
    NULL,
    NULL,
    NULL,
    PVM_EncNReceivers,
    PVM_EncChanScaling,
    0,
    0);

  /* regridding */
  char trajFile[PATH_MAX];
  PvOvlUtilGetExpnoPath(trajFile, PATH_MAX, "traj");
  const int readP = PVM_EncMatrix[0]/2 + RampPoints;
  ATB_SetRecoRegridN(
    readP,
    NPro,
    RecoRegridNTrajRadial,
    trajFile,
    RecoRegridNDCPre,
    NULL);
    
  /* geometry */
  
  for (int i = 0; i < PTB_GetSpatDim(); i++)
  {
    ATB_SetRecoRotate(
      NULL,
      PVM_Fov[i] * PVM_AntiAlias[i],
      NSLICES,     
      1,
      i);      
            
    ATB_SetRecoPhaseCorr(50.0, 0.0, i);
  }
 
  ATB_SetRecoTranspositionFromLoops(PtrType3x3 ACQ_grad_matrix[0],
				    NSLICES,
				    1,
				    NI,
				    ACQ_obj_order);

  /* NR */
  GO_reco_each_nr = PVM_NRepetitions == 1 ? No : Yes;

  DB_MSG(("<--SetRecoParam"));
}

void RecoDerive(void)
{
  DB_MSG(("-->RecoDerive"));

  if (RecoUserUpdate == No)
  {
    DB_MSG(("<--RecoDerive: no update"));
    return;
  }

  /* trajectory measurement: use different reco */
  if (ATB_TrajSetupReco() == Yes)
  {
    DB_MSG(("<--RecoDerive: trajectory measurement"));
    return;
  }
  
  /* create network */
  
  if (ACQ_scan_type != Setup_Experiment)
  {       
    /*** scan experiment or off-line reco ***/
  
    /* standard settings for reconstruction */
    if(RecoPrototype == No)
      SetRecoParam();

    /* provide interleave weights */
    PARX_change_dims("GradAmpR", NPro);
    PARX_change_dims("GradAmpP", NPro);
    PARX_change_dims("GradAmpS", NPro);
    SetProj3D(GradAmpR,
	      GradAmpP,
	      GradAmpS,
	      1, 1, 1);

    /* get trajectory */      
    int nPre = 0;
    int nUsed = PVM_TrajRecoMode == Traj_Measured ? PVM_EncMatrix[0]/2 + RampPoints : 0;
    int nPost = 0;
    char b0File[PATH_MAX]; 
    PvOvlUtilGetExpnoPath(b0File, PATH_MAX, "b0");
   
    if(PVM_TrajRecoMode == Traj_Measured) 
    {
      if (PVM_TrajUpToDate == No)
      {
	UT_ReportError("Measured trajectory is not up-to-date");      
      }
      STB_TrajCreateTrajectory(RecoRegridNTrajFile, b0File, &nPre, &nUsed, &nPost, 0);
    }
    else
    {
      STB_TrajCreateTrajectory(RecoRegridNTrajFile, NULL, &nPre, &nUsed, &nPost, 0);
    }
    
    /* set up default network */
    ParxRelsParRelations("RecoUserUpdate", Yes);

    /* B0 correction */
    if (PVM_TrajRecoMode == Traj_Measured && B0Corr == Yes)
    {
      ATB_TrajInsertB0Correction(b0File, nUsed, RecoRegridNDim);
    }
  }  
  else
  {
    /*** setup experiment ***/
  
    ATB_SetRecoRegridNOff();
      
    const int size[3] = {ACQ_size[0]/2, 1, 1};
    
    for (int i = 0; i < PTB_GetSpatDim(); i++)
    {
      RECO_size[i]     = size[i];
      RECO_inp_size[i] = size[i];
      RECO_ft_size[i]  = size[i];
    }
    
    /* set up default network */
    ParxRelsParRelations("RecoUserUpdate", Yes);
  }
  
  DB_MSG(("<--RecoDerive"));
}

/* Check memory requirements of 3D radial regridding reconstruction.

  The following steps are performed:
  - Determine maximum available memory.
  - Determine optimum oversampling factor for given parameters.
  - Calculate reduction factor, if required memory is too large.

  Calculations are based on:
  MEM = [9 * nPro * nSamp + 2 * (1 + over^3) * matrix^3] * nCoils * sizeof(double)

  Example call:
  ProUnderSampling *= RecoOptimiseMemory(PVM_Matrix[0], NPro, PVM_EncMatrix[0], OverSampling, &RecoRegridNOver);

  @param matrix  size of reconstructed image (isotropic)
  @param nPro    number of projections
  @param nSamp   number of complex samples per projection
  @param nCoils  number of coils
  @param over    oversampling factor used for regridding (may be modified within range [1.25, 2.0])

  @return factor by which the number of incoming data points must be reduced
          - 0 if matrix is too large
          - otherwise >= 1
*/
double RecoOptimiseMemory(
  int matrix,
  int nPro,
  int nSamp,
  int nCoils,
  double *over)
{
  DB_MSG(("-->RecoOptimiseMemory"));
  DB_MSG(("nCoils = %d", nCoils));

  const double sizePerPoint = (double)sizeof(double);                              // size of a real data point in double precision
  const double nIn          = 9.0 * (double)nPro * (double)nSamp * (double)nCoils; // number of complex points required for input data
  const double nOut         = pow((double)matrix, 3.0) * (double)nCoils;           // number of complex output data points

  int64_t physRam=0;
  int ret = UT_AvailableMemory(&physRam);
  if(ret == -1)
  {
    UT_ReportError("Reco memory optimisation failed: information about physical memory is missing.");
  }
  
  DB_MSG(("Memory available = %f GB", physRam/(1024.0*1024.0*1024)));

  double maxMem = 0.8*physRam;

  // oversampling factor
  *over = (maxMem / sizePerPoint - nIn) / (2.0 * nOut) - 1.0;
  *over = MAX_OF(*over, 1.0);
  *over = pow(*over, 1.0 / 3.0);
  *over = MAX_OF(*over, 1.25);
  *over = MIN_OF(*over, 2.0);
  DB_MSG(("oversampling factor = %f", *over));

  // reduction factor
  double factor = (maxMem / sizePerPoint - 2.0 * (1.0 + pow(*over, 3.0)) * nOut) / nIn;
  double reductionFactor;
  if (factor > 0.0 && factor < 1.0)
  {
    reductionFactor = 1.0 / factor;
  }
  else
  {
    factor = MAX_OF(factor, 0.0);
    factor = MIN_OF(factor, 1.0);
    reductionFactor = factor;
  }
  DB_MSG(("factor = %f", factor));
  DB_MSG(("reduction factor = %f", reductionFactor));

  // required memory
#if DEBUG
  double mem = (nIn * factor + 2.0 * (1.0 + pow(*over, 3.0)) * nOut) * sizePerPoint;
  DB_MSG(("memory required = %lu", (unsigned long)mem));
#endif

  DB_MSG(("<--RecoOptimiseMemory"));
  return reductionFactor;
}
