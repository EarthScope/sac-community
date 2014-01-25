#ifndef SUDS_READING_H
#define SUDS_READING_H




int FillDetectorStruct(SUDS_DETECTOR *de, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillAtoDStruct(SUDS_ATODINFO *ad, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillTrigSettingStruct(SUDS_TRIGSETTING *ts, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillEventSettingStruct(SUDS_EVENTSETTING *es, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillStationCompStruct(SUDS_STATIONCOMP *sc, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillDescripTraceStruct(SUDS *S, SUDS_DESCRIPTRACE *dt, 
                                  SUDS_STRUCTTAG Tag, FILE *ptr);
int FillFeatureStruct(SUDS_FEATURE *fe, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillCalibStruct(SUDS_CALIBRATION *ca, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillCommentStruct(SUDS *S, SUDS_COMMENT *co, 
                                  SUDS_STRUCTTAG Tag, FILE *ptr);
int FillEventStruct(SUDS_EVENT *ev, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillTriggerStruct(SUDS_TRIGGERS *tr, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillEVDescriptStruct(SUDS_EVDESCR *ed, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillFocalMechStruct(SUDS_FOCALMECH *fo, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillInstrumentStruct(SUDS_INSTRUMENT *in, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillOriginStruct(SUDS_ORIGIN *orig, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillTimeCorrectionStruct(SUDS_TIMECORRECTION *tc, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillVelocityModelStruct(SUDS_VELMODEL *vm, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillTerminatorStruct(SUDS_TERMINATOR *te, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillLayersStruct(SUDS_LAYERS *la, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillLoctraceStruct(SUDS_LOCTRACE *lo, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillMomentStruct(SUDS_MOMENT *mo, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillProfileStruct(SUDS_PROFILE *pr, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillResidualStruct(SUDS_RESIDUAL *re, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillErrorStruct(SUDS_ERROR *er, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillEquipmentStruct(SUDS_EQUIPMENT *eq, SUDS_STRUCTTAG Tag, FILE *ptr);
int FillChansetStruct(SUDS *S, SUDS_CHANSET *cs, 
                                  SUDS_STRUCTTAG Tag, FILE *ptr);


#endif
