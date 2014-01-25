int gseRead20(char *fileName, char *WorkSetName, int Replace, int MaxWaveforms, int verbose, double MaxPhysMem ) ;

int WriteGSEFile( char *outFile, DBlist tree, char *datasource, int cm6 ) ;

int ReadCompData(FILE *ptr, int Nsamp, int *Data, char *fmt) ;

int CheckSumFromIntArray(int *data, int Npts) ;
int PutArrivalData ( FILE *ptr , DBlist tree ) ;
int PutOriginData ( FILE *ptr , DBlist tree ) ;
