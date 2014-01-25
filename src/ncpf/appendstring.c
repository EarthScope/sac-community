
#include "ncpf.h"


#include "co.h"

void /*FUNCTION*/ appendstring(string, string_s, input, input_s, output, 
	 output_s)
char *string;   int string_s;
char *input;   int input_s;
char *output;   int output_s;
{
	int i, ioutsp, ninput, noutput, nstring;

	/*=====================================================================
	 * PURPOSE:  To append a character string to a text string.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    string:   String to append to input text. [c]
	 *    input:    Input text string. [c]
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    output:   Output text string. [c]
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  881228
	 *===================================================================== */
	/*      include '../../inc/mach' */
	/* PROCEDURE: */
	/* - Determine length of string and input text. */
	nstring = nstrlensp( string,string_s );
	ninput = nstrlensp( input,input_s );

	/* - Concatenate input text and string to form output text.
	 *   Also fill output text with spaces. */

	if( nstring == 0 ){
		if( ninput == 0 ){
			ioutsp = 1;
			}
		else{
			fstrncpy( output, output_s-1, input, ninput);
			ioutsp = ninput + 1;
			}
		}
	else if( ninput == 0 ){
		fstrncpy( output, output_s-1, string, nstring);
		ioutsp = nstring + 1;
		}
	else{
                fstrncpy( output, output_s-1, input, ninput);
                fstrncpy( output+ninput, output_s-1-ninput, string, nstring);
		ioutsp = ninput + nstring + 1;
		}

	noutput = (output_s - 1);
	for( i = ioutsp; i <= noutput; i++ ){
		output[i - 1] = ' ';
		}

       

	return;

} /* end of function */

