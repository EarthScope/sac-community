#ifndef MAT_HEADER_DATA_MANAGE_H
#   define MAT_HEADER_DATA_MANAGE_H

   extern int matGetNumChans(void);
   extern int matGetMaxTraceLen(void);
   extern double *matGetSeisRealPtr(int) ;
   extern double *matGetSeisImagPtr(int) ;
   extern int matGetSeisLength(int );
   extern struct hdrTimes *matGetTimesPtr(int );
   extern struct hdrStation *matGetStationPtr(int );
   extern struct hdrEvent *matGetEventPtr(int );
   extern struct hdrUser *matGetUserPtr(int );
   extern struct hdrDataDescrip *matGetDescripPtr(int );
   extern struct hdrEventSta *matGetEvstaPtr(int);
   extern struct hdrLLNLextensions *matGetLLNLPtr(int );
   extern struct hdrDepMec *matGetDepMecPtr(int );
   extern double  *matGetResponsePtr(int );
   extern double *matGetTraceLengthPtr(int );
#endif
