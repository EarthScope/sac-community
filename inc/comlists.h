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
} ;

/** 
 * @struct kmcomlists
 *   Command Character Lists
 */
struct t_kmcomlists {
    char kcomnames[MCOMNAMES][9];
    char kextcomnames[MEXTCOMNAMES][9];
    char kcomnames_full[MCOMNAMES][30];
} ;

#define COMLISTS_EXTERN \
    extern struct t_kmcomlists kmcomlists;                \
    extern struct t_cmcomlists cmcomlists;                \
    extern int *const Icomindex;                          \
    extern int *const Icomliststart;                      \
    extern int *const Icommodule;                         \
    extern int *const Iextcomindex;                       \
    extern int *const Ncomlistentries;


