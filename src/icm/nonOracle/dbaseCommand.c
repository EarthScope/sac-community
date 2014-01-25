
#include "debug.h"
/** 
 * @file   dbaseCommand.c
 * 
 * @brief  Non-Oracle Functions
 * 
 */
void 
dbaseResponse(int a, 
              double b , 
              double* c, 
              double* d, 
              float* e, 
              int* nerr) {
  UNUSED(a);
  UNUSED(b);
  UNUSED(c);
  UNUSED(d);
  UNUSED(e);
   *nerr = 8101;
}

void 
DisconnectFromOracleTransfer(void) {

}

void 
getSensorInstrumentCalibInfo(double* ncalper, 
                             double* ncalib, 
                             double* calper, 
                             double* calratio ) {
  *ncalper  = -999.0;
  *ncalib   = -999.0;
  *calper   = -999.0;
  *calratio = -999.0;
}


   
