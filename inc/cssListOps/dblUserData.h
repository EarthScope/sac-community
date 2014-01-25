#ifndef FALSE
#       define FALSE 0
#       define TRUE !FALSE
#endif 

#ifndef DBL_USERDATA
#define DBL_USERDATA 

/*                 +=======================================+                 */
/*=================|           UserData structure          |=================*/
/*                 +=======================================+                 */

/* #ifndef DBL_PUBLIC_DEFS */
   struct complex{
      float real;
      float imag;
   };
   typedef struct complex ComplexFloat;
/* #endif */

enum dataType {dblREAL, dblCOMPLEX};
struct matrixDef{
   int nrows;
   int ncols;
   struct complex **matrix;
   enum dataType type;
   char *comment;
};
typedef struct matrixDef Matrix;

struct DataPlusComment{
   ComplexFloat *data;
   int dataLen;
   enum dataType type;
   char *comment;
   int reference;
   int  SacUserNum;
   struct DataPlusComment *prev;
   struct DataPlusComment *next;
};
typedef struct DataPlusComment DataComment;



enum userDataType {DATACOMMENT, MATRIX};
struct userData_{
   enum userDataType type;
   DataComment *dataComment;
   Matrix *matrix;
   char *comment;
};      
typedef struct userData_ userData;      


#endif 
