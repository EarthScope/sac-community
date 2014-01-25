/** 
 * @file   sddhdr.h
 * 
 * @brief  SDD Header
 * 
 * 
 */


#ifndef _SDDHDR_H_
#define _SDDHDR_H_

#define	MBSHDR	716
#define	MSCOM	10
#define	MSREP	150
#define	MWESHD	(MSREP + 12 + 2)
#define	MWSHDR	179

struct t_kmshdr {
  char kshdr[MBSHDR*9];
} kmshdr;


#ifdef DOINITS

   int  *const iscalg = (int*)(kmshdr.kshdr + 112);
   int  *const isclas = (int*)kmshdr.kshdr;
   int  *const iscom  = (int*)(kmshdr.kshdr + 40);
   int  *const isdate = (int*)(kmshdr.kshdr + 80);
   int  *const isdelt = (int*)(kmshdr.kshdr + 88);
   int  *const isfrmt = (int*)(kmshdr.kshdr + 4);
   int  *const ishdr  = (int*)kmshdr.kshdr;
   int  *const isnpts = (int*)(kmshdr.kshdr + 92);
   int  *const isrep  = (int*)(kmshdr.kshdr + 116);
   int  *const issdep = (int*)(kmshdr.kshdr + 108);
   int  *const issel  = (int*)(kmshdr.kshdr + 96);
   int  *const issla  = (int*)(kmshdr.kshdr + 100);
   int  *const isslo  = (int*)(kmshdr.kshdr + 104);
   int  *const istime = (int*)(kmshdr.kshdr + 84);
   char *const kschan = (char*)(kmshdr.kshdr + 28);
   char *const kschdr = (char*)kmshdr.kshdr;
   char *const ksclas = (char*)kmshdr.kshdr;
   char *const kscom  = (char*)(kmshdr.kshdr + 40);
   char *const ksevnm = (char*)(kmshdr.kshdr + 12);
   char *const ksfrmt = (char*)(kmshdr.kshdr + 4);
   char *const ksstnm = (char*)(kmshdr.kshdr + 20);
   int  *const Iscom  = (int*)(kmshdr.kshdr + 40 - 4);
   int  *const Ishdr  = (int*)(kmshdr.kshdr - 4);
   int  *const Isrep  = (int*)(kmshdr.kshdr + 116 - 4);

#else

   extern int  *const iscalg;
   extern int  *const isclas;
   extern int  *const iscom;
   extern int  *const isdate;
   extern int  *const isdelt;
   extern int  *const isfrmt;
   extern int  *const ishdr;
   extern int  *const isnpts;
   extern int  *const isrep;
   extern int  *const issdep;
   extern int  *const issel;
   extern int  *const issla;
   extern int  *const isslo;
   extern int  *const istime;
   extern char *const kschan;
   extern char *const kschdr;
   extern char *const ksclas;
   extern char *const kscom;
   extern char *const ksevnm;
   extern char *const ksfrmt;
   extern char *const ksstnm;
   extern int  *const Iscom;
   extern int  *const Ishdr;
   extern int  *const Isrep;

#endif


#endif /* _SDDHDR_H_ */
