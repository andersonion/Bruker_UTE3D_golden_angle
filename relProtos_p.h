/* /opt/PV6.0.1/prog/curdir/qial/ParaVision/methods/src/golden_angle_UTE3D/initMeth.c */
void initMeth(void);
/* /opt/PV6.0.1/prog/curdir/qial/ParaVision/methods/src/golden_angle_UTE3D/loadMeth.c */
void loadMeth(const char *);
/* /opt/PV6.0.1/prog/curdir/qial/ParaVision/methods/src/golden_angle_UTE3D/parsRelations.c */
void ExcPulse1EnumRel(void);
void ExcPulse1AmplRel(void);
void HandleRFPulseAmplitude(void);
void ExcPulse1Rel(void);
void ExcPulse1Range(void);
void EffSWhRange(void);
void EffSWhRel(void);
void RepetitionTimeRange(void);
void RepetitionTimeRel(void);
void AveragesRange(void);
void AveragesRel(void);
void EchoTimeRange(void);
void EchoTimeRel(void);
void ProUnderRel(void);
void ProUnderRange(void);
void SlewRateRel(void);
void SlewRateRange(void);
/* /opt/PV6.0.1/prog/curdir/qial/ParaVision/methods/src/golden_angle_UTE3D/backbone.c */
void backbone(void);
void UpdateEchoTime(double *const );
void UpdateRepetitionTime(void);
void UpdateGeometry(double *);
double FreqEncodingLimits(int, double *, double *const , double);
void FreqEncodingGradients(double, double, double, double);
void UpdateRFPulses(void);
void CalcNPro(void);
void SetNPro(void);
int SetProj3D(double *, double *, double *, const double, const double, const double);
void UpdateAdjustments(void);
/* /opt/PV6.0.1/prog/curdir/qial/ParaVision/methods/src/golden_angle_UTE3D/BaseLevelRelations.c */
void SetBaseLevelParam(void);
void SetBasicParameters(void);
void SetFrequencyParameters(void);
void SetGradientParameters(void);
void SetPpgParameters(void);
void SetInfoParameters(void);
void SetMachineParameters(void);
void PrintTimingInfo(void);
/* /opt/PV6.0.1/prog/curdir/qial/ParaVision/methods/src/golden_angle_UTE3D/RecoRelations.c */
void SetRecoParam(void);
void RecoDerive(void);
double RecoOptimiseMemory(int, int, int, int, double *);
/* /opt/PV6.0.1/prog/curdir/qial/ParaVision/methods/src/golden_angle_UTE3D/deriveVisu.c */
void deriveVisu(void);
