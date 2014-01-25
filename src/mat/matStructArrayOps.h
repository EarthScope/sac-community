#ifndef MAT_STRUCT_ARRAY_OPS_H
#   define MAT_STRUCT_ARRAY_OPS_H

   extern mxArray *matCreateTimesStruct(struct hdrTimes *);
   extern void matUpdateTimesStruct(mxArray *,  struct hdrTimes * );
   extern mxArray *matCreateStationStruct(struct hdrStation *);
   extern void matUpdateStationStruct(mxArray *,  struct hdrStation * );
   extern mxArray *matCreateEventStruct(struct hdrEvent *);
   extern void matUpdateEventStruct(mxArray *,  struct  hdrEvent * );
   extern mxArray *matCreateUserStructArray(struct hdrUser *);
   extern void matUpdateUserStruct(mxArray *, struct hdrUser * );
   extern mxArray *matCreateDescripStruct(struct hdrDataDescrip *);
   extern void matUpdateDescripStruct(mxArray *,  struct hdrDataDescrip * );
   extern mxArray *matCreateEvstaStruct(struct  hdrEventSta *);
   extern void matUpdateEvstaStruct(mxArray *, struct hdrEventSta * );
   extern mxArray *matCreateLLNLStruct(struct hdrLLNLextensions * );
   extern void matUpdateLLNLStruct(mxArray *structPtr, struct hdrLLNLextensions * ); 
   extern mxArray *matCreateDepMecStruct(struct  hdrDepMec *);
   extern void matUpdateDepMecStruct(mxArray *, struct hdrDepMec * );
   extern int matNumBlackBoardVars(void);
   extern struct BlackBoardVars *matGetBBPtr(int);
   extern mxArray *matSetIntBBval(int);
   extern mxArray *matSetFloatBBval(float);
#endif
