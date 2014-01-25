
#include <string.h>

#include "icm.h"
#include "bool.h"


#include "msg.h"
#include "bot.h"

#include "debug.h"

void /*FUNCTION*/ ckinst(fp, lfp, ip, lip, kp, kp_s, lkp, nerr)
float fp[];
int lfp[];
int ip[];
int lip[];
char *kp;   int kp_s;
int lkp[];
int *nerr;
{
#define KP(I_,J_)	(kp+(I_)*(kp_s)+(J_))
	char temp[21];

	int *const Lfp = &lfp[0] - 1;
	int *const Lip = &lip[0] - 1;
	int *const Lkp = &lkp[0] - 1;

  UNUSED(ip);
  UNUSED(fp);

	/* MODIFICATIONS:
	 *    900409:  Added LNN instrument type.
	 * */
	*nerr = 0;
	strcpy( temp, "                    " );

	if( memcmp(KP(0,0),"ELMAG",5) == 0 && (!Lfp[1] || !Lfp[2]) ){
		*nerr = 2101;
		setmsg( "ERROR", *nerr );

		}
	else if( memcmp(KP(0,0),"EYEOMG",6) == 0 && (!Lip[1]) ){
		*nerr = 2102;
		setmsg( "ERROR", *nerr );

		}
	else if( memcmp(KP(0,0),"GENERAL",7) == 0 && ((!Lip[1]) || ((!Lfp[2]
	  || (!Lfp[1])) || (!Lfp[3]))) ){
		*nerr = 2103;
		setmsg( "ERROR", *nerr );

		}
	else if( memcmp(KP(0,0),"POLEZERO",8) == 0 && (!Lkp[2]) ){
		*nerr = 2104;
		setmsg( "ERROR", *nerr );
		apcmsg( "POLEZERO",9 );

		}
    else if (memcmp(KP(0,0), "FAP", 3) == 0 && (!Lkp[2]) ) {
      *nerr = 2104;
      setmsg("ERROR", *nerr);
      apcmsg("FAP", 3);
    }
	else if( memcmp(KP(0,0),"LLL",3) == 0 && (!Lkp[2]) ){
		*nerr = 2104;
		setmsg( "ERROR", *nerr );
		apcmsg( "LLL",4 );

		}
	else if( memcmp(KP(0,0),"LLL",3) == 0 && (Lkp[2]) ){
		modcase( TRUE, KP(1,0), 8, temp );
		if( ((((temp[0] != 'L' && temp[0] != 'M') && temp[0] != 'K') && 
		 temp[0] != 'E') && temp[0] != 'B') || (((temp[1] != 'V' && 
		 temp[1] != 'R') && temp[1] != 'T') && temp[1] != 'B') ){
			*nerr = 2105;
			setmsg( "ERROR", *nerr );
			apcmsg( "LLL:",5 );
			apcmsg( KP(1,0),kp_s );
			}
		else{
			if( strcmp(temp,"BB                  ") == 0 ){
				if( !Lfp[1] || !Lfp[3] ){
					*nerr = 2106;
					setmsg( "ERROR", *nerr );
					}
				}
			}

		}
	else if( memcmp(KP(0,0),"NORESS",6) == 0 && (!Lkp[2]) ){
		*nerr = 2104;
		setmsg( "ERROR", *nerr );
		apcmsg( "NORESS.",8 );

		}
	else if( memcmp(KP(0,0),"NORESS",6) == 0 && (Lkp[2]) ){
		modcase( TRUE, KP(1,0), 8, temp );
		if( ((temp[0] != 'L' && temp[0] != 'I') && temp[0] != 'S') || 
		 temp[1] != 'P' ){
			*nerr = 2105;
			setmsg( "ERROR", *nerr );
			apcmsg( "NORESS:",8 );
			apcmsg( KP(1,0),kp_s );
			}

		}
	else if( memcmp(KP(0,0),"PORTABLE",8) == 0 && (((!Lfp[1]) || (!Lfp[3]
	 )) || (!Lfp[4])) ){
		*nerr = 2107;
		setmsg( "ERROR", *nerr );

		}
	else if( memcmp(KP(0,0),"RSTN",4) == 0 && (!Lkp[2]) ){
		*nerr = 2104;
		setmsg( "ERROR", *nerr );
		apcmsg( "RSTN.",6 );

		}
	else if( memcmp(KP(0,0),"RSTN",4) == 0 && Lkp[2] ){
		modcase( TRUE, KP(1,0), 8, temp );
		if( (((((((memcmp(temp,"CP",2) != 0 && memcmp(temp
		 ,"NT",2) != 0) && memcmp(temp,"NY",2) != 0) && memcmp(temp
		 ,"ON",2) != 0) && memcmp(temp,"SD",2) != 0) || (temp[2] != 
		 'K' && temp[2] != '7')) || ((temp[3] != 'L' && temp[3] != 
		 'M') && temp[3] != 'S')) || temp[4] != '.') || ((temp[5] != 
		 'Z' && temp[5] != 'N') && temp[5] != 'E') ){
			*nerr = 2105;
			setmsg( "ERROR", *nerr );
			apcmsg( "RSTN:",6 );
			apcmsg( KP(1,0),kp_s );
			}

		}
	else if( memcmp(KP(0,0),"SANDIA",6) == 0 && (!Lkp[2]) ){
		*nerr = 2104;
		setmsg( "ERROR", *nerr );
		apcmsg( "SANDIA.",8 );

		}
	else if( memcmp(KP(0,0),"SANDIA",6) == 0 && Lkp[2] ){
		modcase( TRUE, KP(1,0), 8, temp );
		if( temp[0] != 'N' && temp[0] != 'O' ){
			*nerr = 2105;
			setmsg( "ERROR", *nerr );
			apcmsg( "SANDIA:",8 );
			apcmsg( KP(1,0),kp_s );
			}
		else{
			if( ((((temp[1] != 'T' && temp[1] != 'L') && temp[1] != 
			 'B') && temp[1] != 'D') && temp[1] != 'N') && temp[1] != 
			 'E' ){
				*nerr = 2105;
				setmsg( "ERROR", *nerr );
				apcmsg( "SANDIA:",8 );
				apcmsg( KP(1,0),kp_s );
				}
			else{
				if( temp[1] == 'E' && temp[0] != 'O' ){
					*nerr = 2105;
					setmsg( "ERROR", *nerr );
					apcmsg( "SANDIA:",8 );
					apcmsg( KP(1,0),kp_s );
					aplmsg( "Invalid combination of \"O\" and \"E\"."
					 ,36 );
					}
				if( temp[0] == 'N' && ((temp[2] != 'V' && temp[2] != 
				 'R') && temp[2] != 'T') ){
					*nerr = 2105;
					setmsg( "ERROR", *nerr );
					apcmsg( "SANDIA:",8 );
					apcmsg( KP(1,0),kp_s );
					aplmsg( "Sub-type \"N\" must be combined with"
					 ,35 );
					apcmsg( "\"V\", \"R\", or \"T\".",18 );
					}
				}
			}

		}
	else if( memcmp(KP(0,0),"SRO",3) == 0 && (!Lkp[2]) ){
		*nerr = 2104;
		setmsg( "ERROR", *nerr );
		apcmsg( "SRO.",5 );

		}
	else if( memcmp(KP(0,0),"SRO",3) == 0 && Lkp[2] ){
		modcase( TRUE, KP(1,0), 8, temp );
		if( (strcmp(temp,"BB") == 0 || 
         strcmp(temp,"SP") == 0) || 
        strcmp(temp,"LPDE") == 0 ){
			}
		else{
			*nerr = 2105;
			setmsg( "ERROR", *nerr );
			apcmsg( "SRO:",5 );
			apcmsg( KP(1,0),kp_s );
			if( strcmp(temp,"LPST                ") == 0 )
				aplmsg( "This sub-type is not currently supported."
				 ,42 );
			}

		}
	else if( memcmp(KP(0,0),"REFTEK",6) == 0 && (((!Lfp[1] || !Lfp[3]
	 ) || !Lfp[4]) || !Lfp[6]) ){
		*nerr = 2113;
		setmsg( "ERROR", *nerr );

		}
	else if( memcmp(KP(0,0),"LNN",3) == 0 && !Lkp[2] ){
		*nerr = 2104;
		setmsg( "ERROR", *nerr );
		apcmsg( "LNN.",5 );

		}
	else if( memcmp(KP(0,0),"LNN",3) == 0 && Lkp[2] ){
		modcase( TRUE, KP(1,0), 8, temp );
		if( memcmp(temp,"BB",2) != 0 && memcmp(temp
		 ,"HF",2) != 0 ){
			*nerr = 2105;
			setmsg( "ERROR", *nerr );
			apcmsg( "LNN:",5 );
			apcmsg( KP(1,0),kp_s );
			aplmsg( "Allowed subtypes are BB and HF.",32 );
			}
		}
	else{

		}

       
	return;

#undef	KP
} /* end of function */

