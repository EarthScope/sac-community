#define CHAR_FIELD_LEN 9

struct hdrTimes{
   double delta;
   double b;
   double e;
   double o;
   double a;
   double t[10];
   double f;
   char ko[CHAR_FIELD_LEN];
   char ka[CHAR_FIELD_LEN];
   char kt[10][CHAR_FIELD_LEN];
   char kf[CHAR_FIELD_LEN];
};

struct hdrStation{
   double stla;
   double stlo;
   double stel;
   double stdp;
   double cmpaz;
   double cmpinc;
   char kstnm[CHAR_FIELD_LEN];
   char kcmpnm[CHAR_FIELD_LEN];
   char knetwk[CHAR_FIELD_LEN];
};

struct hdrEvent{
   double evla;
   double evlo;
   double evel;
   double evdp;
   double nzyear;
   double nzjday;
   double nzhour;
   double nzmin;
   double nzsec;
   double nzmsec;
   char kevnm[17];
   double mag;
   double imagtyp;
   double imagsrc;
};

struct hdrUser{
   double data;
   char label[CHAR_FIELD_LEN];
};

struct hdrDataDescrip{
   double iftype;
   double idep;
   double iztype;
   double iinst;
   double istreg;
   double ievreg;
   double ievtyp;
   double iqual;
   double isynth;
   char *filename;
};

struct hdrEventSta{
   double dist;
   double az;
   double baz;
   double gcarc;
};

struct hdrLLNLextensions{
   double xminimum;
   double xmaximum;
   double yminimum;
   double ymaximum;
   double norid;
   double nevid;
   double nwfid;
   double nxsize;
   double nysize;
};

struct hdrDepMec{
  double checked;  /* Checked - trace on/off variable */
  double flipped;  /* Flipped - polarity reversal variable */
  double signoise;  /* SigNoise */
  double snrfixed;  /* SNRFixed */
  double filtertype;     /* FilterType */
  double filterorder;    /* FilterOrder */
  double lowerfiltercorner;  /* LowerFilterCorner */
  double upperfiltercorner;  /* UpperFilterCorner */
  double mtisotropicfraction;  /* MTisotropicFraction */
};

/* Struct defs for blackboard variable manipulation */

struct BlackBoardVars{
   char *name;
   int type;
   float floatval;
   int  intval;
   char *stringval;
   struct BlackBoardVars * next;
};

#define INT_BBVAL 0
#define FLOAT_BBVAL 1
#define STRING_BBVAL 2

#define SAC_UNDEFINED -12345.0
//#define SAC_CHAR_UNDEFINED "-12345"

int DataIsComplex;
