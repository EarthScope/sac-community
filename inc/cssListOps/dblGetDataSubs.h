#ifndef DBL_GET_DATA_SUBS_H
#define DBL_GET_DATA_SUBS_H

int dblGetE1 ( int NPTS , FILE *fptr , struct wfdiscList * wfStruc );
int dblGetS2I2G2 ( int NPTS ,
                    FILE *fptr ,
                    struct wfdiscList * wfStruc ,
                    char * pType );
int dblGetS3 ( int NPTS , FILE * fptr , struct wfdiscList *wfStruc );
int dblGetT4F4 ( int NPTS , FILE * fptr , struct wfdiscList *wfStruc ,
                  char* pType );
int dblGetS4I4 ( int NPTS , FILE *fptr , struct wfdiscList *wfStruc ,
                  char* pType );
int dblGetT8F8 ( int NPTS , FILE * fptr , struct wfdiscList *wfStruc ,
                  char* pType );
void enlarge(FILE *fp, int outMax, int* data, /* int * byteOff,*/ int* nerr);                    


#endif
