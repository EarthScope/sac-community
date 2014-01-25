
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "sss.h"
#include "bool.h"
#include "co.h"
#include "ucf.h"
#include "bot.h"
#include "dff.h"
#include "debug.h"

static int swap_data = FALSE;

double umod(double zs, int isrc[], int nph);
double zmod(double uend, int js, int nph);

struct t_tblfile {
int nasgr;
FILE *file;
} tblfile;

void /*FUNCTION*/ bkin(lu, nrec, len, buf)
int lu, nrec, len;
double buf[];
{
	static int i;
  UNUSED(lu);

	double *const Buf = &buf[0] - 1;



	/* $$$$$ calls no other routines $$$$$
	 *
	 *   Bkin reads a block of len double precision words into array buf(len)
	 *   from record nrec of the direct access unformatted file connected to
	 *   logical unit lu.
	 * */

	if( nrec <= 0 )
		goto L_1;
/*	fseek_rd( UFP(lu), UOS(lu,nrec), URECL(lu), UBUF(lu) );  */
/*	sscanunf( UBUF(lu), "%& ", (size_t)len*sizeof(double),buf ); */

	{
	fseek(tblfile.file, (nrec - 1)*tblfile.nasgr, 0);
	fread(buf, sizeof(double), len, tblfile.file);
        if(swap_data) {
          for(i = 0; i < len; i++) {
            byteswap(&buf[i], sizeof(double));
          }
        }
	}

	return;
	/*   If the record doesn't exist, zero fill the buffer. */
L_1:
	for( i = 1; i <= len; i++ ){
		Buf[i] = 0e0;
		}
	return;
} /* end of function */

#include <string.h>
#define	LCMD	16
#define	NCMD	4

#include "ttlim.h"
struct t_brkc {
	double zs, pk[JSEG], pu[2][JTSM0], pux[2][JXSM], tauu[2][JTSM], 
	 xu[2][JXSM], px[2][JBRN], xt[2][JBRN], taut[JOUT], coef[JOUT][5], 
	 tauc[JTSM], xc[JXSM], tcoef[2][JBRNA][5], tp[2][JBRNU];
	float odep, fcs[3][JSEG];
	int nin, nph0, int0[2], ki, msrc[2], isrc[2], nseg, nbrn, 
	 ku[2], km[2], nafl[3][JSEG], indx[2][JSEG], kndx[2][JSEG], iidx[JSEG], 
	 jidx[JBRN], kk[JSEG];
	}	brkc;
struct t_pcdc {
	char phcd[JBRN][9];
	}	pcdc;
struct t_prtflc {
	int segmsk[JSEG], prnt[2];
	}	prtflc;
void /*FUNCTION*/ brnset(nn, pcntl, pcntl_s, prflg)
int nn;
char *pcntl;   int pcntl_s;
int prflg[];
{
#define PCNTL(I_,J_)	(pcntl+(I_)*(pcntl_s)+(J_))
	static char phlst[JSEG][9], phtmp[9], segcd[JBRN][9];
	static int all, fnd;
	static int i, i_, j, j1, j2, j_, k, k_, kseg, l, 
	  no, nsgpt[JBRN];
	static char cmdcd[NCMD][9]={"P       ","P+      ","basic   ","S+      "};
	static char cmdlst[LCMD][9]={"P       ","PKiKP   ","PcP     ",
	 "pP      ","pPKiKP  ","sP      ","sPKiKP  ","ScP     ","SKP     ",
	 "PKKP    ","SKKP    ","PP      ","S       ","ScS     ","sS      ",
	 "pS      "};
	static int ncmpt[NCMD][2]={1,2,1,7,1,13,13,16};
        char *s1;


	/* int *const Iidx = &brkc.iidx[0] - 1; */
	/* int *const Int0 = &brkc.int0[0] - 1; */
	/* int *const Isrc = &brkc.isrc[0] - 1; */
	int *const Jidx = &brkc.jidx[0] - 1;
	/* int *const Kk = &brkc.kk[0] - 1; */
	/* int *const Km = &brkc.km[0] - 1; */
	/* int *const Ku = &brkc.ku[0] - 1; */
	/* int *const Msrc = &brkc.msrc[0] - 1; */
	int *const Nsgpt = &nsgpt[0] - 1;
	/* double *const Pk = &brkc.pk[0] - 1; */
	int *const Prflg = &prflg[0] - 1;
	int *const Prnt = &prtflc.prnt[0] - 1;
	int *const Segmsk = &prtflc.segmsk[0] - 1;
	/* double *const Tauc = &brkc.tauc[0] - 1; */
	/* double *const Taut = &brkc.taut[0] - 1; */
	/* double *const Xc = &brkc.xc[0] - 1; */



	/*   Brnset takes character array pcntl(nn) as a list of nn tokens to be
	 *   used to select desired generic branches.  Prflg(3) is the old
	 *   prnt(2) debug print flags in the first two elements plus a new print
	 *   flag which controls a branch selected summary from brnset.  Note that
	 *   the original two flags controlled a list of all tau interpolations
	 *   and a branch range summary respectively.  The original summary output
	 *   still goes to logical unit 10 (ttim1.lis) while the new output goes
	 *   to the standard output (so the caller can see what happened).  Each
	 *   token of pcntl may be either a generic branch name (e.g., P, PcP,
	 *   PKP, etc.) or a keyword (defined in the data statement for cmdcd
	 *   below) which translates to more than one generic branch names.  Note
	 *   that generic branch names and keywords may be mixed.  The keywords
	 *   'all' (for all branches) and 'query' (for an interactive token input
	 *   query mode) are also available.
	 * */
	/*   Segmsk is a logical array that actually implements the branch
	 *   editing in depset and depcor. */

	/*   The keywords do the following:
	 *      P      gives P-up, P, Pdiff, PKP, and PKiKP
	 *      P+     gives P-up, P, Pdiff, PKP, PKiKP, PcP, pP, pPdiff, pPKP,
	 *             pPKiKP, sP, sPdiff, sPKP, and sPKiKP
	 *      S+     gives S-up, S, Sdiff, SKS, sS, sSdiff, sSKS, pS, pSdiff,
	 *             and pSKS
	 *      basic  gives P+ and S+ as well as ScP, SKP, PKKP, SKKP, PP, and
	 *             P'P'
	 *   Note that generic S gives S-up, Sdiff, and SKS already and so
	 *   doesn't require a keyword.
	 * */

	/*   Take care of the print flags. */
	Prnt[1] = Prflg[1];
	Prnt[2] = Prflg[2];
	if( Prnt[1] )
		Prnt[2] = TRUE;
	/*   Copy the token list into local storage. */
	no = min( nn, JSEG );
	for( i = 1; i <= no; i++ ){
		i_ = i - 1;
		fstrncpy( phlst[i_], 8, PCNTL(i_,0), strlen(PCNTL(i_,0)));
		}

	/*   An 'all' keyword is easy as this is already the default. */
  //L_1:
	all = FALSE;
	if( no == 1 && (strcmp(phlst[0],"all     ") == 0 || strcmp(phlst[0]
	 ,"ALL     ") == 0) )
		all = TRUE;
	if( all && !Prflg[3] )
		return;

	/*   Make one or two generic branch names for each segment.  For example,
	 *   the P segment will have the names P and PKP, the PcP segment will
	 *   have the name PcP, etc.
	 * */
	kseg = 0;
	j = 0;
	/*   Loop over the segments. */
	for( i = 1; i <= brkc.nseg; i++ ){
		i_ = i - 1;
		if( !all )
			Segmsk[i] = FALSE;
		/*   For each segment, loop over associated branches. */
L_9:
		j = j + 1;
		strcpy( phtmp, pcdc.phcd[j - 1] );
		/*   Turn the specific branch name into a generic name by stripping out
		 *   the crustal branch and core phase branch identifiers. */
		for( l = 2; l <= 8; l++ ){
L_6:
			if( phtmp[l - 1] == ' ' )
				goto L_4;
			if( (phtmp[l - 1] != 'g' && phtmp[l - 1] != 'b') && phtmp[l - 
			 1] != 'n' )
				goto L_5;
			if( l < 8 ) {
                                strncpy((s1=malloc(9-(l+1) + 1)),phtmp+l,9-(l+1));
                                s1[9-(l+1)] = '\0';
				subscpy( phtmp, l - 1, -1, 8, s1 );
                                free(s1);
			      }
			if( l >= 8 )
				subscpy( phtmp, l - 1, -1, 8, " " );
			goto L_6;
L_5:
			if( l >= 8 )
				goto L_3;
			if( (memcmp(phtmp+l - 1,"ab",2) != 0 &&
                             memcmp(phtmp+l - 1,"ac",2) != 0) &&
                             memcmp(phtmp+l - 1,"df",2) != 0 )
				goto L_3;
			subscpy( phtmp, l - 1, -1, 8, " " );
			goto L_4;
L_3:
			;
			}
L_4:
		if( kseg < 1 )
			goto L_7;
		if( memcmp(phtmp,segcd[kseg - 1],strlen(phtmp)) == 0 )
			goto L_8;
L_7:
		kseg = kseg + 1;
		strcpy( segcd[kseg - 1], phtmp );
		Nsgpt[kseg] = i;
L_8:
		if( Jidx[j] < brkc.indx[1][i_] )
			goto L_9;
		}
	if( all )
		goto L_24;

	/*   Interpret the tokens in terms of the generic branch names.
	 * */
	for( i = 1; i <= no; i++ ){
		i_ = i - 1;
		/*   Try for a keyword first. */
		for( j = 1; j <= NCMD; j++ ){
			j_ = j - 1;
			if( memcmp(phlst[i_],cmdcd[j_],strlen(phlst[i_])) == 0 )
				goto L_12;
			}

		/*   If the token isn't a keyword, see if it is a generic branch name. */
		fnd = FALSE;
		for( k = 1; k <= kseg; k++ ){
			k_ = k - 1;
			if( memcmp(phlst[i_],segcd[k_],strlen(phlst[i_])) != 0 )
				goto L_14;
			fnd = TRUE;
			l = Nsgpt[k];
			Segmsk[l] = TRUE;
L_14:
			;
			}
		/*   If no matching entry is found, warn the caller. */
		if( !fnd )
			{
			fprintf( stdout, "Brnset:  phase %s  not found.\n", phlst[i_]
			  );
			}
		goto L_10;

		/*   If the token is a keyword, find the matching generic branch names. */
L_12:
		j1 = ncmpt[j - 1][0];
		j2 = ncmpt[j - 1][1];
		for( j = j1; j <= j2; j++ ){
			j_ = j - 1;
			for( k = 1; k <= kseg; k++ ){
				k_ = k - 1;
				if( memcmp(cmdlst[j_],segcd[k_],strlen(cmdlst[j_])) != 0 )
					goto L_15;
				l = Nsgpt[k];
				Segmsk[l] = TRUE;
L_15:
				;
				}
			}
L_10:
		;
		}

	/*   Make the caller a list of the generic branch names selected.
	 * */
L_24:
	if( !Prflg[3] )
		return;
	fnd = FALSE;
	j2 = 0;
	/*   Loop over segments. */
	for( i = 1; i <= brkc.nseg; i++ ){
		i_ = i - 1;
		if( !Segmsk[i] )
			goto L_16;
		/*   If selected, find the associated generic branch names. */
		j2 = j2 + 1;
		for( j1 = j2; j1 <= kseg; j1++ ){
			if( Nsgpt[j1] == i )
				goto L_18;
			}
		fprintf( stdout, "Brnset:  Segment pointer (%d ) missing?\n", 
		 i );
		goto L_16;
L_18:
		for( j2 = j1; j2 <= kseg; j2++ ){
			if( Nsgpt[j2] != i )
				goto L_20;
			}
		j2 = kseg + 1;
		/*   Print the result. */
L_20:
		j2 = j2 - 1;
		fnd = TRUE;
		/*     print 102,i,(segcd(j),j=j1,j2)
		 *102  format(10x,i5,5(2x,a)) */
L_16:
		;
		}
	return;
#undef	PCNTL
} /* end of function */

struct t_umdc {
	double pm[2][JSRC], zm[2][JSRC];
	int ndex[2][JSRC], mt[2];
	}	umdc;
struct t_tabc {
	double us[2], pt[JOUT], tau[JOUT][4], xlim[JOUT][2], xbrn[3][JBRN], 
	 dbrn[2][JBRN];
	float xn, pn, tn, dn, hn;
	int jndx[2][JBRN], idel[3][JBRN], mbr1, mbr2;
	}	tabc;
struct t_pdec {
	double ua[2][5], taua[2][5];
	float deplim;
	int ka;
	}	pdec;
void /*FUNCTION*/ depcor(nph)
int nph;
{
	static int noend, noext;
	/* int _i, _r; */
	static int i, i1, i2, i_, iph, is, j, j_, k, k1, k2, k_, 
	 kph, ks, l, l_, lp, m, ms, mu, n1;
	static float ztol;
	static double du, fac, sgn, tauus1[2], tauus2[2], ttau, tx, u0, 
	 u1, umin, xus1[2], xus2[2], z0, z1;
/*	void abort();  */
	static float tol = .01;
	static double dtol = 1e-6;
	static int lpower = 7;
	static int _aini = 1;
	double *const tup = (double*)brkc.tauc;

	int *const Iidx = &brkc.iidx[0] - 1;
	/* int *const Int0 = &brkc.int0[0] - 1; */
	int *const Isrc = &brkc.isrc[0] - 1;
	int *const Jidx = &brkc.jidx[0] - 1;
	int *const Kk = &brkc.kk[0] - 1;
	int *const Km = &brkc.km[0] - 1;
	int *const Ku = &brkc.ku[0] - 1;
	int *const Msrc = &brkc.msrc[0] - 1;
	int *const Mt = &umdc.mt[0] - 1;
	double *const Pk = &brkc.pk[0] - 1;
	/* int *const Prnt = &prtflc.prnt[0] - 1; */
	double *const Pt = &tabc.pt[0] - 1;
	int *const Segmsk = &prtflc.segmsk[0] - 1;
	double *const Tauc = &brkc.tauc[0] - 1;
	double *const Taut = &brkc.taut[0] - 1;
	double *const Tauus1 = &tauus1[0] - 1;
	double *const Tauus2 = &tauus2[0] - 1;
	double *const Tup = &tup[0] - 1;
	double *const Us = &tabc.us[0] - 1;
	double *const Xc = &brkc.xc[0] - 1;
	double *const Xus1 = &xus1[0] - 1;
	double *const Xus2 = &xus2[0] - 1;

	if( _aini ){ /* Do 1 TIME INITIALIZATIONS! */
		pdec.deplim = 1.1;
		pdec.ka = 4;
		_aini = TRUE;
	}


	if( nph == brkc.nph0 )
		goto L_1;
	brkc.nph0 = nph;
	Us[nph] = umod( brkc.zs, brkc.isrc, nph );
	/*   If we are in a high slowness zone, find the slowness of the lid. */
	umin = Us[nph];
	ks = Isrc[nph];
	for( i = 1; i <= ks; i++ ){
		i_ = i - 1;
		if( umdc.pm[nph - 1][i_] > umin )
			goto L_2;
		umin = umdc.pm[nph - 1][i_];
L_2:
		;
		}
	/*   Find where the source slowness falls in the ray parameter array. */
	n1 = Ku[nph] + 1;
	for( i = 2; i <= n1; i++ ){
		i_ = i - 1;
		if( brkc.pu[nph - 1][i_] > umin )
			goto L_4;
		}
	k2 = n1;
	if( brkc.pu[nph - 1][n1 - 1] == umin )
		goto L_50;
    fprintf(stderr, "libtau: error, exiting\n");
	abort;
L_4:
	k2 = i;

	/*   Read in the appropriate depth correction values.
	 * */
L_50:
	noext = FALSE;
	sgn = 1e0;
	if( Msrc[nph] == 0 )
		Msrc[nph] = 1;
	/*   See if the source depth coincides with a model sample */
	ztol = tabc.xn*tol/(1. - tabc.xn*brkc.odep);
	if( fabs( brkc.zs - umdc.zm[nph - 1][ks] ) > ztol )
		goto L_5;
	ks = ks + 1;
	goto L_6;
L_5:
	if( fabs( brkc.zs - umdc.zm[nph - 1][ks - 1] ) > ztol )
		goto L_7;
	/*   If so flag the fact and make sure that the right integrals are
	 *   available. */
L_6:
	noext = TRUE;
	if( Msrc[nph] == ks )
		goto L_8;
	bkin( brkc.nin, umdc.ndex[nph - 1][ks - 1], Ku[nph] + Km[nph], 
	 tup );
	goto L_11;
	/*   If it is necessary to interpolate, see if appropriate integrals
	 *   have already been read in. */
L_7:
	if( Msrc[nph] != ks + 1 )
		goto L_9;
	ks = ks + 1;
	sgn = -1e0;
	goto L_8;
L_9:
	if( Msrc[nph] == ks )
		goto L_8;
	/*   If not, read in integrals for the model depth nearest the source
	 *   depth. */
	if( fabs( umdc.zm[nph - 1][ks - 1] - brkc.zs ) <= fabs( umdc.zm[nph - 1][ks] - 
	 brkc.zs ) )
		goto L_10;
	ks = ks + 1;
	sgn = -1e0;
L_10:
	bkin( brkc.nin, umdc.ndex[nph - 1][ks - 1], Ku[nph] + Km[nph], 
	 tup );
	/*   Move the depth correction values to a less temporary area. */
L_11:
	for( i = 1; i <= Ku[nph]; i++ ){
		i_ = i - 1;
		brkc.tauu[nph - 1][i_] = Tup[i];
		}
	k = Ku[nph];
	for( i = 1; i <= Km[nph]; i++ ){
		i_ = i - 1;
		k = k + 1;
		Xc[i] = Tup[k];
		brkc.xu[nph - 1][i_] = Tup[k];
		}

	/*   Fiddle pointers.
	 * */
L_8:
	Msrc[nph] = ks;
	noend = FALSE;
	if( fabs( umin - brkc.pu[nph - 1][k2 - 2] ) <= dtol*umin )
		k2 = k2 - 1;
	if( fabs( umin - brkc.pu[nph - 1][k2 - 1] ) <= dtol*umin )
		noend = TRUE;
	if( Msrc[nph] <= 1 && noext )
		Msrc[nph] = 0;
	k1 = k2 - 1;
	if( noend )
		k1 = k2;
	if( noext )
		goto L_14;

	/*   Correct the integrals for the depth interval [zm(msrc),zs].
	 * */
	ms = Msrc[nph];

        if ( sgn >= 0.0 ) {
/* L_16: */
	u0 = umdc.pm[nph - 1][ms - 1];
	z0 = umdc.zm[nph - 1][ms - 1];
	u1 = Us[nph];
	z1 = brkc.zs;
	goto L_17;
      }
/* L_15: */
	u0 = Us[nph];
	z0 = brkc.zs;
	u1 = umdc.pm[nph - 1][ms - 1];
	z1 = umdc.zm[nph - 1][ms - 1];
L_17:
	mu = 1;
	for( k = 1; k <= k1; k++ ){
		k_ = k - 1;
		tauint( brkc.pu[nph - 1][k_], u0, u1, z0, z1, &ttau, &tx );
		Tauc[k] = brkc.tauu[nph - 1][k_] + sgn*ttau;
		if( fabs( brkc.pu[nph - 1][k_] - brkc.pux[nph - 1][mu - 1] ) > 
		 dtol )
			goto L_18;
		Xc[mu] = brkc.xu[nph - 1][mu - 1] + sgn*tx;
		mu = mu + 1;
L_18:
		;
		}
	goto L_39;
	/*   If there is no correction, copy the depth corrections to working
	 *   storage. */
L_14:
	mu = 1;
	for( k = 1; k <= k1; k++ ){
		k_ = k - 1;
		Tauc[k] = brkc.tauu[nph - 1][k_];
		if( fabs( brkc.pu[nph - 1][k_] - brkc.pux[nph - 1][mu - 1] ) > 
		 dtol )
			goto L_40;
		Xc[mu] = brkc.xu[nph - 1][mu - 1];
		mu = mu + 1;
L_40:
		;
		}

	/*   Calculate integrals for the ray bottoming at the source depth.
	 * */
L_39:
	Xus1[nph] = 0e0;
	Xus2[nph] = 0e0;
	mu = mu - 1;
	if( fabs( umin - Us[nph] ) > dtol && fabs( umin - brkc.pux[nph - 1][mu - 1] ) <= 
	 dtol )
		mu = mu - 1;
	/*   This loop may be skipped only for surface focus as range is not
	 *   available for all ray parameters. */
	if( Msrc[nph] <= 0 )
		goto L_1;
	is = Isrc[nph];
	Tauus2[nph] = 0e0;
	if( fabs( brkc.pux[nph - 1][mu - 1] - umin ) > dtol || fabs( Us[nph] - 
	 umin ) > dtol )
		goto L_48;
	/*   If we happen to be right at a discontinuity, range is available. */
	Tauus1[nph] = Tauc[k1];
	Xus1[nph] = Xc[mu];
	goto L_33;
	/*   Integrate from the surface to the source. */
L_48:
	Tauus1[nph] = 0e0;
	j = 1;
	if( is < 2 )
		goto L_42;
	for( i = 2; i <= is; i++ ){
		i_ = i - 1;
		tauint( umin, umdc.pm[nph - 1][j - 1], umdc.pm[nph - 1][i_], 
		 umdc.zm[nph - 1][j - 1], umdc.zm[nph - 1][i_], &ttau, &tx );
		Tauus1[nph] = Tauus1[nph] + ttau;
		Xus1[nph] = Xus1[nph] + tx;
		j = i;
		}
L_42:
	if( fabs( umdc.zm[nph - 1][is - 1] - brkc.zs ) <= dtol )
		goto L_33;
	/*   Unless the source is right on a sample slowness, one more partial
	 *   integral is needed. */
	tauint( umin, umdc.pm[nph - 1][is - 1], Us[nph], umdc.zm[nph - 1][is - 1], 
	 brkc.zs, &ttau, &tx );
	Tauus1[nph] = Tauus1[nph] + ttau;
	Xus1[nph] = Xus1[nph] + tx;
L_33:
	if( umdc.pm[nph - 1][is] < umin )
		goto L_41;
	/*   If we are in a high slowness zone, we will also need to integrate
	 *   down to the turning point of the shallowest down-going ray. */
	u1 = Us[nph];
	z1 = brkc.zs;
	for( i = is + 1; i <= Mt[nph]; i++ ){
		i_ = i - 1;
		u0 = u1;
		z0 = z1;
		u1 = umdc.pm[nph - 1][i_];
		z1 = umdc.zm[nph - 1][i_];
		if( u1 < umin )
			goto L_36;
		tauint( umin, u0, u1, z0, z1, &ttau, &tx );
		Tauus2[nph] = Tauus2[nph] + ttau;
		Xus2[nph] = Xus2[nph] + tx;
		}
L_36:
	z1 = zmod( umin, i - 1, nph );
	if( fabs( z0 - z1 ) <= dtol )
		goto L_41;
	/*   Unless the turning point is right on a sample slowness, one more
	 *   partial integral is needed. */
	tauint( umin, u0, umin, z0, z1, &ttau, &tx );
	Tauus2[nph] = Tauus2[nph] + ttau;
	Xus2[nph] = Xus2[nph] + tx;

	/*   Take care of converted phases.
	 * */
L_41:
	iph = (nph%2) + 1;
	Xus1[iph] = 0e0;
	Xus2[iph] = 0e0;
	Tauus1[iph] = 0e0;
	Tauus2[iph] = 0e0;
	switch( nph ){
		case 1: goto L_59;
		case 2: goto L_61;
		}
L_61:
	if( umin > brkc.pu[0][Ku[1]] )
		goto L_53;

	/*   If we are doing an S-wave depth correction, we may need range and
	 *   tau for the P-wave which turns at the S-wave source slowness.  This
	 *   would bd needed for sPg and SPg when the source is in the deep mantle.
	 * */
	for( j = 1; j <= brkc.nbrn; j++ ){
		j_ = j - 1;
		if( (memcmp(pcdc.phcd[j_],"sP",2) != 0 && memcmp(pcdc.phcd[j_]
		 ,"SP",2) != 0) || brkc.px[1][j_] <= 0e0 )
			goto L_44;
		if( umin >= brkc.px[0][j_] && umin < brkc.px[1][j_] )
			goto L_45;
L_44:
		;
		}
	goto L_53;

	/*   If we are doing an P-wave depth correction, we may need range and
	 *   tau for the S-wave which turns at the P-wave source slowness.  This
	 *   would be needed for pS and PS.
	 * */
L_59:
	for( j = 1; j <= brkc.nbrn; j++ ){
		j_ = j - 1;
		if( (memcmp(pcdc.phcd[j_],"pS",2) != 0 && memcmp(pcdc.phcd[j_]
		 ,"PS",2) != 0) || brkc.px[1][j_] <= 0e0 )
			goto L_60;
		if( umin >= brkc.px[0][j_] && umin < brkc.px[1][j_] )
			goto L_45;
L_60:
		;
		}
	goto L_53;

	/*   Do the integral. */
L_45:
	j = 1;
	for( i = 2; i <= Mt[iph]; i++ ){
		i_ = i - 1;
		if( umin >= umdc.pm[iph - 1][i_] )
			goto L_47;
		tauint( umin, umdc.pm[iph - 1][j - 1], umdc.pm[iph - 1][i_], 
		 umdc.zm[iph - 1][j - 1], umdc.zm[iph - 1][i_], &ttau, &tx );
		Tauus1[iph] = Tauus1[iph] + ttau;
		Xus1[iph] = Xus1[iph] + tx;
		j = i;
		}
L_47:
	z1 = zmod( umin, j, iph );
	if( fabs( umdc.zm[iph - 1][j - 1] - z1 ) <= dtol )
		goto L_53;
	/*   Unless the turning point is right on a sample slowness, one more
	 *   partial integral is needed. */
	tauint( umin, umdc.pm[iph - 1][j - 1], umin, umdc.zm[iph - 1][j - 1], 
	 z1, &ttau, &tx );
	Tauus1[iph] = Tauus1[iph] + ttau;
	Xus1[iph] = Xus1[iph] + tx;

L_53:
	pdec.ua[nph - 1][0] = -1e0;
	/*     if(odep.ge.deplim.or.odep.le..1) go to 43 */
	if( brkc.odep >= pdec.deplim ) {
		goto L_43;
    }
	for( i = 1; i <= brkc.nseg; i++ ){
		i_ = i - 1;
		if( !Segmsk[i] )
			goto L_57;
		if( (brkc.nafl[0][i_] == nph && brkc.nafl[1][i_] == 0) && 
		 Iidx[i] <= 0 )
			goto L_58;
L_57:
		;
		}
	goto L_43;

	/*   If the source is very shallow, we will need to insert some extra
	 *   ray parameter samples into the up-going branches.
	 * */
L_58:
	du = fmin( 1e-5 + (brkc.odep - .4)*2e-5, 1e-5 );
	lp = lpower;
	k = 0;
	for( l = pdec.ka; l >= 1; l-- ){
		l_ = l - 1;
		k = k + 1;
		pdec.ua[nph - 1][k - 1] = Us[nph] - (ipow(l,lp))*du;
		lp = lp - 1;
		pdec.taua[nph - 1][k - 1] = 0e0;
		j = 1;
		if( is < 2 )
			goto L_54;
		for( i = 2; i <= is; i++ ){
			i_ = i - 1;
			tauint( pdec.ua[nph - 1][k - 1], umdc.pm[nph - 1][j - 1], 
			 umdc.pm[nph - 1][i_], umdc.zm[nph - 1][j - 1], umdc.zm[nph - 1][i_], 
			 &ttau, &tx );
			pdec.taua[nph - 1][k - 1] = pdec.taua[nph - 1][k - 1] + 
			 ttau;
			j = i;
			}
L_54:
		if( fabs( umdc.zm[nph - 1][is - 1] - brkc.zs ) <= dtol )
			goto L_56;
		/*   Unless the source is right on a sample slowness, one more partial
		 *   integral is needed. */
		tauint( pdec.ua[nph - 1][k - 1], umdc.pm[nph - 1][is - 1], 
		 Us[nph], umdc.zm[nph - 1][is - 1], brkc.zs, &ttau, &tx );
		pdec.taua[nph - 1][k - 1] = pdec.taua[nph - 1][k - 1] + ttau;
L_56:
		;
		}
	goto L_43;

	/*   Construct tau for all branches.
	 * */
L_1:
	mu = mu + 1;
L_43:
	j = 1;
	for( i = 1; i <= brkc.nseg; i++ ){
		i_ = i - 1;
		if( !Segmsk[i] )
			goto L_20;
		if( (Iidx[i] > 0 || labs( brkc.nafl[0][i_] ) != nph) || (Msrc[nph] <= 
		 0 && brkc.nafl[0][i_] > 0) )
			goto L_20;

		iph = brkc.nafl[1][i_];
		kph = brkc.nafl[2][i_];
		/*   Handle up-going P and S. */
		if( iph <= 0 )
			iph = nph;
		if( kph <= 0 )
			kph = nph;
		sgn = isign( 1, brkc.nafl[0][i_] );
		i1 = brkc.indx[0][i_];
		i2 = brkc.indx[1][i_];
		m = 1;
		for( k = i1; k <= i2; k++ ){
			k_ = k - 1;
			if( Pt[k] > umin )
				goto L_22;
L_23:
			if( fabs( Pt[k] - brkc.pu[nph - 1][m - 1] ) <= dtol )
				goto L_21;
			m = m + 1;
			goto L_23;
L_21:
			tabc.tau[k_][0] = Taut[k] + sgn*Tauc[m];
			}
		k = i2;
		goto L_24;
L_22:
		if( fabs( Pt[k - 1] - umin ) <= dtol )
			k = k - 1;
		brkc.ki = brkc.ki + 1;
		Kk[brkc.ki] = k;
		Pk[brkc.ki] = Pt[k];
		Pt[k] = umin;
		fac = brkc.fcs[0][i_];
		tabc.tau[k - 1][0] = fac*(Tauus1[iph] + Tauus2[iph] + Tauus1[kph] + 
		 Tauus2[kph]) + sgn*Tauus1[nph];
L_24:
		m = 1;
L_26:
		if( tabc.jndx[0][j - 1] >= brkc.indx[0][i_] )
			goto L_25;
		j = j + 1;
		goto L_26;
L_25:
		tabc.jndx[1][j - 1] = min( Jidx[j], k );
		if( tabc.jndx[0][j - 1] < tabc.jndx[1][j - 1] )
			goto L_37;
		tabc.jndx[1][j - 1] = -1;
		goto L_20;
L_37:
		for( l = 1; l <= 2; l++ ){
			l_ = l - 1;
L_28:
			if( fabs( brkc.pux[nph - 1][m - 1] - brkc.px[l_][j - 1] ) <= 
			 dtol )
				goto L_27;
			if( m >= mu )
				goto L_29;
			m = m + 1;
			goto L_28;
L_27:
			tabc.xbrn[l_][j - 1] = brkc.xt[l_][j - 1] + sgn*Xc[m];
			goto L_30;
L_29:
			tabc.xbrn[l_][j - 1] = fac*(Xus1[iph] + Xus2[iph] + Xus1[kph] + 
			 Xus2[kph]) + sgn*Xus1[nph];
L_30:
			;
			}
		if( j >= brkc.nbrn )
			goto L_20;
		j = j + 1;
		if( tabc.jndx[0][j - 1] <= k )
			goto L_25;
L_20:
		;
		}
	return;
} /* end of function */

void /*FUNCTION*/ depset(dep, usrc)
double dep;
float usrc[];
{
	static int dop, dos;
	/* int _i, _r; */
  int _i;
	static int i, i_, ind, int_, j, k, nph;
	static float rdep;
	static int _aini = 1;

	int *const Iidx = &brkc.iidx[0] - 1;
	int *const Int0 = &brkc.int0[0] - 1;
	/* int *const Isrc = &brkc.isrc[0] - 1; */
	int *const Jidx = &brkc.jidx[0] - 1;
	int *const Kk = &brkc.kk[0] - 1;
	/* int *const Km = &brkc.km[0] - 1; */
	/* int *const Ku = &brkc.ku[0] - 1; */
	int *const Msrc = &brkc.msrc[0] - 1;
	/* int *const Mt = &umdc.mt[0] - 1; */
	double *const Pk = &brkc.pk[0] - 1;
	/* int *const Prnt = &prtflc.prnt[0] - 1; */
	double *const Pt = &tabc.pt[0] - 1;
	int *const Segmsk = &prtflc.segmsk[0] - 1;
	/* double *const Tauc = &brkc.tauc[0] - 1; */
	/* double *const Taut = &brkc.taut[0] - 1; */
	double *const Us = &tabc.us[0] - 1;
	float *const Usrc = &usrc[0] - 1;
	/* double *const Xc = &brkc.xc[0] - 1; */

/*
	static F77FMT _fmts[] = {
	 100, "(/1x,'Depth =',f7.2/)",
	 0L,"" };
*/
	if( _aini ){ /* Do 1 TIME INITIALIZATIONS! */
		for(_i=0L; _i < (int)(sizeof(prtflc.segmsk)/sizeof(int)); _i++)
			prtflc.segmsk[_i] = TRUE;
		for(_i=0L; _i < (int)(sizeof(prtflc.prnt)/sizeof(int)); _i++)
			prtflc.prnt[_i] = FALSE;
		_aini = TRUE;
	}


	if( fmax( dep, .011 ) != brkc.odep )
		goto L_1;
	dop = FALSE;
	dos = FALSE;
	for( i = 1; i <= brkc.nseg; i++ ){
		i_ = i - 1;
		if( !Segmsk[i] || Iidx[i] > 0 )
			goto L_2;
		if( labs( brkc.nafl[0][i_] ) <= 1 )
			dop = TRUE;
		if( labs( brkc.nafl[0][i_] ) >= 2 )
			dos = TRUE;
L_2:
		;
		}
	if( !dop && !dos ) {
		return;
    }
	goto L_3;

L_1:
	brkc.nph0 = 0;
	Int0[1] = 0;
	Int0[2] = 0;
	tabc.mbr1 = brkc.nbrn + 1;
	tabc.mbr2 = 0;
	dop = FALSE;
	dos = FALSE;
	for( i = 1; i <= brkc.nseg; i++ ){
		i_ = i - 1;
		if( !Segmsk[i] )
			goto L_4;
		if( labs( brkc.nafl[0][i_] ) <= 1 )
			dop = TRUE;
		if( labs( brkc.nafl[0][i_] ) >= 2 )
			dos = TRUE;
L_4:
		;
		}
	for( i = 1; i <= brkc.nseg; i++ ){
		i_ = i - 1;
		if( brkc.nafl[1][i_] > 0 || brkc.odep < 0. )
			goto L_5;
		ind = brkc.nafl[0][i_];
		k = 0;
		for( j = brkc.indx[0][i_]; j <= brkc.indx[1][i_]; j++ ){
			k = k + 1;
			Pt[j] = brkc.tp[ind - 1][k - 1];
			}
L_5:
		Iidx[i] = -1;
		}
	for( i = 1; i <= brkc.nbrn; i++ ){
		i_ = i - 1;
		tabc.jndx[1][i_] = -1;
		}
	if( brkc.ki <= 0 )
		goto L_7;
	for( i = 1; i <= brkc.ki; i++ ){
		i_ = i - 1;
		j = Kk[i];
		Pt[j] = Pk[i];
		}
	brkc.ki = 0;
	/*   Sample the model at the source depth. */
L_7:
	brkc.odep = fmax( dep, .011 );
	rdep = dep;
	if( rdep < .011 )
		rdep = 0.;
	brkc.zs = fmin( log( fmax( 1. - rdep*tabc.xn, 1e-30 ) ), 0. );
	tabc.hn = 1./(tabc.pn*(1. - rdep*tabc.xn));
/*
	if( Prnt[1] || Prnt[2] )
		{
		writef( 10, FMTR(100), "%g \n", dep );
		}
*/

L_3:
	if( brkc.nph0 > 1 )
		goto L_12;
	if( dop )
		depcor( 1 );
	if( dos )
		depcor( 2 );
	goto L_14;
L_12:
	if( dos )
		depcor( 2 );
	if( dop )
		depcor( 1 );

	/*   Interpolate all tau branches.
	 * */
L_14:
	j = 1;
	for( i = 1; i <= brkc.nseg; i++ ){
		i_ = i - 1;
		if( !Segmsk[i] ) {
			goto L_9;
        }
		nph = labs( brkc.nafl[0][i_] );
		if( Iidx[i] > 0 || (Msrc[nph] <= 0 && brkc.nafl[0][i_] > 0) )
			goto L_9;
		Iidx[i] = 1;
		if( brkc.nafl[1][i_] <= 0 )
			int_ = brkc.nafl[0][i_];
		if( brkc.nafl[1][i_] > 0 && brkc.nafl[1][i_] == labs( brkc.nafl[0][i_] ) )
			int_ = brkc.nafl[1][i_] + 2;
		if( brkc.nafl[1][i_] > 0 && brkc.nafl[1][i_] != labs( brkc.nafl[0][i_] ) )
			int_ = labs( brkc.nafl[0][i_] ) + 4;
		if( brkc.nafl[1][i_] > 0 && brkc.nafl[1][i_] != brkc.nafl[2][i_] )
			int_ = brkc.nafl[1][i_] + 6;
L_11:
		if( tabc.jndx[0][j - 1] >= brkc.indx[0][i_] )
			goto L_10;
		j = j + 1;
		goto L_11;
L_10:
		tabc.idel[2][j - 1] = brkc.nafl[0][i_];
		spfit( j, int_ );
		tabc.mbr1 = min( tabc.mbr1, j );
		tabc.mbr2 = max( tabc.mbr2, j );
		if( j >= brkc.nbrn )
			goto L_9;
		j = j + 1;
		if( Jidx[j] <= brkc.indx[1][i_] && tabc.jndx[1][j - 1] > 0 )
			goto L_10;
L_9:
		;
		}
	Usrc[1] = Us[1]/tabc.pn;
	Usrc[2] = Us[2]/tabc.pn;

	return;
} /* end of function */

#include <stdarg.h>
void
stop(int value, char *fmt, ...) {
    va_list ap;
    if(!value) {
        fprintf(stderr, "error: ");
        va_start(ap, fmt);
        vfprintf(stderr, fmt, ap);
        va_end(ap);
        fprintf(stderr, "\n");
        exit(-3);
    }
}

void /*FUNCTION*/ findtt(jb, x0, max_, n, tt, dtdd, dtdh, dddp, phnm, 
	 phnm_s)
int jb;
double x0[];
int max_, *n;
float tt[], dtdd[], dtdh[], dddp[];
char *phnm;   int phnm_s;
{
#define PHNM(I_,J_)	(phnm+(I_)*(phnm_s)+(J_))
	/* static char msg[68]; */
	static int i, ie, ij, in, is, j, jj, le, ln, 
	 nph;
	static float dp0, dpn, dsgn, hsgn;
	static double arg, delp, dp, dps, p0, p1, ps, x;
	static double tol = 3e-6;
	static double deps = 1e-10;

	float *const Dddp = &dddp[0] - 1;
	float *const Dtdd = &dtdd[0] - 1;
	float *const Dtdh = &dtdh[0] - 1;
	double *const Pt = &tabc.pt[0] - 1;
	float *const Tt = &tt[0] - 1;
	double *const Us = &tabc.us[0] - 1;
	double *const X0 = &x0[0] - 1;

    char *p;

/*
	static F77FMT _fmts[] = {
	 100, "('Bad sqrt argument:',1pd11.2,'.')",
	 101, "('Failed to find phase:  ',a,f8.1,4f7.4)",
	 102, "('More than',i3,' arrivals found.')",
	 0L,"" };
*/
	nph = labs( tabc.idel[2][jb - 1] );
	hsgn = isign( 1, tabc.idel[2][jb - 1] )*tabc.hn;
	dsgn = powi(-1.,tabc.idel[0][jb - 1])*tabc.dn;
	dpn = -1./tabc.tn;
	for( ij = tabc.idel[0][jb - 1]; ij <= tabc.idel[1][jb - 1]; ij++ ){
		x = X0[ij];
		dsgn = -dsgn;
		if( x < tabc.xbrn[0][jb - 1] || x > tabc.xbrn[1][jb - 1] )
			goto L_12;
		j = tabc.jndx[0][jb - 1];
		is = j + 1;
		ie = tabc.jndx[1][jb - 1];
		for( i = is; i <= ie; i++ ){
            stop(j-1 < JOUT, "tabc.xlim[%d >= %d]", j-1, JOUT);
			if( x <= tabc.xlim[j - 1][0] || x > tabc.xlim[j - 1][1] )
				goto L_8;
			le = *n;
            stop(ie < JOUT, "tabc.pt[%d >= %d]", ie, JOUT);
            stop(j  < JOUT, "tabc.pt[%d >= %d]", j, JOUT);
            stop(i  < JOUT, "tabc.pt[%d >= %d]", i, JOUT);
			p0 = Pt[ie] - Pt[j];
			p1 = Pt[ie] - Pt[i];
			delp = fmax( tol*(Pt[i] - Pt[j]), 1e-3 );
            stop(j-1  < JOUT, "tabc.tau[%d >= %d]", j-1, JOUT);
			if( fabs( tabc.tau[j - 1][2] ) > 1e-30 )
				goto L_2;
			dps = (x - tabc.tau[j - 1][1])/(1.5e0*tabc.tau[j - 1][3]);
			dp = sign( dps*dps, dps );
			dp0 = dp;
			if( dp < p1 - delp || dp > p0 + delp )
				goto L_9;
			if( *n >= max_ )
				goto L_13;
			*n = *n + 1;
			ps = Pt[ie] - dp;
            stop(*n <= MAX_PHASES , "Tt[%d > %d] (%d)", *n, MAX_PHASES, __LINE__);
            stop(nph <= 2, "Us[%d > %d]", nph, 2);
			Tt[*n] = tabc.tn*(tabc.tau[j - 1][0] + dp*(tabc.tau[j - 1][1] + 
			 dps*tabc.tau[j - 1][3]) + ps*x);
			Dtdd[*n] = dsgn*ps;
			Dtdh[*n] = hsgn*sqrt( fabs( (float)( Us[nph]*Us[nph] - 
			 ps*ps ) ) );
			Dddp[*n] = dpn*.75e0*tabc.tau[j - 1][3]/fmax( fabs( dps ), 
			 deps );
            stop(jb-1 < JBRN, "pcdc.phcd[%d >= %d]", jb-1, JBRN);
            stop(*n < MAX_PHASES , "PHMN[%d >= %d]", *n, MAX_PHASES);
			fstrncpy( PHNM(*n - 1,0), phnm_s-1, pcdc.phcd[jb - 1],
                                                     strlen(pcdc.phcd[jb - 1]));

            in = 0;
            if ( (p = (strstr(PHNM(*n - 1,0), "ab" ))) != 0) {
                in -= p - PHNM(*n - 1,0) + 1;
            }

/*			in = istrstr( PHNM(*n - 1,0), "ab" ); */
			if( in <= 0 )
				goto L_8;
            stop(jb-1 < JBRN, "tabc.xbrn[%d >= %d]", jb-1, JBRN);
			if( ps <= tabc.xbrn[2][jb - 1] ) {
				subscpy( PHNM(*n - 1,0), in - 1, -1, phnm_s - 1, "bc");
            }
			goto L_8;
L_2:
			for( jj = 1; jj <= 2; jj++ ){
				switch( jj ){
					case 1: goto L_5;
					case 2: goto L_6;
					}
L_5:
                stop(j-1 < JOUT, "tabc.tau[%d >= %d (%d)", j-1, JOUT, __LINE__);
				arg = 9e0*tabc.tau[j - 1][3]*tabc.tau[j - 1][3] + 
				 32e0*tabc.tau[j - 1][2]*(x - tabc.tau[j - 1][1]);
				if( arg >= 0e0 )
					goto L_3;
                                fprintf(stdout,"Bad sqrt argument: %11.2g\n",arg);
L_3:
                stop(j-1 < JOUT, "tabc.tau[%d >= %d] (%d)", j-1, JOUT, __LINE__);
				dps = -(3e0*tabc.tau[j - 1][3] + sign( sqrt( fabs( arg ) ), 
				 tabc.tau[j - 1][3] ))/(8e0*tabc.tau[j - 1][2]);
				dp = sign( dps*dps, dps );
				dp0 = dp;
				goto L_7;
L_6:
                stop(j-1 < JOUT, "tabc.tau[%d >= %d] (%d)", j-1, JOUT, __LINE__);
				dps = (tabc.tau[j - 1][1] - x)/(2e0*tabc.tau[j - 1][2]*
				 dps);
				dp = sign( dps*dps, dps );
L_7:
				if( dp < p1 - delp || dp > p0 + delp )
					goto L_4;
				if( *n >= max_ )
					goto L_13;
				*n = *n + 1;
				ps = Pt[ie] - dp;
                stop(j-1 < JOUT, "tabc.tau[%d >= %d] (%d)", j-1, JOUT, __LINE__);
                stop(*n <= MAX_PHASES, "Tt[%d > %d] (%d)", *n, MAX_PHASES, __LINE__);
                stop(nph <= 2, "Us[%d > %d]", nph, 2);
                stop(jb-1 < JBRN, "pcdc.phcd[%d >= %d] (%d)", jb-1, JBRN, __LINE__);
				Tt[*n] = tabc.tn*(tabc.tau[j - 1][0] + dp*(tabc.tau[j - 1][1] + 
				 dp*tabc.tau[j - 1][2] + dps*tabc.tau[j - 1][3]) + 
				 ps*x);
				Dtdd[*n] = dsgn*ps;
				Dtdh[*n] = hsgn*sqrt( fabs( (float)( Us[nph]*Us[nph] - 
				 ps*ps ) ) );
				Dddp[*n] = dpn*(2e0*tabc.tau[j - 1][2] + .75e0*tabc.tau[j - 1][3]/
				 fmax( fabs( dps ), deps ));
				fstrncpy( PHNM(*n - 1,0), phnm_s-1, pcdc.phcd[jb - 1],
                                                             strlen(pcdc.phcd[jb - 1]));

                in = 0;
                if ( ( p = (strstr(PHNM(*n - 1,0), "ab" ))) != 0) {
                    in = p - PHNM(*n - 1,0) +1;
                } 
/*				in = istrstr( PHNM(*n - 1,0), "ab" ); */
				if( in <= 0 )
					goto L_4;
                stop(jb-1 < JBRN, "tabc.xbrn[%d >= %d] (%d)", jb-1, JBRN, __LINE__);
				if( ps <= tabc.xbrn[2][jb - 1] ) {
					subscpy( PHNM(*n - 1,0), in - 1, -1, phnm_s - 1, "bc" );
                }
L_4:
				;
				}
L_9:
			if( *n > le )
				goto L_8;
                        fprintf(stdout,"Failed to find phase: %s%8.1f%7.4f%7.4f%7.4f%7.4f\n",
			 pcdc.phcd[jb - 1], x, dp0, dp, p1, p0 );
L_8:
			j = i;
			}

L_12:
		if( x < tabc.dbrn[0][jb - 1] || x > tabc.dbrn[1][jb - 1] )
			goto L_10;
		if( *n >= max_ )
			goto L_13;
		j = tabc.jndx[0][jb - 1];
		i = tabc.jndx[1][jb - 1];
		dp = Pt[i] - Pt[j];
		dps = sqrt( fabs( dp ) );
		*n = *n + 1;
		Tt[*n] = tabc.tn*(tabc.tau[j - 1][0] + dp*(tabc.tau[j - 1][1] + 
		 dp*tabc.tau[j - 1][2] + dps*tabc.tau[j - 1][3]) + Pt[j]*x);
		Dtdd[*n] = dsgn*(float)( Pt[j] );
		Dtdh[*n] = hsgn*sqrt( fabs( (float)( Us[nph]*Us[nph] - Pt[j]*
		 Pt[j] ) ) );
		Dddp[*n] = dpn*(2e0*tabc.tau[j - 1][2] + .75e0*tabc.tau[j - 1][3]/
		 fmax( dps, deps ));
/*		ln = istrstr( pcdc.phcd[jb - 1], " " ) - 1; */

        if ( (p = strstr(pcdc.phcd[jb - 1]," ")) != 0) {
            ln = p - pcdc.phcd[jb - 1] +1;
        }

		if( ln <= 0 )
			ln = (8);
                fstrncpy( PHNM(*n - 1,0), phnm_s-1, pcdc.phcd[jb - 1], min(ln,8));
                fstrncpy( PHNM(*n - 1,0)+min(ln,8), phnm_s-1-min(ln,8),
                                                "diff", 4);
L_10:
		;
		}
	return;
L_13:
        fprintf(stdout,"More than %3d arrivals found.\n",max_);

	return;

#undef	PHNM
} /* end of function */

void /*FUNCTION*/ fitspl(i1, i2, tau, x1, xn, coef)
int i1, i2;
double tau[][4], x1, xn, coef[][5];
{
	static int i, i_, ie, is, j, j_, n, n1;
	static double a[MAX_PHASES][2], alr, ap[3], b[MAX_PHASES], gn;

	double *const Ap = &ap[0] - 1;
	double *const B = &b[0] - 1;



	/* $$$$$ calls only library routines $$$$$
	 *
	 *   Given ray parameter grid p;i (p sub i), i=1,2,...,n, corresponding
	 *   tau;i values, and x;1 and x;n (x;i = -dtau/dp|p;i); tauspl finds
	 *   interpolation I such that:  tau(p) = a;1,i + Dp * a;2,i + Dp**2 *
	 *   a;3,i + Dp**(3/2) * a;4,i where Dp = p;n - p and p;i <= p < p;i+1.
	 *   Interpolation I has the following properties:  1) x;1, x;n, and
	 *   tau;i, i=1,2,...,n are fit exactly, 2) the first and second
	 *   derivitives with respect to p are continuous everywhere, and
	 *   3) because of the paramaterization d**2 tau/dp**2|p;n is infinite.
	 *   Thus, interpolation I models the asymptotic behavior of tau(p)
	 *   when tau(p;n) is a branch end due to a discontinuity in the
	 *   velocity model.  Note that array a must be dimensioned at least
	 *   a(4,n) though the interpolation coefficients will be returned in
	 *   the first n-1 columns.  The remaining column is used as scratch
	 *   space and returned as all zeros.  Programmed on 16 August 1982 by
	 *   R. Buland.
	 * */


        if ( (i2-i1) < 0 ) return;
        if ( (i2-i1) == 0 ) {
 	  tau[i1 - 1][1] = x1;
	  return;

	}

/* (i2-i1) > 0 case   */
/* L_2: */
	n = 0;
	for( i = i1; i <= i2; i++ ){
		i_ = i - 1;
		n = n + 1;
		B[n] = tau[i_][0];
		for( j = 1; j <= 2; j++ ){
			j_ = j - 1;
			a[n - 1][j_] = coef[i_][j_];
			}
		}
	for( j = 1; j <= 3; j++ ){
		j_ = j - 1;
		Ap[j] = coef[i2 - 1][j_ + 2];
		}
	n1 = n - 1;

	/*   Arrays ap(*,1), a, and ap(*,2) comprise n+2 x n+2 penta-diagonal
	 *   matrix A.  Let x1, tau, and xn comprise corresponding n+2 vector b.
	 *   Then, A * g = b, may be solved for n+2 vector g such that
	 *   interpolation I is given by I(p) = sum(i=0,n+1) g;i * G;i(p).
	 *
	 *   Eliminate the lower triangular portion of A to form A'.  A
	 *   corresponding transformation applied to vector b is stored in
	 *   a(4,*). */
	alr = a[0][0]/coef[i1 - 1][2];
	a[0][0] = 1e0 - coef[i1 - 1][3]*alr;
	a[0][1] = a[0][1] - coef[i1 - 1][4]*alr;
	B[1] = B[1] - x1*alr;
	j = 1;
	for( i = 2; i <= n; i++ ){
		i_ = i - 1;
		alr = a[i_][0]/a[j - 1][0];
		a[i_][0] = 1e0 - a[j - 1][1]*alr;
		B[i] = B[i] - B[j]*alr;
		j = i;
		}
	alr = Ap[1]/a[n1 - 1][0];
	Ap[2] = Ap[2] - a[n1 - 1][1]*alr;
	gn = xn - B[n1]*alr;
	alr = Ap[2]/a[n - 1][0];
	/*   Back solve the upper triangular portion of A' for coefficients g;i.
	 *   When finished, storage g(2), a(4,*), g(5) will comprise vector g. */
	gn = (gn - B[n]*alr)/(Ap[3] - a[n - 1][1]*alr);
	B[n] = (B[n] - gn*a[n - 1][1])/a[n - 1][0];
	j = n;
	for( i = n1; i >= 1; i-- ){
		i_ = i - 1;
		B[i] = (B[i] - B[j]*a[i_][1])/a[i_][0];
		j = i;
		}
	//g1 = (x1 - coef[i1 - 1][3]*B[1] - coef[i1 - 1][4]*B[2])/coef[i1 - 1][2];

	tau[i1 - 1][1] = x1;
	is = i1 + 1;
	ie = i2 - 1;
	j = 1;
	for( i = is; i <= ie; i++ ){
		i_ = i - 1;
		j = j + 1;
		tau[i_][1] = coef[i_][2]*B[j - 1] + coef[i_][3]*B[j] + coef[i_][4]*
		 B[j + 1];
		}
	tau[i2 - 1][1] = xn;
	return;
} /* end of function */

int /*FUNCTION*/ iupcor(phnm, dtdd, xcor, tcor)
char *phnm;
double dtdd;
float *xcor, *tcor;
{
	static int i, ie, is, iupcor_v, j, jb, jp_, js_;
	static double dp, dps, ps, x;
	static float oldep = -1.;
	static int jp = 0;
	static int js = 0;
	static double cn = 57.295779e0;

	/* int *const Iidx = &brkc.iidx[0] - 1; */
	/* int *const Int0 = &brkc.int0[0] - 1; */
	/* int *const Isrc = &brkc.isrc[0] - 1; */
	/* int *const Jidx = &brkc.jidx[0] - 1; */
	/* int *const Kk = &brkc.kk[0] - 1; */
	/* int *const Km = &brkc.km[0] - 1; */
	/* int *const Ku = &brkc.ku[0] - 1; */
	/* int *const Msrc = &brkc.msrc[0] - 1; */
	/* double *const Pk = &brkc.pk[0] - 1; */
	double *const Pt = &tabc.pt[0] - 1;
	/* double *const Tauc = &brkc.tauc[0] - 1; */
	/* double *const Taut = &brkc.taut[0] - 1; */
	/* double *const Us = &tabc.us[0] - 1; */
	/* double *const Xc = &brkc.xc[0] - 1; */



	iupcor_v = 1;
	if( oldep == brkc.odep )
		goto L_1;
	oldep = brkc.odep;
	/*   Find the upgoing P branch. */
	for( jp = tabc.mbr1; jp <= tabc.mbr2; jp++ ){
		jp_ = jp - 1;
		if( (((strcmp(pcdc.phcd[jp_],"Pg      ") == 0 || strcmp(pcdc.phcd[jp_]
		 ,"Pb      ") == 0) || strcmp(pcdc.phcd[jp_],"Pn      ") == 
		 0) || strcmp(pcdc.phcd[jp_],"P       ") == 0) && tabc.xbrn[0][jp_] <= 
		 0e0 )
			goto L_3;
		}
	jp = 0;
	/*   Find the upgoing S branch. */
L_3:
	for( js = tabc.mbr1; js <= tabc.mbr2; js++ ){
		js_ = js - 1;
		if( (((strcmp(pcdc.phcd[js_],"Sg      ") == 0 || strcmp(pcdc.phcd[js_]
		 ,"Sb      ") == 0) || strcmp(pcdc.phcd[js_],"Sn      ") == 
		 0) || strcmp(pcdc.phcd[js_],"S       ") == 0) && tabc.xbrn[0][js_] <= 
		 0e0 )
			goto L_1;
		}
	js = 0;

L_1:
	if( memcmp(phnm,"P",1) != 0 && memcmp(phnm,"p",1) != 0 )
		goto L_5;
	jb = jp;

        if ( jb <= 0 ) goto L_14;
        else goto L_6;

L_5:
	if( memcmp(phnm,"S",1) != 0 && memcmp(phnm,"s",1) != 0 )
		goto L_13;
	jb = js;

        if ( jb <= 0 ) goto L_14;
        else goto L_6;

L_6:
	is = tabc.jndx[0][jb - 1] + 1;
	ie = tabc.jndx[1][jb - 1];
	ps = fabs( dtdd )/tabc.dn;
	if( ps < Pt[is - 1] || ps > Pt[ie] )
		goto L_13;
	for( i = is; i <= ie; i++ ){
		if( ps <= Pt[i] )
			goto L_8;
		}
	goto L_13;

L_8:
	j = i - 1;
	dp = Pt[ie] - ps;
	dps = sqrt( fabs( dp ) );
	x = tabc.tau[j - 1][1] + 2e0*dp*tabc.tau[j - 1][2] + 1.5e0*dps*
	 tabc.tau[j - 1][3];
	*tcor = tabc.tn*(tabc.tau[j - 1][0] + dp*(tabc.tau[j - 1][1] + 
	 dp*tabc.tau[j - 1][2] + dps*tabc.tau[j - 1][3]) + ps*x);
	*xcor = cn*x;
	return( iupcor_v );

L_13:
	iupcor_v = -1;
L_14:
	*xcor = 0.;
	*tcor = 0.;
	return( iupcor_v );
} /* end of function */

void /*FUNCTION*/ pdecu(i1, i2, x0, x1, xmin, int_, len)
int i1, i2;
double x0, x1, xmin;
int int_, *len;
{
	static int i, i_, ie, is, j, k, m, n;
	static double axm, dx, dx2, h1, h2, hh, rnd, sgn, x, xm, xs;

	double *const Pt = &tabc.pt[0] - 1;
	/* double *const Us = &tabc.us[0] - 1; */



	if( pdec.ua[int_ - 1][0] <= 0e0 )
		goto L_17;
	k = i1 + 1;
	for( i = 1; i <= pdec.ka; i++ ){
		i_ = i - 1;
		Pt[k] = pdec.ua[int_ - 1][i_];
		tabc.tau[k - 1][0] = pdec.taua[int_ - 1][i_];
		k = k + 1;
		}
	Pt[k] = Pt[i2];
	tabc.tau[k - 1][0] = tabc.tau[i2 - 1][0];
	goto L_19;

L_17:
	is = i1 + 1;
	ie = i2 - 1;
	xs = x1;
	for( i = ie; i >= i1; i-- ){
		i_ = i - 1;
		x = xs;
		if( i != i1 )
			goto L_12;
		xs = x0;
		goto L_14;
L_12:
		h1 = Pt[i - 1] - Pt[i];
		h2 = Pt[i + 1] - Pt[i];
		hh = h1*h2*(h1 - h2);
		h1 = h1*h1;
		h2 = -h2*h2;
		xs = -(h2*tabc.tau[i_ - 1][0] - (h2 + h1)*tabc.tau[i_][0] + 
		 h1*tabc.tau[i_ + 1][0])/hh;
L_14:
		if( fabs( x - xs ) <= xmin )
			goto L_15;
		}
	*len = i2;
	return;
L_15:
	ie = i;
	if( fabs( x - xs ) > .75e0*xmin || ie == i2 )
		goto L_16;
	xs = x;
	ie = ie + 1;
L_16:
	n = max( (int)( fabs( xs - x0 )/xmin + .8e0 ), 1 );
	dx = (xs - x0)/n;
	dx2 = fabs( .5e0*dx );
	sgn = sign( 1e0, dx );
	rnd = 0e0;
	if( sgn > 0e0 )
		rnd = 1e0;
	xm = x0 + dx;
	k = i1;
	m = is;
	axm = 1e10;
	for( i = is; i <= ie; i++ ){
		i_ = i - 1;
		if( i < ie )
			goto L_8;
		x = xs;
		goto L_5;
L_8:
		h1 = Pt[i - 1] - Pt[i];
		h2 = Pt[i + 1] - Pt[i];
		hh = h1*h2*(h1 - h2);
		h1 = h1*h1;
		h2 = -h2*h2;
		x = -(h2*tabc.tau[i_ - 1][0] - (h2 + h1)*tabc.tau[i_][0] + 
		 h1*tabc.tau[i_ + 1][0])/hh;
L_5:
		if( sgn*(x - xm) <= dx2 )
			goto L_2;
		if( k < m )
			goto L_3;
		for( j = m; j <= k; j++ ){
			Pt[j] = -1e0;
			}
L_3:
		m = k + 2;
		k = i - 1;
		axm = 1e10;
/* L_7: */
		xm = xm + dx*(int)( (x - xm - dx2)/dx + rnd );
L_2:
		if( fabs( x - xm ) >= axm )
			goto L_1;
		axm = fabs( x - xm );
		k = i - 1;
L_1:
		;
		}
	if( k < m )
		goto L_9;
	for( j = m; j <= k; j++ ){
		Pt[j] = -1e0;
		}
L_9:
	k = i1;
	for( i = is; i <= i2; i++ ){
		i_ = i - 1;
		if( Pt[i] < 0e0 )
			goto L_10;
		k = k + 1;
		Pt[k] = Pt[i];
		tabc.tau[k - 1][0] = tabc.tau[i_][0];
L_10:
		;
		}
L_19:
	*len = k;
	return;
} /* end of function */

void /*FUNCTION*/ r4sort(n, rkey, iptr)
int n;
float rkey[];
int iptr[];
{
	static int i, ib, ij, il[10], it, iu[10], j, k, kk, l, 
	 m;
	static float r, tmpkey;

	int *const Il = &il[0] - 1;
	int *const Iptr = &iptr[0] - 1;
	int *const Iu = &iu[0] - 1;
	float *const Rkey = &rkey[0] - 1;



	/* $$$$$ calls no other routine $$$$$
	 *
	 *   R4sort sorts the n elements of array rkey so that rkey(i),
	 *   i = 1, 2, 3, ..., n are in asending order.  R4sort is a trivial
	 *   modification of ACM algorithm 347:  "An efficient algorithm for
	 *   sorting with minimal storage" by R. C. Singleton.  Array rkey is
	 *   sorted in place in order n*alog2(n) operations.  Coded on
	 *   8 March 1979 by R. Buland.  Modified to handle real*4 data on
	 *   27 September 1983 by R. Buland.
	 * */
	/*   Note:  il and iu implement a stack containing the upper and
	 *   lower limits of subsequences to be sorted independently.  A
	 *   depth of k allows for n<=2**(k+1)-1. */
	if( n <= 0 )
		return;
	for( i = 1; i <= n; i++ ){
		Iptr[i] = i;
		}
	if( n <= 1 )
		return;
	r = .375;
	m = 1;
	i = 1;
	j = n;

	/*   The first section interchanges low element i, middle element ij,
	 *   and high element j so they are in order.
	 * */
L_5:
	if( i >= j )
		goto L_70;
L_10:
	k = i;
	/*   Use a floating point modification, r, of Singleton's bisection
	 *   strategy (suggested by R. Peto in his verification of the
	 *   algorithm for the ACM). */
	if( r > .58984375 )
		goto L_11;
	r = r + .0390625;
	goto L_12;
L_11:
	r = r - .21875;
L_12:
	ij = i + (j - i)*r;
	if( Rkey[Iptr[i]] <= Rkey[Iptr[ij]] )
		goto L_20;
	it = Iptr[ij];
	Iptr[ij] = Iptr[i];
	Iptr[i] = it;
L_20:
	l = j;
	if( Rkey[Iptr[j]] >= Rkey[Iptr[ij]] )
		goto L_39;
	it = Iptr[ij];
	Iptr[ij] = Iptr[j];
	Iptr[j] = it;
	if( Rkey[Iptr[i]] <= Rkey[Iptr[ij]] )
		goto L_39;
	it = Iptr[ij];
	Iptr[ij] = Iptr[i];
	Iptr[i] = it;
L_39:
	tmpkey = Rkey[Iptr[ij]];
	goto L_40;

	/*   The second section continues this process.  K counts up from i and
	 *   l down from j.  Each time the k element is bigger than the ij
	 *   and the l element is less than the ij, then interchange the
	 *   k and l elements.  This continues until k and l meet.
	 * */
L_30:
	it = Iptr[l];
	Iptr[l] = Iptr[k];
	Iptr[k] = it;
L_40:
	l = l - 1;
	if( Rkey[Iptr[l]] > tmpkey )
		goto L_40;
L_50:
	k = k + 1;
	if( Rkey[Iptr[k]] < tmpkey )
		goto L_50;
	if( k <= l )
		goto L_30;

	/*   The third section considers the intervals i to l and k to j.  The
	 *   larger interval is saved on the stack (il and iu) and the smaller
	 *   is remapped into i and j for another shot at section one.
	 * */
	if( l - i <= j - k )
		goto L_60;
	Il[m] = i;
	Iu[m] = l;
	i = k;
	m = m + 1;
	goto L_80;
L_60:
	Il[m] = k;
	Iu[m] = j;
	j = l;
	m = m + 1;
	goto L_80;

	/*   The fourth section pops elements off the stack (into i and j).  If
	 *   necessary control is transfered back to section one for more
	 *   interchange sorting.  If not we fall through to section five.  Note
	 *   that the algorighm exits when the stack is empty.
	 * */
L_70:
	m = m - 1;
	if( m == 0 )
		return;
	i = Il[m];
	j = Iu[m];
L_80:
	if( j - i >= 11 )
		goto L_10;
	if( i == 1 )
		goto L_5;
	i = i - 1;

	/*   The fifth section is the end game.  Final sorting is accomplished
	 *   (within each subsequence popped off the stack) by rippling out
	 *   of order elements down to their proper positions.
	 * */
L_90:
	i = i + 1;
	if( i == j )
		goto L_70;
	if( Rkey[Iptr[i]] <= Rkey[Iptr[i + 1]] )
		goto L_90;
	k = i;
	kk = k + 1;
	ib = Iptr[kk];
L_100:
	Iptr[kk] = Iptr[k];
	kk = k;
	k = k - 1;
	if( Rkey[ib] < Rkey[Iptr[k]] )
		goto L_100;
	Iptr[kk] = ib;
	goto L_90;
} /* end of function */

void /*FUNCTION*/ spfit(jb, int_)
int jb, int_;
{
	static char _c0[2], disc[4];
	static int makgrd, newgrd;
	static int i, i1, i2, i_, is, j, k, mncnt, mxcnt, 
	 nn;
	static double dmn, dmx, dpe, dtau, hm, p0, p1, pe, pe0, pe1, pmn, 
	 scpe0, scpe1, shm, spe0, spe1, tau0, tau1, thm, x0, x1;
	static double dbrnch = 2.5307274e0;
	/* static double cn = 57.295779e0; */
	static double x180 = 3.1415927e0;
	static double x360 = 6.2831853e0;
	static double xmin = 3.92403e-3;
	static double dtol = 1e-6;
	static double ptol = 2e-6;
        char *cattemp;


	/* int *const Iidx = &brkc.iidx[0] - 1; */
	int *const Int0 = &brkc.int0[0] - 1;
	/* int *const Isrc = &brkc.isrc[0] - 1; */
	/* int *const Jidx = &brkc.jidx[0] - 1; */
	/* int *const Kk = &brkc.kk[0] - 1; */
	/* int *const Km = &brkc.km[0] - 1; */
	/* int *const Ku = &brkc.ku[0] - 1; */
	/* int *const Msrc = &brkc.msrc[0] - 1; */
	/* int *const Mt = &umdc.mt[0] - 1; */
	/* double *const Pk = &brkc.pk[0] - 1; */
	/* int *const Prnt = &prtflc.prnt[0] - 1; */
	double *const Pt = &tabc.pt[0] - 1;
	/* int *const Segmsk = &prtflc.segmsk[0] - 1; */
	/* double *const Tauc = &brkc.tauc[0] - 1; */
	/* double *const Taut = &brkc.taut[0] - 1; */
	/* double *const Us = &tabc.us[0] - 1; */
	/* double *const Xc = &brkc.xc[0] - 1; */

/*
	static F77FMT _fmts[] = {
	 100, "(1x,a,i5,f10.6,1p4e10.2,0p2f7.2)",
	 102, "()",
	 101, "(1x,a,2i5,2f8.2,f8.4,2f8.2,4i3,2l2)",
	 103, "(1x,a,2i5,2f8.2,f8.4,16x,4i3,2l2)",
	 0L,"" };
*/
	/*     logical log */
/*
	if( Prnt[1] )
		{
		writef( 10, FMTR(102), "\n" );
		}
*/
	i1 = tabc.jndx[0][jb - 1];
	i2 = tabc.jndx[1][jb - 1];
	if( i2 - i1 > 1 || fabs( Pt[i2] - Pt[i1] ) > ptol )
		goto L_14;
	tabc.jndx[1][jb - 1] = -1;
	return;
L_14:
	newgrd = FALSE;
	makgrd = FALSE;
	if( fabs( brkc.px[1][jb - 1] - Pt[i2] ) > dtol )
		newgrd = TRUE;
	if( !newgrd )
		goto L_10;
	k = ((int_ - 1)%2) + 1;
	if( int_ != Int0[k] )
		makgrd = TRUE;
	if( int_ > 2 )
		goto L_12;
	xmin = tabc.xn*fmin( fmax( 2.*brkc.odep, 2. ), 25. );
	pdecu( i1, i2, tabc.xbrn[0][jb - 1], tabc.xbrn[1][jb - 1], xmin, 
	 int_, &i2 );
	tabc.jndx[1][jb - 1] = i2;
L_12:
	nn = i2 - i1 + 1;
	if( makgrd )
		tauspl( 1, nn, &Pt[i1], (double(*)[5])&brkc.tcoef[k - 1][0][0] );
	fitspl( 1, nn, (double(*)[4])&tabc.tau[i1 - 1][0], tabc.xbrn[0][jb - 1], 
	 tabc.xbrn[1][jb - 1], (double(*)[5])&brkc.tcoef[k - 1][0][0] );
	Int0[k] = int_;
	goto L_11;
L_10:
	fitspl( i1, i2, tabc.tau, tabc.xbrn[0][jb - 1], tabc.xbrn[1][jb - 1], 
	 brkc.coef );
L_11:
	pmn = Pt[i1];
	dmn = tabc.xbrn[0][jb - 1];
	dmx = dmn;
	mxcnt = 0;
	mncnt = 0;
	pe = Pt[i2];
	p1 = Pt[i1];
	tau1 = tabc.tau[i1 - 1][0];
	x1 = tabc.tau[i1 - 1][1];
	pe1 = pe - p1;
	spe1 = sqrt( fabs( pe1 ) );
	scpe1 = pe1*spe1;
	j = i1;
	is = i1 + 1;
	for( i = is; i <= i2; i++ ){
		i_ = i - 1;
		p0 = p1;
		p1 = Pt[i];
		tau0 = tau1;
		tau1 = tabc.tau[i_][0];
		x0 = x1;
		x1 = tabc.tau[i_][1];
		dpe = p0 - p1;
		dtau = tau1 - tau0;
		pe0 = pe1;
		pe1 = pe - p1;
		spe0 = spe1;
		spe1 = sqrt( fabs( pe1 ) );
		scpe0 = scpe1;
		scpe1 = pe1*spe1;
		tabc.tau[j - 1][3] = (2e0*dtau - dpe*(x1 + x0))/(.5e0*(scpe1 - 
		 scpe0) - 1.5e0*spe1*spe0*(spe1 - spe0));
		tabc.tau[j - 1][2] = (dtau - dpe*x0 - (scpe1 + .5e0*scpe0 - 
		 1.5e0*pe1*spe0)*tabc.tau[j - 1][3])/(dpe*dpe);
		tabc.tau[j - 1][1] = (dtau - (pe1*pe1 - pe0*pe0)*tabc.tau[j - 1][2] - 
		 (scpe1 - scpe0)*tabc.tau[j - 1][3])/dpe;
		tabc.tau[j - 1][0] = tau0 - scpe0*tabc.tau[j - 1][3] - pe0*
		 (pe0*tabc.tau[j - 1][2] + tabc.tau[j - 1][1]);
		tabc.xlim[j - 1][0] = fmin( x0, x1 );
		tabc.xlim[j - 1][1] = fmax( x0, x1 );
		if( tabc.xlim[j - 1][0] >= dmn )
			goto L_5;
		dmn = tabc.xlim[j - 1][0];
		pmn = Pt[j];
		if( x1 < x0 )
			pmn = Pt[i];
L_5:
		strcpy( disc, "   " );
		if( fabs( tabc.tau[j - 1][2] ) <= 1e-30 )
			goto L_4;
		shm = -.375e0*tabc.tau[j - 1][3]/tabc.tau[j - 1][2];
		hm = shm*shm;
		if( shm <= 0e0 || (hm <= pe1 || hm >= pe0) )
			goto L_4;
/* L_7: */
		thm = tabc.tau[j - 1][1] + shm*(2e0*shm*tabc.tau[j - 1][2] + 
		 1.5e0*tabc.tau[j - 1][3]);
		tabc.xlim[j - 1][0] = fmin( tabc.xlim[j - 1][0], thm );
		tabc.xlim[j - 1][1] = fmax( tabc.xlim[j - 1][1], thm );
		if( thm >= dmn )
			goto L_6;
		dmn = thm;
		pmn = pe - hm;
L_6:
		strcpy( disc, "max" );
		if( tabc.tau[j - 1][3] < 0e0 )
			strcpy( disc, "min" );
		if( strcmp(disc,"max") == 0 )
			mxcnt = mxcnt + 1;
		if( strcmp(disc,"min") == 0 )
			mncnt = mncnt + 1;
L_4:
/*
		if( Prnt[1] )
			{
			writef( 10, FMTR(100), "%s %d %lg ", disc, j, Pt[j] );
			for( k = 1; k <= 4; k++ ){
				wrtfmt( 10, "%lg ", tabc.tau[j - 1][k - 1] );
				}
			for( k = 1; k <= 2; k++ ){
				wrtfmt( 10, "%lg ", cn*tabc.xlim[j - 1][k - 1] );
				}
			wrtfmt( 10, "\n" );
			ios = UERR(10);
			}
*/
		dmx = fmax( dmx, tabc.xlim[j - 1][1] );
		j = i;
		}
	tabc.xbrn[0][jb - 1] = dmn;
	tabc.xbrn[1][jb - 1] = dmx;
	tabc.xbrn[2][jb - 1] = pmn;
	tabc.idel[0][jb - 1] = 1;
	tabc.idel[1][jb - 1] = 1;
	if( tabc.xbrn[0][jb - 1] > x180 )
		tabc.idel[0][jb - 1] = 2;
	if( tabc.xbrn[1][jb - 1] > x180 )
		tabc.idel[1][jb - 1] = 2;
	if( tabc.xbrn[0][jb - 1] > x360 )
		tabc.idel[0][jb - 1] = 3;
	if( tabc.xbrn[1][jb - 1] > x360 )
		tabc.idel[1][jb - 1] = 3;
	if( int_ > 2 )
		goto L_1;
        _c0[0] = pcdc.phcd[jb - 1][0];
        fstrncpy ( pcdc.phcd[jb - 1], 8, _c0, 1);
	i = jb;
	for( j = 1; j <= brkc.nbrn; j++ ){
		i = (i%brkc.nbrn) + 1;
		if( (memcmp(&pcdc.phcd[i - 1][0],pcdc.phcd[jb - 1],1) == 0
		  && pcdc.phcd[i - 1][1] != 'P') && (pe >= brkc.px[0][i - 1] && 
		 pe <= brkc.px[1][i - 1]) )
			goto L_9;
		}
	goto L_1;
L_9:
	strcpy( pcdc.phcd[jb - 1], pcdc.phcd[i - 1] );
	if( fabs( Pt[i2] - Pt[tabc.jndx[0][i - 1]] ) <= dtol )
		strcpy( pcdc.phcd[jb - 1], pcdc.phcd[i - 2] );
L_1:
/*
	if( Prnt[1] && Prnt[2] )
		{
		writef( 10, FMTR(102), "\n" );
		}
*/
	if( tabc.dbrn[0][jb - 1] <= 0e0 )
		goto L_3;
	tabc.dbrn[0][jb - 1] = dmx;
	tabc.dbrn[1][jb - 1] = dbrnch;
/*
	if( Prnt[2] )
		{
		writef( 10, FMTR(101), "%s ", pcdc.phcd[jb - 1] );
		for( k = 1; k <= 2; k++ ){
			wrtfmt( 10, "%d ", tabc.jndx[k - 1][jb - 1] );
			}
		for( k = 1; k <= 2; k++ ){
			wrtfmt( 10, "%lg ", cn*tabc.xbrn[k - 1][jb - 1] );
			}
		wrtfmt( 10, "%lg ", tabc.xbrn[2][jb - 1] );
		for( k = 1; k <= 2; k++ ){
			wrtfmt( 10, "%lg ", cn*tabc.dbrn[k - 1][jb - 1] );
			}
		for( k = 1; k <= 3; k++ ){
			wrtfmt( 10, "%d ", tabc.idel[k - 1][jb - 1] );
			}
		wrtfmt( 10, "%d %lt %lt \n", int_, newgrd, makgrd );
		ios = UERR(10);
		}
*/
	goto L_15;
L_3:
/*
	if( Prnt[2] )
		{
		writef( 10, FMTR(103), "%s ", pcdc.phcd[jb - 1] );
		for( k = 1; k <= 2; k++ ){
			wrtfmt( 10, "%d ", tabc.jndx[k - 1][jb - 1] );
			}
		for( k = 1; k <= 2; k++ ){
			wrtfmt( 10, "%lg ", cn*tabc.xbrn[k - 1][jb - 1] );
			}
		wrtfmt( 10, "%lg ", tabc.xbrn[2][jb - 1] );
		for( k = 1; k <= 3; k++ ){
			wrtfmt( 10, "%d ", tabc.idel[k - 1][jb - 1] );
			}
		wrtfmt( 10, "%d %lt %lt \n", int_, newgrd, makgrd );
		ios = UERR(10);
		}
*/
L_15:
	if( mxcnt > mncnt || mncnt > mxcnt + 1 )
		{
                cattemp = malloc(21+strlen(pcdc.phcd[jb - 1])+1);
                strcpy(cattemp,"Bad interpolation on ");
                strcat(cattemp,pcdc.phcd[jb - 1]);
		fprintf( stdout, "%s \n", cattemp );
                free(cattemp);
		}

	return;

} /* end of function */

void
check_size(int size1, int size2, int block) {
    if(size1 != size2) {
        fprintf(stderr, "libtau: bad sizes %d != %d block: %d\n", size1, size2, block);
        exit(-1);
    }
}

char *
modelpath(char *name, char *ext, int upper) {
  int i;
  char *tmp, *model;
  tmp = strdup(name);
  for(i = 0; i < strlen(tmp); i++) {
    switch(upper){
    case 0: tmp[i] = toupper(tmp[i]);      break;
    case 1: tmp[i] = tolower(tmp[i]);      break;
    case 2: break;
    }
  }
  asprintf(&model, "%s/tables/%s.%s", sacaux(), tmp, ext);
  FREE(tmp);
  return model;
}

void /*FUNCTION*/ tabin(modnam, modnam_s)
char *modnam;   int modnam_s;
{
  char kfile[MCPFN+1], phdif[6][9], *model, *model_name;
  /* short int _i0; */
  FILE *file;

	int i, i_, idx, ind, ireterror, j, j_, k, l, len2, nasgr, 
	 nerr, nl, nph, nph_;
	/* double tauc[JTSM], xc[JXSM]; */
	static int _aini = 1;

        model = NULL;
        model_name = NULL;
	/* int *const Iidx = &brkc.iidx[0] - 1; */
	/* int *const Int0 = &brkc.int0[0] - 1; */
	/* int *const Isrc = &brkc.isrc[0] - 1; */
	int *const Jidx = &brkc.jidx[0] - 1;
	/* int *const Kk = &brkc.kk[0] - 1; */
	/* int *const Km = &brkc.km[0] - 1; */
	int *const Ku = &brkc.ku[0] - 1;
	int *const Msrc = &brkc.msrc[0] - 1;
	/* int *const Mt = &umdc.mt[0] - 1; */
	/* double *const Pk = &brkc.pk[0] - 1; */
	double *const Pt = &tabc.pt[0] - 1;
	/* double *const Tauc = &brkc.tauc[0] - 1; */
	/* double *const Taut = &brkc.taut[0] - 1; */
	/* double *const Us = &tabc.us[0] - 1; */
	/* double *const Xc = &brkc.xc[0] - 1; */

	if( _aini){ /* Do 1 TIME INITIALIZATIONS! */
          memset(&brkc,   0, sizeof(brkc));
          memset(&pcdc,   0, sizeof(pcdc));
          memset(&prtflc, 0, sizeof(prtflc));
          memset(&umdc,   0, sizeof(umdc));
          memset(&tabc,   0, sizeof(tabc));
          memset(&pdec,   0, sizeof(pdec));

		_aini = TRUE;
	}

        for( idx = 0 ; idx < MCPFN ; idx++ )
            kfile[ idx ] = ' ' ;
        kfile[ MCPFN ] = '\0' ;

	/*     logical log */


	strcpy( phdif[0], "P       " );
	strcpy( phdif[1], "S       " );
	strcpy( phdif[2], "pP      " );
	strcpy( phdif[3], "sP      " );
	strcpy( phdif[4], "pS      " );
	strcpy( phdif[5], "sS      " );
	/*++ */

	{
          int i, n, size1, size2;
	file = NULL;
        model_name = fstrdup(modnam, modnam_s);
        for(j = 0; j < 3; j++) {
          FREE(model);
          model = modelpath(model_name, "hed", j);
          file = fopen(model, "r");
          if(file) {
            break;
          }
        }
        if (file == NULL) {
          fprintf(stdout, "Error: can't find iaspmodel header file \"%s\"\n", model);
          FREE(model);
          exit(-1);
        }
        FREE(model);
	fread(&size1, sizeof(int), 1, file);
		fread(&nasgr    , sizeof(int), 1      , file);
		fread(&nl       , sizeof(int), 1      , file);
		fread(&len2     , sizeof(int), 1      , file);
		fread(&tabc.xn  , sizeof(float   ), 1      , file);
		fread(&tabc.pn  , sizeof(float   ), 1      , file);
		fread(&tabc.tn  , sizeof(float   ), 1      , file);
		fread( umdc.mt  , sizeof(int), 2      , file);
		fread(&brkc.nseg, sizeof(int), 1      , file);
		fread(&brkc.nbrn, sizeof(int), 1      , file);
		fread( brkc.ku  , sizeof(int), 2      , file);
		fread( brkc.km  , sizeof(int), 2      , file);
		fread( brkc.fcs , sizeof(float   ), 3*JSEG , file);
		fread( brkc.nafl, sizeof(int), 3*JSEG , file);
		fread( brkc.indx, sizeof(int), 2*JSEG , file);
		fread( brkc.kndx, sizeof(int), 2*JSEG , file);
	fread(&size2, sizeof(int), 1, file);

    check_size(size1, size2, 1);

	fread(&size1, sizeof(int), 1, file);
		fread( umdc.pm  , sizeof(double  ), 2*JSRC , file);
		fread( umdc.zm  , sizeof(double  ), 2*JSRC , file);
		fread( umdc.ndex, sizeof(int), 2*JSRC , file);
	fread(&size2, sizeof(int), 1, file);

    check_size(size1, size2, 2);

	fread(&size1, sizeof(int), 1, file);
		fread( brkc.pu  , sizeof(double  ), 2*JTSM0, file);
		fread( brkc.pux , sizeof(double  ), 2*JXSM , file);
	fread(&size2, sizeof(int), 1, file);

    check_size(size1, size2, 3);

	fread(&size1, sizeof(int), 1, file);
		for(n = 0; n < JBRN; n++) {
			fread( pcdc.phcd[n], sizeof(char    ), 8*   1, file);
			pcdc.phcd[n][8] = '\0';
			}
		/*
		fread( pcdc.phcd, sizeof(char    ), 9*JBRN , file);
		*/
		fread( brkc.px  , sizeof(double  ), 2*JBRN , file);
		fread( brkc.xt  , sizeof(double  ), 2*JBRN , file);
		fread( tabc.jndx, sizeof(int), 2*JBRN , file);
	fread(&size2, sizeof(int), 1, file);

    check_size(size1, size2, 4);

	fread(&size1, sizeof(int), 1, file);
		fread( tabc.pt  , sizeof(double  ), 1*JOUT , file);
		fread( brkc.taut, sizeof(double  ), 1*JOUT , file);
	fread(&size2, sizeof(int), 1, file);

    check_size(size1, size2, 5);

	fread(&size1, sizeof(int), 1, file);
		fread( brkc.coef, sizeof(double  ), 5*JOUT , file);
	fread(&size2, sizeof(int), 1, file);

    check_size(size1, size2, 6);	

	fclose(file);
        if(size1 < 0) {
          swap_data = TRUE;
          byteswap(&nasgr, sizeof(int));
          byteswap(&nl, sizeof(int));
          byteswap(&len2, sizeof(int));

          byteswap(&tabc.xn, sizeof(float));
          byteswap(&tabc.pn, sizeof(float));
          byteswap(&tabc.tn, sizeof(float));

          byteswap(&umdc.mt[0], sizeof(int));
          byteswap(&umdc.mt[1], sizeof(int));

          byteswap(&brkc.nseg, sizeof(int));
          byteswap(&brkc.nbrn, sizeof(int));

          byteswap(&brkc.ku[0], sizeof(int));
          byteswap(&brkc.ku[1], sizeof(int));

          byteswap(&brkc.km[0], sizeof(int));
          byteswap(&brkc.km[1], sizeof(int));
          for(i = 0; i < JSEG; i++) {
            byteswap(&brkc.fcs[0][i], sizeof(float));
            byteswap(&brkc.fcs[1][i], sizeof(float));
            byteswap(&brkc.fcs[2][i], sizeof(float));
            byteswap(&brkc.nafl[0][i], sizeof(int));
            byteswap(&brkc.nafl[1][i], sizeof(int));
            byteswap(&brkc.nafl[2][i], sizeof(int));
            byteswap(&brkc.indx[0][i], sizeof(int));          
            byteswap(&brkc.indx[1][i], sizeof(int));          
            byteswap(&brkc.kndx[0][i], sizeof(int));          
            byteswap(&brkc.kndx[1][i], sizeof(int));          
          }

          for(i = 0; i < JSRC; i++) {
            byteswap(&umdc.pm[0][i], sizeof(double));
            byteswap(&umdc.pm[1][i], sizeof(double));
            byteswap(&umdc.zm[0][i], sizeof(double));
            byteswap(&umdc.zm[1][i], sizeof(double));
            byteswap(&umdc.ndex[0][i], sizeof(int));
            byteswap(&umdc.ndex[1][i], sizeof(int));
          }
          for(i = 0; i < JTSM0; i++) {
            byteswap(&brkc.pu[0][i], sizeof(double));
            byteswap(&brkc.pu[1][i], sizeof(double));
          }
          for(i = 0; i < JXSM; i++) {
            byteswap(&brkc.pux[0][i], sizeof(double));
            byteswap(&brkc.pux[1][i], sizeof(double));
          }
          for(i = 0; i < JBRN; i++) {
            byteswap(&brkc.px[0][i], sizeof(double));
            byteswap(&brkc.px[1][i], sizeof(double));
            byteswap(&brkc.xt[0][i], sizeof(double));
            byteswap(&brkc.xt[1][i], sizeof(double));
            byteswap(&tabc.jndx[0][i], sizeof(int));
            byteswap(&tabc.jndx[1][i], sizeof(int));
          }
          for(i = 0; i < JOUT; i++) {
            byteswap(&tabc.pt[i], sizeof(double));
            byteswap(&brkc.taut[i], sizeof(double));
          }
          for(i = 0; i < JOUT; i++) {
            byteswap(&brkc.coef[i][0], sizeof(double));
            byteswap(&brkc.coef[i][1], sizeof(double));
            byteswap(&brkc.coef[i][2], sizeof(double));
            byteswap(&brkc.coef[i][3], sizeof(double));
            byteswap(&brkc.coef[i][4], sizeof(double));
          }
        }
        
	}

        
	{
          
          int j;
          tblfile.file = NULL;
          for(j = 0; j < 3; j++) {
            FREE(model);
            model = modelpath(model_name, "tbl", j);
            tblfile.file = fopen(model, "r");
            if(tblfile.file) {
              break;
            }
          }
          if (tblfile.file == NULL) {
            fprintf(stderr, "Error: can't find iaspmodel table file \"%s\"\n", model);
            exit(-1);
          }
          FREE(model);
          tblfile.nasgr = nasgr;
	}

	for( nph = 1; nph <= 2; nph++ ){
		nph_ = nph - 1;
		brkc.pu[nph_][Ku[nph]] = umdc.pm[nph_][0];
		}

	tabc.tn = 1./tabc.tn;
	tabc.dn = 3.1415927/(180.*tabc.pn*tabc.xn);
	brkc.odep = -1.;
	brkc.ki = 0;
	Msrc[1] = 0;
	Msrc[2] = 0;
	k = 1;
	for( i = 1; i <= brkc.nbrn; i++ ){
		i_ = i - 1;
		Jidx[i] = tabc.jndx[1][i_];
		for( j = 1; j <= 2; j++ ){
			j_ = j - 1;
			tabc.dbrn[j_][i_] = -1e0;
			}
L_8:
		if( tabc.jndx[1][i_] <= brkc.indx[1][k - 1] )
			goto L_7;
		k = k + 1;
		goto L_8;
L_7:
		if( brkc.nafl[1][k - 1] > 0 )
			goto L_9;
		ind = brkc.nafl[0][k - 1];
		l = 0;
		for( j = tabc.jndx[0][i_]; j <= tabc.jndx[1][i_]; j++ ){
			j_ = j - 1;
			l = l + 1;
			brkc.tp[ind - 1][l - 1] = Pt[j];
			}
L_9:
		if( brkc.nafl[0][k - 1] > 0 && (pcdc.phcd[i_][0] == 'P' || 
		 pcdc.phcd[i_][0] == 'S') )
			goto L_3;
		for( j = 1; j <= 6; j++ ){
			j_ = j - 1;
			if( memcmp(pcdc.phcd[i_],phdif[j_],strlen(phdif[j_])) == 0 )
				goto L_6;
			}
		goto L_3;
L_6:
		tabc.dbrn[0][i_] = 1e0;
		strcpy( phdif[j - 1], "        " );
L_3:
		;
		}
	return;
} /* end of function */

void /*FUNCTION*/ tauint(ptk, ptj, pti, zj, zi, tau, x)
double ptk, ptj, pti, zj, zi, *tau, *x;
{
	/* static char msg[72]; */
	static double b, sqb, sqi, sqj, sqk, xx;
/*
	static F77FMT _fmts[] = {
	 100, "('Bad range: ',1p5d12.4)",
	 101, "('Bad tau: ',1p5d12.4)",
	 0L,"" };
*/


	/*   Tauint evaluates the intercept (tau) and distance (x) integrals  for
	 *   the spherical earth assuming that slowness is linear between radii
	 *   for which the model is known.  The partial integrals are performed
	 *   for ray slowness ptk between model radii with slownesses ptj and pti
	 *   with equivalent flat earth depths zj and zi respectively.  The partial
	 *   integrals are returned in tau and x.  Note that ptk, ptj, pti, zj, zi,
	 *   tau, and x are all double precision.
	 * */

	if( fabs( zj - zi ) <= 1e-9 )
		goto L_13;
	if( fabs( ptj - pti ) > 1e-9 )
		goto L_10;
	if( fabs( ptk - pti ) <= 1e-9 )
		goto L_13;
	b = fabs( zj - zi );
	sqj = sqrt( fabs( ptj*ptj - ptk*ptk ) );
	*tau = b*sqj;
	*x = b*ptk/sqj;
	goto L_4;
L_10:
	if( ptk > 1e-9 || pti > 1e-9 )
		goto L_1;
	/*   Handle the straight through ray. */
	*tau = ptj;
	*x = 1.5707963267948966e0;
	goto L_4;
L_1:
	b = ptj - (pti - ptj)/(exp( zi - zj ) - 1e0);
	if( ptk > 1e-9 )
		goto L_2;
	*tau = -(pti - ptj + b*log( pti/ptj ) - b*log( fmax( (ptj - b)*
	 pti/((pti - b)*ptj), 1e-30 ) ));
	*x = 0e0;
	goto L_4;
L_2:
	if( ptk == pti )
		goto L_3;
	if( ptk == ptj )
		goto L_11;
	sqk = ptk*ptk;
	sqi = sqrt( fabs( pti*pti - sqk ) );
	sqj = sqrt( fabs( ptj*ptj - sqk ) );
	sqb = sqrt( fabs( b*b - sqk ) );
	if( sqb > 1e-30 )
		goto L_5;
	xx = 0e0;
	*x = ptk*(sqrt( fabs( (pti + b)/(pti - b) ) ) - sqrt( fabs( (ptj + 
	 b)/(ptj - b) ) ))/b;
	goto L_6;
L_5:
	if( b*b < sqk )
		goto L_7;
	xx = log( fmax( (ptj - b)*(sqb*sqi + b*pti - sqk)/((pti - b)*(sqb*
	 sqj + b*ptj - sqk)), 1e-30 ) );
	*x = ptk*xx/sqb;
	goto L_6;
L_7:
	xx = asin( fmax( fmin( (b*pti - sqk)/(ptk*fabs( pti - b )), 1e0 ), 
	 -1e0 ) ) - asin( fmax( fmin( (b*ptj - sqk)/(ptk*fabs( ptj - b )), 
	 1e0 ), -1e0 ) );
	*x = -ptk*xx/sqb;
L_6:
	*tau = -(sqi - sqj + b*log( (pti + sqi)/(ptj + sqj) ) - sqb*xx);
	goto L_4;
L_3:
	sqk = pti*pti;
	sqj = sqrt( fabs( ptj*ptj - sqk ) );
	sqb = sqrt( fabs( b*b - sqk ) );
	if( b*b < sqk )
		goto L_8;
	xx = log( fmax( (ptj - b)*(b*pti - sqk)/((pti - b)*(sqb*sqj + 
	 b*ptj - sqk)), 1e-30 ) );
	*x = pti*xx/sqb;
	goto L_9;
L_8:
	xx = sign( 1.5707963267948966e0, b - pti ) - asin( fmax( fmin( (b*
	 ptj - sqk)/(pti*fabs( ptj - b )), 1e0 ), -1e0 ) );
	*x = -pti*xx/sqb;
L_9:
	*tau = -(b*log( pti/(ptj + sqj) ) - sqj - sqb*xx);
	goto L_4;
L_11:
	sqk = ptj*ptj;
	sqi = sqrt( fabs( pti*pti - sqk ) );
	sqb = sqrt( fabs( b*b - sqk ) );
	if( b*b < sqk )
		goto L_12;
	xx = log( fmax( (ptj - b)*(sqb*sqi + b*pti - sqk)/((pti - b)*(b*
	 ptj - sqk)), 1e-30 ) );
	*x = ptj*xx/sqb;
	goto L_14;
L_12:
	xx = asin( fmax( fmin( (b*pti - sqk)/(ptj*fabs( pti - b )), 1e0 ), 
	 -1e0 ) ) - sign( 1.5707963267948966e0, b - ptj );
	*x = -ptj*xx/sqb;
L_14:
	*tau = -(b*log( (pti + sqi)/ptj ) + sqi - sqb*xx);

	/*   Handle various error conditions.
	 * */
L_4:
	if( *x >= -1e-10 )
		goto L_15;

        fprintf(stdout,"Bad range: %12.4g%12.4g%12.4g%12.4g%12.4g\n",
                                             ptk, ptj, pti, *tau, *x );
L_15:
	if( *tau >= -1e-10 )
		goto L_16;

        fprintf(stdout,"Bad tau: %12.4g%12.4g%12.4g%12.4g%12.4g\n",
                                          ptk, ptj, pti, *tau, *x );

L_16:
	return;
	/*   Trap null integrals and handle them properly. */
L_13:
	*tau = 0e0;
	*x = 0e0;
	return;
} /* end of function */

void /*FUNCTION*/ tauspl(i1, i2, pt, coef)
int i1, i2;
double pt[], coef[][5];
{
	static int i, is, j, k, l, m, n2;
	static double ali, alr, b1h, b3h, bih, d[4], d1h[4], d3h[4], del[5], 
	 deli[5], dih[4], sdel[5], th0p, th2m, th2p, th3p;

	double *const D = &d[0] - 1;
	double *const D1h = &d1h[0] - 1;
	double *const D3h = &d3h[0] - 1;
	double *const Del = &del[0] - 1;
	double *const Deli = &deli[0] - 1;
	double *const Dih = &dih[0] - 1;
	double *const Pt = &pt[0] - 1;
	double *const Sdel = &sdel[0] - 1;



	/* $$$$$ calls only library routines $$$$$
	 *
	 *   Given ray parameter grid pt;i (pt sub i), i=i1,i1+1,...,i2, tauspl
	 *   determines the i2-i1+3 basis functions for interpolation I such
	 *   that:
	 *
	 *      tau(p) = a;1,i + Dp * a;2,i + Dp**2 * a;3,i + Dp**(3/2) * a;4,i
	 *
	 *   where Dp = pt;n - p, pt;i <= p < pt;i+1, and the a;j,i's are
	 *   interpolation coefficients.  Rather than returning the coefficients,
	 *   a;j,i, which necessarily depend on tau(pt;i), i=i1,i1+1,...,i2 and
	 *   x(pt;i) (= -d tau(p)/d p | pt;i), i=i1,i2, tauspl returns the
	 *   contribution of each basis function and its derivitive at each
	 *   sample.  Each basis function is non-zero at three grid points,
	 *   therefore, each grid point will have contributions (function values
	 *   and derivitives) from three basis functions.  Due to the basis
	 *   function normalization, one of the function values will always be
	 *   one and is not returned in array coef with the other values.
	 *   Rewritten on 23 December 1983 by R. Buland.
	 * */

	n2 = i2 - i1 - 1;
	if( n2 <= -1 )
		return;
	is = i1 + 1;

	/*   To achieve the requisite stability, proceed by constructing basis
	 *   functions G;i, i=0,1,...,n+1.  G;i will be non-zero only on the
	 *   interval [p;i-2,p;i+2] and will be continuous with continuous first
	 *   and second derivitives.  G;i(p;i-2) and G;i(p;i+2) are constrained
	 *   to be zero with zero first and second derivitives.  G;i(p;i) is
	 *   normalized to unity.
	 *
	 *   Set up temporary variables appropriate for G;-1.  Note that to get
	 *   started, the ray parameter grid is extrapolated to yeild p;i, i=-2,
	 *   -1,0,1,...,n. */
	Del[2] = Pt[i2] - Pt[i1] + 3e0*(Pt[is] - Pt[i1]);
	Sdel[2] = sqrt( fabs( Del[2] ) );
	Deli[2] = 1e0/Sdel[2];
	m = 2;
	for( k = 3; k <= 5; k++ ){
		Del[k] = Pt[i2] - Pt[i1] + (5 - k)*(Pt[is] - Pt[i1]);
		Sdel[k] = sqrt( fabs( Del[k] ) );
		Deli[k] = 1e0/Sdel[k];
		D3h[m] = Del[k]*Sdel[k] - Del[m]*Sdel[m];
		D1h[m] = Sdel[k] - Sdel[m];
		Dih[m] = Deli[k] - Deli[m];
		m = k;
		}
	l = i1 - 1;
	if( n2 <= 0 )
		goto L_10;
	/*   Loop over G;i, i=0,1,...,n-3. */
	for( i = 1; i <= n2; i++ ){
		m = 1;
		/*   Update temporary variables for G;i-1. */
		for( k = 2; k <= 5; k++ ){
			Del[m] = Del[k];
			Sdel[m] = Sdel[k];
			Deli[m] = Deli[k];
			if( k >= 5 )
				goto L_3;
			D3h[m] = D3h[k];
			D1h[m] = D1h[k];
			Dih[m] = Dih[k];
L_3:
			m = k;
			}
		l = l + 1;
		Del[5] = Pt[i2] - Pt[l + 1];
		Sdel[5] = sqrt( fabs( Del[5] ) );
		Deli[5] = 1e0/Sdel[5];
		D3h[4] = Del[5]*Sdel[5] - Del[4]*Sdel[4];
		D1h[4] = Sdel[5] - Sdel[4];
		Dih[4] = Deli[5] - Deli[4];
		/*   Construct G;i-1. */
		ali = 1e0/(.125e0*D3h[1] - (.75e0*D1h[1] + .375e0*Dih[1]*Del[3])*
		 Del[3]);
		alr = ali*(.125e0*Del[2]*Sdel[2] - (.75e0*Sdel[2] + .375e0*
		 Del[3]*Deli[2] - Sdel[3])*Del[3]);
		b3h = D3h[2] + alr*D3h[1];
		b1h = D1h[2] + alr*D1h[1];
		bih = Dih[2] + alr*Dih[1];
		th0p = D1h[1]*b3h - D3h[1]*b1h;
		th2p = D1h[3]*b3h - D3h[3]*b1h;
		th3p = D1h[4]*b3h - D3h[4]*b1h;
		th2m = Dih[3]*b3h - D3h[3]*bih;
		/*   The d;i's completely define G;i-1. */
		D[4] = ali*((Dih[1]*b3h - D3h[1]*bih)*th2p - th2m*th0p)/((Dih[4]*
		 b3h - D3h[4]*bih)*th2p - th2m*th3p);
		D[3] = (th0p*ali - th3p*D[4])/th2p;
		D[2] = (D3h[1]*ali - D3h[3]*D[3] - D3h[4]*D[4])/b3h;
		D[1] = alr*D[2] - ali;
		/*   Construct the contributions G;i-1(p;i-2) and G;i-1(p;i).
		 *   G;i-1(p;i-1) need not be constructed as it is normalized to unity. */
		coef[l - 1][0] = (.125e0*Del[5]*Sdel[5] - (.75e0*Sdel[5] + 
		 .375e0*Deli[5]*Del[4] - Sdel[4])*Del[4])*D[4];
		if( i >= 3 )
			coef[l - 3][1] = (.125e0*Del[1]*Sdel[1] - (.75e0*Sdel[1] + 
			 .375e0*Deli[1]*Del[2] - Sdel[2])*Del[2])*D[1];
		/*   Construct the contributions -dG;i-1(p)/dp | p;i-2, p;i-1, and p;i. */
		coef[l - 1][2] = -.75e0*(Sdel[5] + Deli[5]*Del[4] - 2e0*Sdel[4])*
		 D[4];
		if( i >= 2 )
			coef[l - 2][3] = -.75e0*((Sdel[2] + Deli[2]*Del[3] - 2e0*
			 Sdel[3])*D[2] - (D1h[1] + Dih[1]*Del[3])*D[1]);
		if( i >= 3 )
			coef[l - 3][4] = -.75e0*(Sdel[1] + Deli[1]*Del[2] - 2e0*
			 Sdel[2])*D[1];
		}
	/*   Loop over G;i, i=n-2,n-1,n,n+1.  These cases must be handled
	 *   seperately because of the singularities in the second derivitive
	 *   at p;n. */
L_10:
	for( j = 1; j <= 4; j++ ){
		m = 1;
		/*   Update temporary variables for G;i-1. */
		for( k = 2; k <= 5; k++ ){
			Del[m] = Del[k];
			Sdel[m] = Sdel[k];
			Deli[m] = Deli[k];
			if( k >= 5 )
				goto L_5;
			D3h[m] = D3h[k];
			D1h[m] = D1h[k];
			Dih[m] = Dih[k];
L_5:
			m = k;
			}
		l = l + 1;
		Del[5] = 0e0;
		Sdel[5] = 0e0;
		Deli[5] = 0e0;
		/*   Construction of the d;i's is different for each case.  In cases
		 *   G;i, i=n-1,n,n+1, G;i is truncated at p;n to avoid patching across
		 *   the singularity in the second derivitive. */
		if( j < 4 )
			goto L_6;
		/*   For G;n+1 constrain G;n+1(p;n) to be .25. */
		D[1] = 2e0/(Del[1]*Sdel[1]);
		goto L_9;
		/*   For G;i, i=n-2,n-1,n, the condition dG;i(p)/dp|p;i = 0 has been
		 *   substituted for the second derivitive continuity condition that
		 *   can no longer be satisfied. */
L_6:
		alr = (Sdel[2] + Deli[2]*Del[3] - 2e0*Sdel[3])/(D1h[1] + Dih[1]*
		 Del[3]);
		D[2] = 1e0/(.125e0*Del[2]*Sdel[2] - (.75e0*Sdel[2] + .375e0*
		 Deli[2]*Del[3] - Sdel[3])*Del[3] - (.125e0*D3h[1] - (.75e0*
		 D1h[1] + .375e0*Dih[1]*Del[3])*Del[3])*alr);
		D[1] = alr*D[2];

                if ((j-2) < 0) goto L_8;
                if ((j-2) == 0) goto L_7;
                else  goto L_9;

		/*   For G;n-1 constrain G;n-1(p;n) to be .25. */
L_7:
		D[3] = (2e0 + D3h[2]*D[2] + D3h[1]*D[1])/(Del[3]*Sdel[3]);
		goto L_9;
		/*   No additional constraints are required for G;n-2. */
L_8:
		D[3] = -((D3h[2] - D1h[2]*Del[4])*D[2] + (D3h[1] - D1h[1]*
		 Del[4])*D[1])/(D3h[3] - D1h[3]*Del[4]);
		D[4] = (D3h[3]*D[3] + D3h[2]*D[2] + D3h[1]*D[1])/(Del[4]*Sdel[4]);
		/*   Construct the contributions G;i-1(p;i-2) and G;i-1(p;i). */
L_9:
		if( j <= 2 )
			coef[l - 1][0] = (.125e0*Del[3]*Sdel[3] - (.75e0*Sdel[3] + 
			 .375e0*Deli[3]*Del[4] - Sdel[4])*Del[4])*D[3] - (.125e0*
			 D3h[2] - (.75e0*D1h[2] + .375e0*Dih[2]*Del[4])*Del[4])*
			 D[2] - (.125e0*D3h[1] - (.75e0*D1h[1] + .375e0*Dih[1]*
			 Del[4])*Del[4])*D[1];
		if( l - i1 > 1 )
			coef[l - 3][1] = (.125e0*Del[1]*Sdel[1] - (.75e0*Sdel[1] + 
			 .375e0*Deli[1]*Del[2] - Sdel[2])*Del[2])*D[1];
		/*   Construct the contributions -dG;i-1(p)/dp | p;i-2, p;i-1, and p;i. */
		if( j <= 2 )
			coef[l - 1][2] = -.75e0*((Sdel[3] + Deli[3]*Del[4] - 2e0*
			 Sdel[4])*D[3] - (D1h[2] + Dih[2]*Del[4])*D[2] - (D1h[1] + 
			 Dih[1]*Del[4])*D[1]);
		if( j <= 3 && l - i1 > 0 )
			coef[l - 2][3] = 0e0;
		if( l - i1 > 1 )
			coef[l - 3][4] = -.75e0*(Sdel[1] + Deli[1]*Del[2] - 2e0*
			 Sdel[2])*D[1];
		}
	return;
} /* end of function */

void /*FUNCTION*/ trtm(delta, max_, n, tt, dtdd, dtdh, dddp, phnm, 
	 phnm_s)
double delta;
int max_, *n;
float tt[], dtdd[], dtdh[], dddp[];
char *phnm;   int phnm_s;
{
#define PHNM(I_,J_)	(phnm+(I_)*(phnm_s)+(J_))
	static char ctmp[MAX_PHASES][9];
	static int i, iptr[MAX_PHASES], j, j_, k;
	static float tmp[4][MAX_PHASES];
	static double x[3];
	static double cn = .017453292519943296e0;
	static double dtol = 1e-6;
	static float atol = .005;
	static double pi = 3.1415926535897932e0;
	static double pi2 = 6.2831853071795865e0;

	float *const Dddp = &dddp[0] - 1;
	float *const Dtdd = &dtdd[0] - 1;
	float *const Dtdh = &dtdh[0] - 1;
	int *const Iptr = &iptr[0] - 1;
	/* double *const Pt = &tabc.pt[0] - 1; */
	float *const Tt = &tt[0] - 1;
	/* double *const Us = &tabc.us[0] - 1; */
	double *const X = &x[0] - 1;

	*n = 0;
	if( tabc.mbr2 <= 0 )
		return;
	X[1] = fmod( fabs( cn*delta ), pi2 );
	if( X[1] > pi )
		X[1] = pi2 - X[1];
	X[2] = pi2 - X[1];
	X[3] = X[1] + pi2;
	if( fabs( X[1] ) > dtol )
		goto L_9;
	X[1] = dtol;
	X[3] = -10e0;
L_9:
	if( fabs( X[1] - pi ) > dtol )
		goto L_7;
	X[1] = pi - dtol;
	X[2] = -10e0;
L_7:
	for( j = tabc.mbr1; j <= tabc.mbr2; j++ ){
		j_ = j - 1;
		if( tabc.jndx[1][j_] > 0 )
			findtt( j, x, max_, n, (float*)tmp, &tmp[1][0], &tmp[2][0], 
                    &tmp[3][0], (char*)ctmp,9 );
		}

        if ((*n - 1) < 0 ) goto L_3;
        if ((*n - 1) == 0 ) goto L_4;
        else goto L_5;

L_4:
	Iptr[1] = 1;
	goto L_6;
L_5:
	r4sort( *n, (float*)tmp, iptr );
L_6:
	k = 0;
	for( i = 1; i <= *n; i++ ){
		j = Iptr[i];
		if( k <= 0 )
			goto L_8;
		if( memcmp(PHNM(k - 1,0),ctmp[j - 1],strlen(ctmp[j - 1])) == 0 && fabs( Tt[k] - 
		 tmp[0][j - 1] ) <= atol )
			goto L_2;
L_8:
		k = k + 1;
		Tt[k] = tmp[0][j - 1];
		Dtdd[k] = tmp[1][j - 1];
		Dtdh[k] = tmp[2][j - 1];
		Dddp[k] = tmp[3][j - 1];
		fstrncpy( PHNM(k - 1,0), phnm_s-1, ctmp[j - 1],
                                            strlen(ctmp[j - 1]));
L_2:
		;
		}
	*n = k;
L_3:
	return;
#undef	PHNM
} /* end of function */

/* ENTRY translation - externals & entries */
struct {
	double zs;
	int *isrc;
	int nph;
	double uend;
	int js;
	} _umod;
double umod_v;
double zmod_v;

double /*FUNCTION*/ umod(zs, isrc, nph)
double zs;
int isrc[], nph;
{
	_umod.zs = zs;
	_umod.isrc = isrc;
	_umod.nph = nph;
	_umod_( 0 );
	return( umod_v );
}

double /*FUNCTION*/ zmod(uend, js, nph)
double uend;
int js, nph;
{
	_umod.uend = uend;
	_umod.js = js;
	_umod.nph = nph;
	_umod_( 1 );
	return( zmod_v );
}

int
_umod_( _entry_ )
int _entry_;
{
	double zs = _umod.zs;
	int *isrc = _umod.isrc;
	int nph = _umod.nph;
	double uend = _umod.uend;
	int js = _umod.js;
	static char msg[32];
	static int i, i_, j, m1;
	static float dep;
/*	void abort();  */
	static double dtol = 1e-6;

	int *const Isrc = &isrc[0] - 1;
	int *const Mt = &umdc.mt[0] - 1;
	/* double *const Pt = &tabc.pt[0] - 1; */
	/* double *const Us = &tabc.us[0] - 1; */

/*
	static F77FMT _fmts[] = {
	 100, "('Source depth (',f6.1,') too deep.')",
	 0L,"" };
*/
	/* ENTRY Jumps */
	switch( _entry_ ){
		case 1: goto _zmod_;
		case 0:
		default: break;
		}
	/* Main ENTRY Point */

	m1 = Mt[nph];
	for( i = 2; i <= m1; i++ ){
		i_ = i - 1;
		if( umdc.zm[nph - 1][i_] <= zs )
			goto L_2;
		}
	dep = (1e0 - exp( zs ))/tabc.xn;
        sprintf(msg,"Source depth (%6.1f) too deep.",dep);
        fprintf(stdout,"Source depth (%6.1f) too deep.",dep);

	abort;
L_2:
	if( fabs( zs - umdc.zm[nph - 1][i - 1] ) <= dtol && fabs( umdc.zm[nph - 1][i - 1] - 
	 umdc.zm[nph - 1][i] ) <= dtol )
		goto L_3;
	j = i - 1;
	Isrc[nph] = j;
	umod_v = umdc.pm[nph - 1][j - 1] + (umdc.pm[nph - 1][i - 1] - 
	 umdc.pm[nph - 1][j - 1])*(exp( zs - umdc.zm[nph - 1][j - 1] ) - 
	 1e0)/(exp( umdc.zm[nph - 1][i - 1] - umdc.zm[nph - 1][j - 1] ) - 
	 1e0);
	return( umod_v );
L_3:
	Isrc[nph] = i;
	umod_v = umdc.pm[nph - 1][i];
	return( umod_v );

_zmod_:	/*ENTRY Point*/
	i = js + 1;
	zmod_v = umdc.zm[nph - 1][js - 1] + log( fmax( (uend - umdc.pm[nph - 1][js - 1])*
	 (exp( umdc.zm[nph - 1][i - 1] - umdc.zm[nph - 1][js - 1] ) - 
	 1e0)/(umdc.pm[nph - 1][i - 1] - umdc.pm[nph - 1][js - 1]) + 1e0, 
	 1e-30 ) );
	return( umod_v );
} /* end of function */

void /*FUNCTION*/ iaspcl(nerr)
int *nerr;
{
	/* char cmdcd[NCMD][9], cmdlst[LCMD][9], phcd[9], phlst[JSEG][9],  */
	/*  phtmp[9], segcd[JBRN][9]; */
	/* int all, fnd, prnt, segmsk; */
	/* int ncmpt[NCMD][2], nsgpt[JBRN]; */

	/* int *const Iidx = &brkc.iidx[0] - 1; */
	/* int *const Int0 = &brkc.int0[0] - 1; */
	/* int *const Isrc = &brkc.isrc[0] - 1; */
	/* int *const Jidx = &brkc.jidx[0] - 1; */
	/* int *const Kk = &brkc.kk[0] - 1; */
	/* int *const Km = &brkc.km[0] - 1; */
	/* int *const Ku = &brkc.ku[0] - 1; */
	/* int *const Msrc = &brkc.msrc[0] - 1; */
	/* int *const Nsgpt = &nsgpt[0] - 1; */
	/* double *const Pk = &brkc.pk[0] - 1; */
	/* double *const Tauc = &brkc.tauc[0] - 1; */
	/* double *const Taut = &brkc.taut[0] - 1; */
	/* double *const Xc = &brkc.xc[0] - 1; */


	if( brkc.nin > 0 )
		zclose( &brkc.nin, nerr );
	{
	if(tblfile.file != NULL) 
		fclose(tblfile.file);
	}
	return;
} /* end of function */

