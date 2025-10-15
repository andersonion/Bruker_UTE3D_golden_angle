/* ***************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/UTE3D/BaseLevelRelations.c,v $
 *
 * Copyright (c) 2009
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: BaseLevelRelations.c,v 1.24 2013/04/22 13:29:10 sako Exp $
 *
 * ***************************************************************/

static const char resid[] = "$Id: BaseLevelRelations.c,v 1.24 2013/04/22 13:29:10 sako Exp $ (C) 2009 Bruker BioSpin MRI GmbH";


#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0


#if DEBUG
#define PRINTARRAY(file, array, nelem)  \
{\
int ix; \
FILE *dfp; \
if((dfp=fopen(file,"w"))!=NULL) \
{ \
for(ix=0; ix<(nelem); ix++) fprintf(dfp, " %g\n", (double) array[ix]);\
fclose(dfp); \
} \
}
#else
#define PRINTARRAY(file, array, nelem) ;
#endif

#include "method.h"

void SetBaseLevelParam()
{

  DB_MSG(("-->SetBaseLevelParam"));


  SetBasicParameters();
    
  SetFrequencyParameters();
  
  SetGradientParameters();
  
  SetPpgParameters();

  SetInfoParameters();
  
  SetMachineParameters();
  

  /* setting baselevel parameters used by modules */
  ATB_SetFovSatBaseLevel();
  ATB_SetFatSupBaselevel();
  ATB_SetMagTransBaseLevel();

  PrintTimingInfo();

  DB_MSG(("<--SetBaseLevelParam"));
  

}

void SetBasicParameters( void )
{
  int spatDim, specDim;
  int nSlices;


  DB_MSG(("-->SetBasicParameters"));
    
  /* ACQ_dim */

  spatDim = PTB_GetSpatDim();
 
  specDim = PTB_GetSpecDim();
  
  ACQ_dim = spatDim + specDim;
  ParxRelsParRelations("ACQ_dim",Yes);
  
  /* ACQ_dim_desc */  
  ATB_SetAcqDimDesc( specDim, spatDim, NULL );
  
  /* ACQ_size */  
  ATB_SetAcqSize( Spatial, spatDim, PVM_EncMatrix, PVM_AntiAlias, No );
  
  /* setting ACQ_sizes for radial acquisition */
  ACQ_size[0] = PVM_EncMatrix[0] + 2*RampPoints;
  ACQ_size[1] = NPro;
  ACQ_size[2] = 1;

  
  /* NSLICES */  
  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );  
  ATB_SetNSlices( nSlices );
  
  /* NR */  
  ATB_SetNR( PVM_NRepetitions );

  
  /* NI */  
  ATB_SetNI( nSlices );
 
  /* AVERAGING */
  ATB_SetNA( 1 );

  ATB_SetNAE( PVM_NAverages );
   
  /* ACQ_ns */  
  ACQ_ns_list_size = 1;
  ParxRelsParRelations("ACQ_ns_list_size",Yes);

  ACQ_ns_list[0] = 1;
  
  NS = ACQ_ns = ACQ_ns_list[0];
  
  /* NECHOES */
  NECHOES = 1;
   
  /* ACQ_obj_order */
  PARX_change_dims("ACQ_obj_order",NI);
  
  ATB_SetAcqObjOrder( nSlices, PVM_ObjOrderList,1, 1);

  
  /* DS */
  DS = PVM_DummyScans;
  
  ACQ_DS_enabled = Yes;
  
  /* ACQ_user_filter for Frequency Drift Correction*/
  if(PVM_FreqDriftYN == Yes)
  {
    ACQ_user_filter = Yes;
    ParxRelsParRelations("ACQ_user_filter", Yes);
    ACQ_user_filter_mode = Standard;  
    ACQ_user_filter_memory = For_one_scan;
    sprintf(ACQ_user_filter_name,"FreqDriftCorr");
    sprintf(ACQ_user_filter_setup_name[0],"NoOperation");
    ParxRelsParRelations("ACQ_user_filter_mode", Yes);
  }
  else
  {
    ATB_DisableAcqUserFilter();
  }
  
  ATB_SetAcqScanSize( One_scan );

    
  DB_MSG(("<--SetBasicParameters"));
}

void SetFrequencyParameters( void )
{
  int nslices;


  DB_MSG(("-->SetFrequencyParameters"));


  ATB_SetNuc1(PVM_Nucleus1);
   
  sprintf(NUC2,"off");
  sprintf(NUC3,"off");
  sprintf(NUC4,"off");
  sprintf(NUC5,"off");
  sprintf(NUC6,"off");
  sprintf(NUC7,"off");
  sprintf(NUC8,"off");
  
  ATB_SetNucleus(PVM_Nucleus1);
  
   
 
  /* setting of AQ_mod DSPFIRM SW_h */

  ATB_SetDigPars();
    
  
  ACQ_O1_mode = BF_plus_Offset_list;
  ParxRelsParRelations("ACQ_O1_mode",Yes);
  
  ACQ_O2_mode = BF_plus_Offset_list;
  ParxRelsParRelations("ACQ_O2_mode",Yes);
  
  ACQ_O3_mode = BF_plus_Offset_list;
  ParxRelsParRelations("ACQ_O3_mode",Yes);
  
  O1 = 0.0;
  O2 = 0.0;
  O3 = 0.0;
  O4 = 0.0;
  O5 = 0.0;
  O6 = 0.0;
  O7 = 0.0;
  O8 = 0.0;
  
  /* Set BF's to working freuncies on used channels */
  ACQ_BF_enable = No;
  BF1 = PVM_FrqWork[0];
  BF2 = PVM_FrqWork[1];
  /* call relations of BF1 (no need for other BF's) */
  ParxRelsParRelations("BF1", Yes); 

  nslices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  
  ATB_SetAcqO1List( nslices,
                    PVM_ObjOrderList,
                    PVM_SliceOffsetHz );

  
  ATB_SetAcqO1BList( nslices,
                     PVM_ObjOrderList,
                     PVM_ReadOffsetHz);
  
  
  
  DB_MSG(("<--SetFrequencyParameters"));
}


void SetGradientParameters( void )
{
  int spatDim, dim, i;
  double dur1, dur2, dur3;
  double riseT;

  DB_MSG(("-->SetGradientParameters"));

  riseT = CFG_GradientRiseTime();
  
  ATB_SetAcqPhaseFactor( 1 );

  
  spatDim = PTB_GetSpatDim();
  
  dim = PARX_get_dim("ACQ_phase_encoding_mode", 1 );
  
  if( dim != spatDim )
  {
    PARX_change_dims("ACQ_phase_encoding_mode", spatDim );
  }
  
  ACQ_phase_encoding_mode[0] = Read;
  for( i=1; i<spatDim; i++ )
  {
    ACQ_phase_encoding_mode[i] = Linear;
  }

  ParxRelsParRelations("ACQ_phase_encoding_mode",Yes);
  
  dim = PARX_get_dim("ACQ_phase_enc_start", 1 );
  
  if( dim != spatDim )
  {
    PARX_change_dims("ACQ_phase_enc_start", spatDim );
  }
  
  for( i=0; i<spatDim; i++ )
  {
    ACQ_phase_enc_start[i] = -1;
  }
  
  
  ATB_SetAcqGradMatrix( PVM_NSPacks, PVM_SPackArrNSlices,
			PtrType3x3 PVM_SPackArrGradOrient[0],
			PVM_ObjOrderList );
  

  
  
  ACQ_scaling_read  = 1.0;
  ACQ_scaling_phase = 1.0;
  ACQ_scaling_slice = 1.0;
  
  ACQ_rare_factor = 1;
  
  ACQ_grad_str_X = 0.0;
  ACQ_grad_str_Y = 0.0;
  ACQ_grad_str_Z = 0.0;

  
  /*
   *  creation of gradient shapes
   */

  /* time stamps for projection gradient shape */
  dur1 = RampTime;
  dur2 = dur1 + PVM_AcquisitionTime + RampCompTime + SpoilDur;
  dur3 = dur2 + riseT;
  
  GradShapeSize = (int)(dur3/GradRes);
  PARX_change_dims("GradShape",GradShapeSize);  

  
  /* GradShape */
  MRT_MakeRamp(GradShape,
	       GradShapeSize,
	       dur3,
	       0,
	       dur1,
               0.0,
	       1.0,
	       ramp_lin);

  MRT_MakeRamp(GradShape,
	       GradShapeSize,
	       dur3,
	       dur1,
	       dur2,
               1.0,
	       1.0,
	       ramp_lin);

  MRT_MakeRamp(GradShape,
	       GradShapeSize,
	       dur3,
	       dur2,
	       dur3,
               1.0,
	       0.0,
	       ramp_lin);



  /* print shapes for plotting */
  PRINTARRAY("/tmp/UTE3D_GradShape",GradShape, GradShapeSize);
 
  strcpy(GRDPROG, "");


  /* gradient amplitudes */
  ACQ_gradient_amplitude[0] = ReadGrad;     /* g0 */
  ACQ_gradient_amplitude[1] = PhaseGrad;    /* g1 */
  ACQ_gradient_amplitude[2] = SliceGrad;    /* g2 */

  
  DB_MSG(("<--SetGradientParameters"));

}

void SetPpgParameters(void)
{
  double igwT;
  int slices;
  double riseT;

  DB_MSG(("-->SetPpgParameters"));
  
  ACQ_vd_list_size=1;
  PARX_change_dims("ACQ_vd_list",1);
  ACQ_vd_list[0] = 1e-6;
  ParxRelsParRelations("ACQ_vd_list",Yes);
  
  ACQ_vp_list_size=1;
  PARX_change_dims("ACQ_vp_list",1);
  ACQ_vp_list[0] = 1e-6;
  ParxRelsParRelations("ACQ_vp_list",Yes);
  
  
  ATB_SetPulprog("golden_angle_UTE3D.ppg");

  riseT = CFG_GradientRiseTime();
  slices  = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  igwT    = CFG_InterGradientWaitTime();
  
  D[0] = (PVM_RepetitionTime - PVM_MinRepetitionTime + 0.01)/1000.0;
  D[1] = riseT/1000.0;
  D[2] = CFG_AmplifierEnable()/1000.0;
  D[3] = (PVM_EchoTime - PVM_MinEchoTime)/1000.0;
  D[4] = (RampTime+ PVM_AcquisitionTime + RampCompTime + SpoilDur +riseT)/1000.0;   /* shape duration */
  
  /* loop counters */
  L[11] = PVM_DummyScans;

  /* set shaped pulses, in this method ACQ_RfShapes[0] is used           
     the pulse duration is stored in baselevel parameter P[0] */
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]","P[0]");
  
  /* set radial projections */
  PARX_change_dims("GradAmpR",NPro);
  PARX_change_dims("GradAmpP",NPro);
  PARX_change_dims("GradAmpS",NPro);

  SetProj3D(GradAmpR,
	    GradAmpP,
	    GradAmpS,
	    1,1,1);


  DB_MSG(("<--SetPpgParameters"));
}


void SetInfoParameters( void )
{
  int slices, i, spatDim;
  
  DB_MSG(("-->SetInfoParameters"));
  
  spatDim = PTB_GetSpatDim();
  
  ATB_SetAcqMethod();

  ATB_SetAcqFov( Spatial, spatDim, PVM_Fov, PVM_AntiAlias );
  
  ACQ_flip_angle = ExcPulse1.Flipangle;
  
  PARX_change_dims("ACQ_echo_time",1);

  ACQ_echo_time[0] = PVM_EchoTime;
  
  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = PVM_EchoTime;
  
  PARX_change_dims("ACQ_repetition_time",1);
  ACQ_repetition_time[0] = PVM_RepetitionTime;
  
  PARX_change_dims("ACQ_recov_time",1);
  ACQ_recov_time[0] =  
    PVM_RepetitionTime - 
    (PVM_EchoTime/2 + ExcPulse1.Length/2);
  
  PARX_change_dims("ACQ_inversion_time",1);
  ACQ_inversion_time[0] = PVM_InversionTime;
  
  ATB_SetAcqSliceAngle( PtrType3x3 PVM_SPackArrGradOrient[0],
			PVM_NSPacks );
  
  ACQ_slice_orient = Arbitrary_Oblique;
  
  ACQ_slice_thick = PVM_SliceThick;
  
  slices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  
  PARX_change_dims("ACQ_slice_offset",slices);
  PARX_change_dims("ACQ_read_offset",slices);
  PARX_change_dims("ACQ_phase1_offset",slices);
  PARX_change_dims("ACQ_phase2_offset",slices);
  
  for(i=0;i<slices;i++)
  {
    ACQ_slice_offset[i]  = PVM_SliceOffset[i];
    ACQ_read_offset[i]   = PVM_ReadOffset[i];
    ACQ_phase1_offset[i] = PVM_Phase1Offset[i];
    ACQ_phase2_offset[i] = PVM_Phase2Offset[i];
  }
  
  ACQ_read_ext = (int)PVM_AntiAlias[0];
  
  PARX_change_dims("ACQ_slice_sepn", slices==1 ? 1 : slices-1);
  
  if( slices == 1 )
  {
    ACQ_slice_sepn[0] = 0.0;
  }
  else
  {
    for( i=1; i<slices;i++ )
    {
      ACQ_slice_sepn[i-1]=PVM_SliceOffset[i]-PVM_SliceOffset[i-1];
    }
  }
  
  ATB_SetAcqSliceSepn( PVM_SPackArrSliceDistance,
                       PVM_NSPacks );
  
  
  
  ATB_SetAcqPatientPosition();
  
  ACQ_n_t1_points = 1;
  
  if( ParxRelsParHasValue("ACQ_transmitter_coil") == No )
  {
    ACQ_transmitter_coil[0] = '\0';
  }
  
  if( ParxRelsParHasValue("ACQ_contrast_agent") == No )
  {
    ACQ_contrast_agent[0] = '\0';
  }
  
  if( ParxRelsParHasValue("ACQ_contrast") == No )
  {
    ACQ_contrast.volume = 0.0;
    ACQ_contrast.dose = 0.0;
    ACQ_contrast.route[0] = '\0';
    ACQ_contrast.start_time[0] = '\0';
    ACQ_contrast.stop_time[0] = '\0';
  }
  
  ParxRelsParRelations("ACQ_contrast_agent",Yes);
  
  ACQ_position_X = 0.0;
  ACQ_position_Y = 0.0;
  ACQ_position_Z = 0.0;

  PARX_change_dims("ACQ_temporal_delay",1);
  ACQ_temporal_delay[0] = 0.0;
  
  ACQ_RF_power = 0;
  
  ACQ_flipback = No;
 
  // initialize ACQ_n_echo_images ACQ_echo_descr
  //            ACQ_n_movie_frames ACQ_movie_descr
  ATB_ResetEchoDescr();
  ATB_ResetMovieDescr();
  
  
  DB_MSG(("<--SetInfoParameters"));
  
}

void SetMachineParameters( void )
{

  double riseT;

  DB_MSG(("-->SetMachineParameters"));

  riseT = CFG_GradientRiseTime();
 
  if( ParxRelsParHasValue("ACQ_word_size") == No )
  {
    ACQ_word_size = _32_BIT;
  }
 
  DEOSC = (RampTime + PVM_AcquisitionTime + RampCompTime + 2*riseT + SpoilDur) * 1000.0;

  ACQ_scan_shift = -1;
  ParxRelsParRelations("ACQ_scan_shift",Yes);
  
  DE = DE < 6.0 ? 6.0: DE;
  
  PAPS = QP;
 
  /* ACQ_experiment _mode and ACQ_ReceiverSelect: */

  ATB_SetMultiRec();

   
  DB_MSG(("<--SetMachineParameters"));
}


void PrintTimingInfo(void)
{
  double te,tr;

  DB_MSG(("-->PrintTimingInfo"));
  
  te=(P[0]/2000)+D[3]*1000.0 +0.006;

  tr = (te + P[0]/2000.0 +
      (D[2]+D[4]+D[1]+D[0])*1000+0.03);


  DB_MSG(("te: %f should be: %f diff %f\n",
	  te,PVM_EchoTime,
	  te-PVM_EchoTime));


  DB_MSG(("tr: %f  should be: %f  diff %f\n",tr,PVM_RepetitionTime,
	  tr-PVM_RepetitionTime));


  DB_MSG(("<--PrintTimingInfo"));
  
}



