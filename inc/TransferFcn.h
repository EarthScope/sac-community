#ifndef TRANSFER_FCN_H
#define TRANSFER_FCN_H

   enum InstrumentType{BroadBandDisplacement,           BroadBandVel,           Benioff,     
                       LLNL_DSS,                WWSSN,           EKA_LP6,    
                       EKA_SP2,                 GBA_LP,          GBA_SP,     
                       USGS_REF,                HFS_LPWB,        S750,      
                       LLSN_L4,                 LRSM_LP,         LRSM_SP, 
                       NORESS_HF,               PORTABLE_LP,     RED_Kirnos,              
                       SL_210,                  Wood_Anderson,   Wood_Anderson_Blacknest, 
                       Wiechert,                WWSSN_Blacknest, WWSP,                    
                       WWSP_Blacknest,          YKA_LP,          YKA_SP};


int GetTheoreticTransferFcn(enum InstrumentType InstType, int NumFreqs, 
                            double DelFreq, double *RealArray, double *ImagArray);

int TransferFcnFromPoleZeroFile(char *Dir, char *file, int Npts, double DelFreq, 
                                double *Real, double *Imag);
                                
int TransferFcnFromEvresp(char *station, char *component, char * units, char *net_code, 
                          double EpochTime, int Verbose, char *Dir, char *file, int nfreq, 
                          double delfrq, double *xre, double *xim); 
                                                         
int TransferFcnFromFAPFile(char *Dir, char *file, int nfreq, double delfrq, 
                                double *xre, double *xim);
                                                                
#endif
