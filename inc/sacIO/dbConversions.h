#ifndef DB_CONVERSIONS_H
#define DB_CONVERSIONS_H

void dbDelaz(float *slat, float *slon, float *elat, float *elon, 
           float *delt, float *dist, float *azim, float *bazim);

void dbGetEnclosingBox(float latc, float lonc, float radius, float *minLat,
                     float *maxLat, float *minLon, float *maxLon);



#endif
