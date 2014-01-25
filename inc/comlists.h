/** 
 * @file   comlists.h
 * 
 * @brief  Com Lists
 * 
 */


#define	MCOMNAMES	800
#define	MEXTCOMNAMES	100
#define	MODULEEXTCOM	100
#define	MPROCESSES	5

/** 
 * @struct kmcomlists
 *   Command Character Lists Lengths
 */
struct t_cmcomlists {
  int icomlist;
  int icomliststart[MPROCESSES];
  int ncomlistentries[MPROCESSES];
  int icommodule[MCOMNAMES];
  int icomindex[MCOMNAMES];
  int nextcomnames;
  int iextcomindex[MEXTCOMNAMES];
} cmcomlists;

/** 
 * @struct kmcomlists
 *   Command Character Lists
 */
struct t_kmcomlists {
  char kcomnames[MCOMNAMES][9];
  char kextcomnames[MEXTCOMNAMES][9];
  char kcomnames_full[MCOMNAMES][30];
} kmcomlists;


#ifdef DOINITS

   int *const Icomindex = &cmcomlists.icomindex[0] - 1;
   int *const Icomliststart = &cmcomlists.icomliststart[0] - 1;
   int *const Icommodule = &cmcomlists.icommodule[0] - 1;
   int *const Iextcomindex = &cmcomlists.iextcomindex[0] - 1;
   int *const Ncomlistentries = &cmcomlists.ncomlistentries[0] - 1;

#else

   extern int *const Icomindex;
   extern int *const Icomliststart;
   extern int *const Icommodule;
   extern int *const Iextcomindex;
   extern int *const Ncomlistentries;

#endif

