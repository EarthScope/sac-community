/** 
 * @file   wapf.c
 * 
 * @brief  Write a pick to a file
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "ucf.h"
#include "dfm.h"
#include "hdr.h"
#include "eam.h"
#include "bot.h"

#include "clf.h"

/** 
 * Write a pick to an alphanumeric pick file (APF)
 * 
 * @date   860319:  Added PTP option.
 * @date   810105:  Changed Ew.d formats to Gw.d.
 * @date   800829:  Added ability to write times as GMT or not.
 *                  Added ability to write either header info or filename.
 * @date   800505:  Original version.
 *
 */
void 
wapf() {

	char kapfln[133];
	int j, nexday, npkmsc, npksec;
  char *tmp;

  memset(&kapfln[0], 0, 133);

	/* - Each pick consists of a pick_id, event_id, station_id, component_id,
	 *   and pick source (automatic, manual etc.), as well as the date, time,
	 *   and amplitude of the pick itself. 
   *
   * - For certain picks, auxiliary information is also added to the card. 
   */
    
  /* Get filename. */
  tmp = string_list_get(datafiles, cmdfm.idflc-1);
	/* - Each pick output line can contain 200 columns. */

	if( cmeam.lpfgmt ){
		inctim( *nzhour, *nzmin, *nzsec, *nzmsec, cmeam.pkseci, &cmeam.npkhr, 
            &cmeam.npkmn, &npksec, &npkmsc, &nexday );
		cmeam.pksecs = tosecs( npksec, npkmsc );
		incdat( *nzyear, *nzjday, nexday, &cmeam.npkyr, &cmeam.npkjdy );
  }
  
	if( cmeam.lpfgmt && cmeam.lpfstd ){
		if( (strcmp(kmeam.kpkid,"MWF     ") == 0 || 
         strcmp(kmeam.kpkid,"WF      ") == 0) || 
        strcmp(kmeam.kpkid,"WAWF    ") == 0 ){
      if (sprintf(kapfln,"%16s%8s%7.2f%7.2f%4s%5d%3d%3d%3d%6.2f %10.4g %1s %3s%c"
                  ,kevnm, kstnm, *cmpaz, *cmpinc, kmeam.kpkid, cmeam.npkyr,
                  cmeam.npkjdy, cmeam.npkhr, cmeam.npkmn, cmeam.pksecs, 
                  cmeam.pkampl, kmeam.kpksrc, kmeam.kpkrid, 'B' ) < 0) goto L_8000;
      
			for( j = 2; j <= 5; j++ ){
        if(sprintf(kapfln+80+((j-2)*6),"%6.3f",Dtwf[j]) < 0)
          goto L_8000;
      }
      if( sprintf(kapfln+80+(4*6)," %10.4g %10.4g\n", Awf[2], Awf[4] )
          < 0 ) goto L_8000;
    }
		else if( strcmp(kmeam.kpkid,"PTP     ") == 0 ){
      if(sprintf(kapfln,"%16s%8s%7.2f%7.2f%4s%5d%3d%3d%3d%6.2f %10.4g %1s %3s%c%6.3f%10.4g\n",
                 kevnm, kstnm, *cmpaz, *cmpinc, kmeam.kpkid, cmeam.npkyr, 
                 cmeam.npkjdy, cmeam.npkhr, cmeam.npkmn, cmeam.pksecs, 
                 cmeam.pkampl, kmeam.kpksrc, kmeam.kpkrid, 'B', Dtwf[4], 
                 Awf[4] ) < 0) 
        goto L_8000;
    }
		else{
      if(sprintf(kapfln,"%16s%8s%7.2f%7.2f%4s%5d%3d%3d%3d%6.2f %10.4g %1s %3s %c\n",
                 kevnm, kstnm, *cmpaz, *cmpinc, kmeam.kpkid, cmeam.npkyr, 
                 cmeam.npkjdy, cmeam.npkhr, cmeam.npkmn, cmeam.pksecs, 
                 cmeam.pkampl, kmeam.kpksrc, kmeam.kpkrid, 'B' ) < 0) 
        goto L_8000;
    }
  }
	else if( cmeam.lpfgmt && !cmeam.lpfstd ){
		if( (strcmp(kmeam.kpkid,"MWF     ") == 0 || 
         strcmp(kmeam.kpkid,"WF      ") == 0) || 
        strcmp(kmeam.kpkid,"WAWF    ") == 0) {
      if(sprintf(kapfln,"%32s      %4s%5d%3d%3d%3d%6.2f %10.4g %1s %3s%c",
                 tmp, kmeam.kpkid, cmeam.npkyr, cmeam.npkjdy, cmeam.npkhr, 
                 cmeam.npkmn, cmeam.pksecs, cmeam.pkampl, kmeam.kpksrc, 
                 kmeam.kpkrid, 'C' ) < 0) {
        goto L_8000;
      }
			for( j = 2; j <= 5; j++ ){
        if(sprintf(kapfln+80+((j-2)*6),"%6.3f",Dtwf[j] ) < 0)
          goto L_8000;
      }
      if(sprintf(kapfln+80+(4*6)," %10.4g %10.4g\n", Awf[2], Awf[4] )
         < 0) goto L_8000;
    }
		else if( strcmp(kmeam.kpkid,"PTP     ") == 0 ){
      if(sprintf(kapfln,"%32s      %4s%5d%3d%3d%3d%6.2f %10.4g %1s %3s%c%6.3f%10.4g\n",
                 tmp, kmeam.kpkid, cmeam.npkyr, cmeam.npkjdy, cmeam.npkhr, 
                 cmeam.npkmn, cmeam.pksecs, cmeam.pkampl, kmeam.kpksrc, 
                 kmeam.kpkrid, 'C', Dtwf[4], Awf[4] ) < 0) {
        goto L_8000;
      }
    } else {
      if(sprintf(kapfln,"%32s      %4s%5d%3d%3d%3d%6.2f %10.4g %1s %3s%c\n",
                 tmp, kmeam.kpkid, cmeam.npkyr, cmeam.npkjdy, cmeam.npkhr, 
                 cmeam.npkmn, cmeam.pksecs, cmeam.pkampl, kmeam.kpksrc, 
                 kmeam.kpkrid, 'C' ) < 0) {
        goto L_8000;
      }
    }
  }
	else if( !cmeam.lpfgmt && cmeam.lpfstd ){
		if( (strcmp(kmeam.kpkid,"MWF     ") == 0 || 
         strcmp(kmeam.kpkid,"WF      ") == 0) || 
        strcmp(kmeam.kpkid,"WAWF    ") == 0) {
      if(sprintf(kapfln,"%16s%8s%7.2f%7.2f%4s          %10.4g %10.4g %1s %3s%c",
                 kevnm, kstnm, *cmpaz, *cmpinc, kmeam.kpkid, cmeam.pkseci, 
                 cmeam.pkampl, kmeam.kpksrc, kmeam.kpkrid, 'D' ) < 0) goto L_8000;
      
      for( j = 2; j <= 5; j++ ){
        if(sprintf(kapfln+80+((j-2)*6),"%6.3f", Dtwf[j] ) < 0) 
          goto L_8000;
      }
      if(sprintf(kapfln+80+(4*6)," %10.4g %10.4g\n", Awf[2], Awf[4] ) < 0) 
        goto L_8000;
    }
		else if( strcmp(kmeam.kpkid,"PTP     ") == 0 ){
      if(sprintf(kapfln,"%16s%8s%7.2f%7.2f%4s          %10.4g %10.4g %1s %3s%c%6.3f%10.4g\n",
			 kevnm, kstnm, *cmpaz, *cmpinc, kmeam.kpkid, cmeam.pkseci, 
			 cmeam.pkampl, kmeam.kpksrc, kmeam.kpkrid, 'D', Dtwf[4], 
			 Awf[4] ) < 0) goto L_8000;
			}
		else{
      if(sprintf(kapfln,"%16s%8s%7.2f%7.2f%4s          %10.4g %10.4g %1s %3s%c\n",
			 kevnm, kstnm, *cmpaz, *cmpinc, kmeam.kpkid, cmeam.pkseci, 
			 cmeam.pkampl, kmeam.kpksrc, kmeam.kpkrid, 'D' ) < 0) goto L_8000;
			}
		}
	else{
		if( (strcmp(kmeam.kpkid,"MWF     ") == 0 || 
         strcmp(kmeam.kpkid,"WF      ") == 0) || 
        strcmp(kmeam.kpkid,"WAWF    ") == 0){
      if(sprintf(kapfln,"%32s      %4s          %10.4g %10.4g %1s %3s%c",
                 tmp, kmeam.kpkid, cmeam.pkseci, 
                 cmeam.pkampl, kmeam.kpksrc, kmeam.kpkrid, 'E' ) < 0) {
        goto L_8000;
      }
      for( j = 2; j <= 5; j++ ){
        if(sprintf(kapfln+80+((j-2)*6),"%6.3f", Dtwf[j]) < 0) goto L_8000;
      }
      if(sprintf(kapfln+80+(4*6)," %10.4g %10.4g\n", Awf[2], Awf[4] ) < 0)
        goto L_8000;
    }
		else if( strcmp(kmeam.kpkid,"PTP     ") == 0 ){
      if(sprintf(kapfln,"%32s      %4s          %10.4g %10.4g %1s %3s%c%6.3f%10.4g\n",
                 tmp, kmeam.kpkid, cmeam.pkseci, cmeam.pkampl, kmeam.kpksrc, 
                 kmeam.kpkrid, 'E', Dtwf[4], Awf[4] ) < 0) {
        goto L_8000;
      }
    }
		else{
      if(sprintf(kapfln,"%32s      %4s          %10.4g %10.4g %1s %3s%c\n",
                 tmp, kmeam.kpkid, cmeam.pkseci, 
                 cmeam.pkampl, kmeam.kpksrc, kmeam.kpkrid, 'E' ) < 0) {
        goto L_8000;
      }
    }
  }

	/* - Write encoded character string to APF. */

L_8000:
        fprintf(cmeam.napfun,"%s\n",kapfln);

       
	return;

} /* end of function */

