
#include <string.h>

#include "ncpf.h"
#include "cpf.h"

void /*FUNCTION*/ inicpf()
{



	/*=====================================================================
	 * PURPOSE: Variable initialization of common blocks cmcpf and kmcpf.
	 *=====================================================================
	 * PARAMETERS:
	 *    MSIZE:      Size of vars section used to store macro arguments. [i]
	 *    MMACLEVEL:  Maximum number of levels of macro nesting. [i]
	 *=====================================================================
	 * VARIABLE DEFINITIONS:
	 *    knoval:  Character string used to designate that a particular
	 *             macro keyword has no current value. [c8]
	 *    kbb:     Character used to designate a blackboard variable. [c1]
	 *    khdr:    Character used to designate a header variable. [c1]
	 *    karg:    Character used to designate a macro argument. [c1]
	 *    kfill:   Character used to fill common block to a word boundary.
	 *    nmaclevel:  Current number of levels of macro nesting. [i]
	 *    knames:  Names of macro files open at each level of nesting. [ca]
	 *    imacstatus:  Current macro status. [i]
	 *                 = 0 ok
	 *                 = 1 execution error has occurred.
	 *=====================================================================
	 * MODULE/LEVEL:  cpf/4
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920325:  Added getxtime function.
	 *    890818:  Added initialization for REPLY function.
	 *    881228:  Added inline function initialization.
	 *    870722:  Added macro status information initialization.
	 *    870410:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870410
	 *===================================================================== */
	/* PROCEDURE: */
	strcpy( kmcpf.knoval, "NO VALUE" );

	kmcpf.kbb = '%';
	kmcpf.khdr = '&';
	kmcpf.karg = '$';
	kmcpf.kescape = '@';

	kmcpf.kfunctionbegin = '(';
	kmcpf.kfunctionend = ')';

	cmcpf.nstringfuncs = 7;
	strcpy( kmcpf.kstringfuncs[0], "CHANGE  " );
	Nstringargs[1] = 3;
	strcpy( kmcpf.kstringfuncs[1], "DELETE  " );
	Nstringargs[2] = 2;
	strcpy( kmcpf.kstringfuncs[2], "BEFORE  " );
	Nstringargs[3] = 2;
	strcpy( kmcpf.kstringfuncs[3], "AFTER   " );
	Nstringargs[4] = 2;
	strcpy( kmcpf.kstringfuncs[4], "SUBSTRIN" );
	Nstringargs[5] = 3;
	strcpy( kmcpf.kstringfuncs[5], "CONCATEN" );
	Nstringargs[6] = -1;
	strcpy( kmcpf.kstringfuncs[6], "REPLY   " );
	Nstringargs[7] = 1;

	cmcpf.nnumericfuncs = 22;
	strcpy( kmcpf.knumericfuncs[0], "ADD     " );
	Nnumericargs[1] = -1;
	strcpy( kmcpf.knumericfuncs[1], "SUBTRACT" );
	Nnumericargs[2] = -1;
	strcpy( kmcpf.knumericfuncs[2], "MULTIPLY" );
	Nnumericargs[3] = -1;
	strcpy( kmcpf.knumericfuncs[3], "DIVIDE  " );
	Nnumericargs[4] = -1;
	strcpy( kmcpf.knumericfuncs[4], "SQRT    " );
	Nnumericargs[5] = 1;
	strcpy( kmcpf.knumericfuncs[5], "EXP     " );
	Nnumericargs[6] = 1;
	strcpy( kmcpf.knumericfuncs[6], "ALOG    " );
	Nnumericargs[7] = 1;
	strcpy( kmcpf.knumericfuncs[7], "POWER   " );
	Nnumericargs[8] = 1;
	strcpy( kmcpf.knumericfuncs[8], "ALOG10  " );
	Nnumericargs[9] = 1;
	strcpy( kmcpf.knumericfuncs[9], "SINE    " );
	Nnumericargs[10] = 1;
	strcpy( kmcpf.knumericfuncs[10], "ARCSINE " );
	Nnumericargs[11] = 1;
	strcpy( kmcpf.knumericfuncs[11], "COSINE  " );
	Nnumericargs[12] = 1;
	strcpy( kmcpf.knumericfuncs[12], "ARCCOSIN" );
	Nnumericargs[13] = 1;
	strcpy( kmcpf.knumericfuncs[13], "TANGENT " );
	Nnumericargs[14] = 1;
	strcpy( kmcpf.knumericfuncs[14], "ARCTANGE" );
	Nnumericargs[15] = 1;
	strcpy( kmcpf.knumericfuncs[15], "INTEGER " );
	Nnumericargs[16] = 1;
	strcpy( kmcpf.knumericfuncs[16], "PI      " );
	Nnumericargs[17] = 0;
	strcpy( kmcpf.knumericfuncs[17], "MINIMUM " );
	Nnumericargs[18] = -1;
	strcpy( kmcpf.knumericfuncs[18], "MAXIMUM " );
	Nnumericargs[19] = -1;
	strcpy( kmcpf.knumericfuncs[19], "ABSOLUTE" );
	Nnumericargs[20] = 1;
	strcpy( kmcpf.knumericfuncs[20], "GETTIME " );
	Nnumericargs[21] = 1;
	strcpy( kmcpf.knumericfuncs[21], "GETEPOCH" );
	Nnumericargs[22] = 2;

	cmcpf.nnumericabbrevs = 3;
	strcpy( kmcpf.knumericabbrevs[0], "ASINE   " );
	Inumericabbrevs[1] = 11;
	strcpy( kmcpf.knumericabbrevs[1], "ACOSINE " );
	Inumericabbrevs[2] = 13;
	strcpy( kmcpf.knumericabbrevs[2], "ATANGENT" );
	Inumericabbrevs[3] = 15;

	setmacrolevel( 0 );
	setmacrostatus( "OK",3 );

       
	return;

} /* end of function */

