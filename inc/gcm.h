/** 
 * @file   gcm.h
 * 
 * @brief  Graphics 
 * 
 */

#ifndef _GCM_H_
#define _GCM_H_

void xbegindevices ( int *nerr);
void xenddevices   ( int *nerr);
void xgcmc         ( int index, 
                     int *nerr);
void xsgf          ( int *nerr);
void xvspac        ( int *nerr);

#endif /* _GCM_H_ */
