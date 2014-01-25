
#include <string.h>

#include "unit.h"

#include "hdr.h"
#include "mach.h"
#include "msg.h"
#include "lhf.h"
#include "bool.h"
#include "dff.h"

int
main(void) {

  sac_warning_stdout();
  sac_error_stdout();
  
  sacio_initialize_common();

  /* inihdr */
  ok(cmhdr.nvhdrc == SAC_HEADER_MAJOR_VERSION,   "cmdhr.nvhdrc");
  ok(cmhdr.fundef == -12345.0, "cmhdr.fundef -12345.0");
  ok(cmhdr.iundef == -12345,   "cmhdr.iundef -12345");
  ok(cmhdr.nundef == -12345,   "cmhdr.nundef -12345");
  ok(strcmp(kmhdr.kundef, "-12345  ") == 0, "kmhdr.kundef -12345");
  ok(cmhdr.linc == FALSE, "cmhdr.linc");
  ok(cmhdr.llh == FALSE, "cmhdr.llh");

  /* inilhf */
  ok(cmlhf.nrpttp == 4, "cmlhf.nrpttp");
  ok(strcmp(kmlhf.krpttp[0], "DEFAULT ") == 0, "kmlhf.krpttp[0]");
  ok(strcmp(kmlhf.krpttp[1], "PICKS   ") == 0, "kmlhf.krpttp[1]");
  ok(strcmp(kmlhf.krpttp[2], "SPECIAL ") == 0, "kmlhf.krpttp[2]");
  ok(strcmp(kmlhf.krpttp[3], "STD     ") == 0, "kmlhf.krpttp[3]");
  ok(cmlhf.irpttp == 1, "cmlhf.irpttp");
  ok(cmlhf.lstall == TRUE, "cmlhf.lstall");
  ok(cmlhf.nlhcol == 1, "cmlhf.nlhcol");

  ok(cmlhf.nstrpt == MSTRPT, "cmlhf.nstrpt");
  ok(strcmp( kmlhf.kstrpt[0], "NPTS    ") == 0, "kmlhf.kstrpt[0]");
  ok(strcmp( kmlhf.kstrpt[1], "B       ") == 0, "kmlhf.kstrpt[1]");
  ok(strcmp( kmlhf.kstrpt[2], "E       ") == 0, "kmlhf.kstrpt[2]");
  ok(strcmp( kmlhf.kstrpt[3], "IFTYPE  ") == 0, "kmlhf.kstrpt[3]");
  ok(strcmp( kmlhf.kstrpt[4], "LEVEN   ") == 0, "kmlhf.kstrpt[4]");
  ok(strcmp( kmlhf.kstrpt[5], "DELTA   ") == 0, "kmlhf.kstrpt[5]");
  ok(strcmp( kmlhf.kstrpt[6], "ODELTA  ") == 0, "kmlhf.kstrpt[6]");
  ok(strcmp( kmlhf.kstrpt[7], "IDEP    ") == 0, "kmlhf.kstrpt[7]");
  ok(strcmp( kmlhf.kstrpt[8], "DEPMIN  ") == 0, "kmlhf.kstrpt[8]");
  ok(strcmp( kmlhf.kstrpt[9], "DEPMAX  ") == 0, "kmlhf.kstrpt[9]");
  ok(strcmp( kmlhf.kstrpt[10], "DEPMEN  ") == 0, "kmlhf.kstrpt[10]");
  ok(strcmp( kmlhf.kstrpt[11], "OMARKER ") == 0, "kmlhf.kstrpt[11]");
  ok(strcmp( kmlhf.kstrpt[12], "AMARKER ") == 0, "kmlhf.kstrpt[12]");
  ok(strcmp( kmlhf.kstrpt[13], "T0MARKER") == 0, "kmlhf.kstrpt[13]");
  ok(strcmp( kmlhf.kstrpt[14], "T1MARKER") == 0, "kmlhf.kstrpt[14]");
  ok(strcmp( kmlhf.kstrpt[15], "T2MARKER") == 0, "kmlhf.kstrpt[15]");
  ok(strcmp( kmlhf.kstrpt[16], "T3MARKER") == 0, "kmlhf.kstrpt[16]");
  ok(strcmp( kmlhf.kstrpt[17], "T4MARKER") == 0, "kmlhf.kstrpt[17]");
  ok(strcmp( kmlhf.kstrpt[18], "T5MARKER") == 0, "kmlhf.kstrpt[18]");
  ok(strcmp( kmlhf.kstrpt[19], "T6MARKER") == 0, "kmlhf.kstrpt[19]");
  ok(strcmp( kmlhf.kstrpt[20], "T7MARKER") == 0, "kmlhf.kstrpt[20]");
  ok(strcmp( kmlhf.kstrpt[21], "T8MARKER") == 0, "kmlhf.kstrpt[21]");
  ok(strcmp( kmlhf.kstrpt[22], "T9MARKER") == 0, "kmlhf.kstrpt[22]");
  ok(strcmp( kmlhf.kstrpt[23], "FMARKER ") == 0, "kmlhf.kstrpt[23]");
  ok(strcmp( kmlhf.kstrpt[24], "KZDATE  ") == 0, "kmlhf.kstrpt[24]");
  ok(strcmp( kmlhf.kstrpt[25], "KZTIME  ") == 0, "kmlhf.kstrpt[25]");
  ok(strcmp( kmlhf.kstrpt[26], "IZTYPE  ") == 0, "kmlhf.kstrpt[26]");
  ok(strcmp( kmlhf.kstrpt[27], "KINST   ") == 0, "kmlhf.kstrpt[27]");
  ok(strcmp( kmlhf.kstrpt[28], "RESP0   ") == 0, "kmlhf.kstrpt[28]");
  ok(strcmp( kmlhf.kstrpt[29], "RESP1   ") == 0, "kmlhf.kstrpt[29]");
  ok(strcmp( kmlhf.kstrpt[30], "RESP2   ") == 0, "kmlhf.kstrpt[30]");
  ok(strcmp( kmlhf.kstrpt[31], "RESP3   ") == 0, "kmlhf.kstrpt[31]");
  ok(strcmp( kmlhf.kstrpt[32], "RESP4   ") == 0, "kmlhf.kstrpt[32]");
  ok(strcmp( kmlhf.kstrpt[33], "RESP5   ") == 0, "kmlhf.kstrpt[33]");
  ok(strcmp( kmlhf.kstrpt[34], "RESP6   ") == 0, "kmlhf.kstrpt[34]");
  ok(strcmp( kmlhf.kstrpt[35], "RESP7   ") == 0, "kmlhf.kstrpt[35]");
  ok(strcmp( kmlhf.kstrpt[36], "RESP8   ") == 0, "kmlhf.kstrpt[36]");
  ok(strcmp( kmlhf.kstrpt[37], "RESP9   ") == 0, "kmlhf.kstrpt[37]");
  ok(strcmp( kmlhf.kstrpt[38], "KDATRD  ") == 0, "kmlhf.kstrpt[38]");
  ok(strcmp( kmlhf.kstrpt[39], "KSTNM   ") == 0, "kmlhf.kstrpt[30]");
  ok(strcmp( kmlhf.kstrpt[40], "CMPAZ   ") == 0, "kmlhf.kstrpt[40]");
  ok(strcmp( kmlhf.kstrpt[41], "CMPINC  ") == 0, "kmlhf.kstrpt[41]");
  ok(strcmp( kmlhf.kstrpt[42], "ISTREG  ") == 0, "kmlhf.kstrpt[42]");
  ok(strcmp( kmlhf.kstrpt[43], "STLA    ") == 0, "kmlhf.kstrpt[43]");
  ok(strcmp( kmlhf.kstrpt[44], "STLO    ") == 0, "kmlhf.kstrpt[44]");
  ok(strcmp( kmlhf.kstrpt[45], "STEL    ") == 0, "kmlhf.kstrpt[45]");
  ok(strcmp( kmlhf.kstrpt[46], "STDP    ") == 0, "kmlhf.kstrpt[46]");
  ok(strcmp( kmlhf.kstrpt[47], "KEVNM   ") == 0, "kmlhf.kstrpt[47]");
  ok(strcmp( kmlhf.kstrpt[48], "IEVREG  ") == 0, "kmlhf.kstrpt[48]");
  ok(strcmp( kmlhf.kstrpt[49], "EVLA    ") == 0, "kmlhf.kstrpt[49]");
  ok(strcmp( kmlhf.kstrpt[50], "EVLO    ") == 0, "kmlhf.kstrpt[50]");
  ok(strcmp( kmlhf.kstrpt[51], "EVEL    ") == 0, "kmlhf.kstrpt[51]");
  ok(strcmp( kmlhf.kstrpt[52], "EVDP    ") == 0, "kmlhf.kstrpt[52]");
  ok(strcmp( kmlhf.kstrpt[53], "IEVTYP  ") == 0, "kmlhf.kstrpt[53]");
  ok(strcmp( kmlhf.kstrpt[54], "KHOLE   ") == 0, "kmlhf.kstrpt[54]");
  ok(strcmp( kmlhf.kstrpt[55], "DIST    ") == 0, "kmlhf.kstrpt[55]");
  ok(strcmp( kmlhf.kstrpt[56], "AZ      ") == 0, "kmlhf.kstrpt[56]");
  ok(strcmp( kmlhf.kstrpt[57], "BAZ     ") == 0, "kmlhf.kstrpt[57]");
  ok(strcmp( kmlhf.kstrpt[58], "GCARC   ") == 0, "kmlhf.kstrpt[58]");
  ok(strcmp( kmlhf.kstrpt[59], "LOVROK  ") == 0, "kmlhf.kstrpt[59]");
  ok(strcmp( kmlhf.kstrpt[60], "IQUAL   ") == 0, "kmlhf.kstrpt[60]");
  ok(strcmp( kmlhf.kstrpt[61], "ISYNTH  ") == 0, "kmlhf.kstrpt[61]");
  ok(strcmp( kmlhf.kstrpt[62], "USER0   ") == 0, "kmlhf.kstrpt[62]");
  ok(strcmp( kmlhf.kstrpt[63], "USER1   ") == 0, "kmlhf.kstrpt[63]");
  ok(strcmp( kmlhf.kstrpt[64], "USER2   ") == 0, "kmlhf.kstrpt[64]");
  ok(strcmp( kmlhf.kstrpt[65], "USER3   ") == 0, "kmlhf.kstrpt[65]");
  ok(strcmp( kmlhf.kstrpt[66], "USER4   ") == 0, "kmlhf.kstrpt[66]");
  ok(strcmp( kmlhf.kstrpt[67], "USER5   ") == 0, "kmlhf.kstrpt[67]");
  ok(strcmp( kmlhf.kstrpt[68], "USER6   ") == 0, "kmlhf.kstrpt[68]");
  ok(strcmp( kmlhf.kstrpt[69], "USER7   ") == 0, "kmlhf.kstrpt[69]");
  ok(strcmp( kmlhf.kstrpt[70], "USER8   ") == 0, "kmlhf.kstrpt[70]");
  ok(strcmp( kmlhf.kstrpt[71], "USER9   ") == 0, "kmlhf.kstrpt[71]");
  ok(strcmp( kmlhf.kstrpt[72], "KUSER0  ") == 0, "kmlhf.kstrpt[72]");
  ok(strcmp( kmlhf.kstrpt[73], "KUSER1  ") == 0, "kmlhf.kstrpt[73]");
  ok(strcmp( kmlhf.kstrpt[74], "KUSER2  ") == 0, "kmlhf.kstrpt[74]");
  ok(strcmp( kmlhf.kstrpt[75], "NXSIZE  ") == 0, "kmlhf.kstrpt[75]");
  ok(strcmp( kmlhf.kstrpt[76], "XMINIMUM") == 0, "kmlhf.kstrpt[76]");
  ok(strcmp( kmlhf.kstrpt[77], "XMAXIMUM") == 0, "kmlhf.kstrpt[77]");
  ok(strcmp( kmlhf.kstrpt[78], "NYSIZE  ") == 0, "kmlhf.kstrpt[78]");
  ok(strcmp( kmlhf.kstrpt[79], "YMINIMUM") == 0, "kmlhf.kstrpt[79]");
  ok(strcmp( kmlhf.kstrpt[80], "YMAXIMUM") == 0, "kmlhf.kstrpt[80]");
  ok(strcmp( kmlhf.kstrpt[81], "NVHDR   ") == 0, "kmlhf.kstrpt[81]"); 
  ok(strcmp( kmlhf.kstrpt[82], "SCALE   ") == 0, "kmlhf.kstrpt[82]"); 
  ok(strcmp( kmlhf.kstrpt[83], "NORID   ") == 0, "kmlhf.kstrpt[83]");
  ok(strcmp( kmlhf.kstrpt[84], "NEVID   ") == 0, "kmlhf.kstrpt[84]");
  ok(strcmp( kmlhf.kstrpt[85], "NWFID   ") == 0, "kmlhf.kstrpt[85]");
  ok(strcmp( kmlhf.kstrpt[86], "IINST   ") == 0, "kmlhf.kstrpt[86]");
  ok(strcmp( kmlhf.kstrpt[87], "LPSPOL  ") == 0, "kmlhf.kstrpt[87]");
  ok(strcmp( kmlhf.kstrpt[88], "LCALDA  ") == 0, "kmlhf.kstrpt[88]");
  ok(strcmp( kmlhf.kstrpt[89], "KCMPNM  ") == 0, "kmlhf.kstrpt[89]");
  ok(strcmp( kmlhf.kstrpt[90], "KNETWK  ") == 0, "kmlhf.kstrpt[90]");
  ok(strcmp( kmlhf.kstrpt[91], "MAG     ") == 0, "kmlhf.kstrpt[91]");
  ok(strcmp( kmlhf.kstrpt[92], "IMAGTYP ") == 0, "kmlhf.kstrpt[92]");
  ok(strcmp( kmlhf.kstrpt[93], "IMAGSRC ") == 0, "kmlhf.kstrpt[93]");
  
  ok(cmlhf.npkrpt == MPKRPT, "cmlhf.npkrpt");
  ok(strcmp( kmlhf.kpkrpt[0], "NPTS    ")  == 0, "kmlhf.kpkrpt[0]");
  ok(strcmp( kmlhf.kpkrpt[1], "B       ")  == 0, "kmlhf.kpkrpt[1]");
  ok(strcmp( kmlhf.kpkrpt[2], "E       ")  == 0, "kmlhf.kpkrpt[2]");
  ok(strcmp( kmlhf.kpkrpt[3], "OMARKER ")  == 0, "kmlhf.kpkrpt[3]");
  ok(strcmp( kmlhf.kpkrpt[4], "AMARKER ")  == 0, "kmlhf.kpkrpt[4]");
  ok(strcmp( kmlhf.kpkrpt[5], "T0MARKER")  == 0, "kmlhf.kpkrpt[5]");
  ok(strcmp( kmlhf.kpkrpt[6], "T1MARKER")  == 0, "kmlhf.kpkrpt[6]");
  ok(strcmp( kmlhf.kpkrpt[7], "T2MARKER")  == 0, "kmlhf.kpkrpt[7]");
  ok(strcmp( kmlhf.kpkrpt[8], "T3MARKER")  == 0, "kmlhf.kpkrpt[8]");
  ok(strcmp( kmlhf.kpkrpt[9], "T4MARKER")  == 0, "kmlhf.kpkrpt[9]");
  ok(strcmp( kmlhf.kpkrpt[10], "T5MARKER") == 0, "kmlhf.kpkrpt[10]");
  ok(strcmp( kmlhf.kpkrpt[11], "T6MARKER") == 0, "kmlhf.kpkrpt[11]");
  ok(strcmp( kmlhf.kpkrpt[12], "T7MARKER") == 0, "kmlhf.kpkrpt[12]");
  ok(strcmp( kmlhf.kpkrpt[13], "T8MARKER") == 0, "kmlhf.kpkrpt[13]");
  ok(strcmp( kmlhf.kpkrpt[14], "T9MARKER") == 0, "kmlhf.kpkrpt[14]");
  ok(strcmp( kmlhf.kpkrpt[15], "FMARKER ") == 0, "kmlhf.kpkrpt[15]");
  ok(strcmp( kmlhf.kpkrpt[16], "KZDATE  ") == 0, "kmlhf.kpkrpt[16]");
  ok(strcmp( kmlhf.kpkrpt[17], "KZTIME  ") == 0, "kmlhf.kpkrpt[17]");
  
  ok(cmlhf.nrpt = MSTRPT, "cmlhf.nrpt");
  ok(cmlhf.itmkrf == 5, "cmlhf.itmkrf");

  /* inimsg */
  ok(cmmsg.nunits == 2, "cmmsg.nunits 2 <=> %d", cmmsg.nunits); /* stdout and stderr */
  ok(cmmsg.iunits[0] == stdout, "cmmsg.uunits[0] stdout");
  ok(cmmsg.iunits[1] == stderr, "cmmsg.uunits[1] stderr");

  ok(cmmsg.lsend[0][MERRORS - 1]    == TRUE, "Error on stdout");
  ok(cmmsg.lsend[0][MWARNINGS - 1]  == TRUE, "Warnings on stdout");
  ok(cmmsg.lsend[0][MOUTPUT - 1]    == TRUE, "Output on stdout");
  ok(cmmsg.lsend[0][MCOMMANDS - 1]  == FALSE, "Commands on stdout");
  ok(cmmsg.lsend[0][MMACROS - 1]    == FALSE, "Macros on stdout");
  ok(cmmsg.lsend[0][MPROCESSED - 1] == FALSE, "Processed on stdout");

  /*
  ok(cmmsg.lsend[1][MERRORS - 1]    == TRUE,  "Error on stderr");
  ok(cmmsg.lsend[1][MWARNINGS - 1]  == FALSE, "Warnings on stderr");
  ok(cmmsg.lsend[1][MOUTPUT - 1]    == FALSE, "Output on stderr");
  ok(cmmsg.lsend[1][MCOMMANDS - 1]  == FALSE, "Commands on stderr");
  ok(cmmsg.lsend[1][MMACROS - 1]    == FALSE, "Macros on stderr");
  ok(cmmsg.lsend[1][MPROCESSED - 1] == FALSE, "Processed on stderr");
  */
  ok(strcmp(kmmsg.ktpmsg[MERRORS - 1], "ERRORS  ") == 0,
     "Error messages is ERRORS");
  ok(strcmp(kmmsg.ktpmsg[MWARNINGS - 1], "WARNINGS") == 0,
     "Warnigns messages is WARNINGS");
  ok(strcmp(kmmsg.ktpmsg[MOUTPUT - 1], "OUTPUT  ") == 0,
     "Output messages is OUTPUT");
  ok(strcmp(kmmsg.ktpmsg[MCOMMANDS - 1], "COMMANDS") == 0,
     "Commands messages is COMMANDS");
  ok(strcmp(kmmsg.ktpmsg[MMACROS - 1], "MACROS  ") == 0,
     "Macros messages is MACROS");
  ok(strcmp(kmmsg.ktpmsg[MPROCESSED - 1], "PROCESSE") == 0,
     "Processed messages is PROCESSE");

  TEST_FINISH;
}


