/** 
 * @file   gtm.h
 * 
 * @brief  Graphics Transfomation
 * 
 */

#ifndef _GTM_H_
#define _GTM_H_

#include "gdm.h"

#define	MSISYM	8
#define	MSYM	16
#define	MSYMTB	40
#define	MUNSYM	1

struct t_cmgtm {
  float  xwcmin; /* World Coordinate Min X */
  float  xwcmax; /* World Coordinate Max X */
  float  ywcmin; /* World Coordinate Min Y */
  float  ywcmax; /* World Coordinate Max Y */
  float  xvpmin; /* Viewport Min X */
  float  xvpmax; /* Viewport Max X */
  float  yvpmin; /* Viewport Min Y */
  float  yvpmax; /* Viewport Max Y */
  float  xmpwv1; /* World to Viewport Mapping Transformation, X */
  float  xmpwv2; /* World to Viewport Mapping Transformation, X */
  float  ympwv1; /* World to Viewport Mapping Transformation, Y */
  float  ympwv2; /* World to Viewport Mapping Transformation, Y */
  int    lvpclip; /* Viewport Clipping */
  float  xvpold; /* Current Viewport Coordinate, X */
  float  yvpold; /* Current Viewport Coordinate, Y */
  int    ivpold; /* Current Viewport Coordinate, Inside or Outside */
  int    isym;   /* Symbol Number */
  float  symsz;  /* Symbol Size */
  float  symgap; /* Symbol Gap */
  int    lxdiv;  /* X Division Spacing Flag */
  float  xdiv;   /* X Division Spacing Used when lxdiv is ON */
  int    lnxdiv; /* Number of X Division Spacings Flag */
  int    nxdiv;  /* Number of X Division Spacings when lnxdiv is ON */
  float  xnicsp; /* X Number of Divisions, used with text  */
  int    lydiv;  /* Y Division Spacing Flag */
  float  ydiv;   /* X Division Spacing Used when lxdiv is ON */
  int    lnydiv; /* Number of X Division Spacings Flag */
  int    nydiv;  /* Number of X Division Spacings when lnxdiv is ON */
  float  ynicsp; /* Y Number of Divisions, used with text  */
  int    lnwsym; /* Unused */
  int    lscsym; /* Scaled Symbol Flag */
  int    jsyml1; /* Only in setsymbolnum() ------------*/
  int    jsym1b; /* Starting First Symbol */
  int    jsym1e; /* Ending First Symbol */
  int    ldbsym; /* Double Symbol Flag */
  int    jsyml2; /* Only in setsymbolnum() ------------*/
  int    jsym2b; /* Starting Second Symbol */
  int    jsym2e; /* Ending Second Symbol */
  int    isyml1[MSYM]; /* Symbol Numbers */
  int    isyml2[MSYM]; /* Symbol Numbers */
  int    nsymlc[MSISYM + 1]; /* Symbol location in symbol Table (x|y)symtb */
  int    ldrsym[MSYMTB]; /* Symbol Table - Draw or Movey */
  float  xsymtb[MSYMTB]; /* Symbol Table x position */
  float  ysymtb[MSYMTB]; /* Symbol Table y position */
  float  fac[9];         /* Fractions used in logarithmic scaling */
} cmgtm;

struct t_kmgtm {
  char kfac[9]; /* The number 1 to 9 used in logarithmic scaling */
} kmgtm;


#ifdef DOINITS

   float *const xvp = (float*)&cmgtm.xvpmin;
   float *const xwc = (float*)&cmgtm.xwcmin;
   float *const yvp = (float*)&cmgtm.yvpmin;
   float *const ywc = (float*)&cmgtm.ywcmin;
   float *const Xvp = (float*)(&cmgtm.xvpmin - 1);
   float *const Xwc = (float*)(&cmgtm.xwcmin - 1);
   float *const Yvp = (float*)(&cmgtm.yvpmin - 1);
   float *const Ywc = (float*)(&cmgtm.ywcmin - 1);

#else

   extern float *const xvp;
   extern float *const xwc;
   extern float *const yvp;
   extern float *const ywc;
   extern float *const Xvp;
   extern float *const Xwc;
   extern float *const Yvp;
   extern float *const Ywc;

#endif


void calwvtransform (void);
void getvport ( float *xmin, 
                float *xmax, 
                float *ymin, 
                float *ymax);
void getworld ( float *xmin, 
                float *xmax, 
                float *ymin, 
                float *ymax);
void inigtm (void);
void line ( double xloc1, 
            double yloc1, 
            double xloc2, 
            double yloc2);
void polyline ( float xloc[], 
                float yloc[], 
                int *number);
void polyfill(float *x, float *y, int n, int positive, int color);
void rectangle ( float *xloc1, 
                 float *xloc2, 
                 float *yloc1, 
                 float *yloc2);
void setsymbolgap ( double gap);
void setsymbolnum ( int number);
void setsymbolsize ( double size);
void setvport ( double xmin, 
                double xmax, 
                double ymin, 
                double ymax);
void setvportratio ( double ratio);
void setworld ( double xwmin, 
                double xwmax, 
                double ywmin, 
                double ywmax);
void symbol ( float xloc[], 
              float yloc[], 
              int number, 
              int lnewdp);
void vporttoworld ( double xloc, 
                    double yloc, 
                    float *xwloc, 
                    float *ywloc);
void worldcursor ( float *xwloc, 
                   float *ywloc, 
                   char *kchar);
void worlddraw ( double xwloc, 
                 double ywloc);
void worldline ( double xwloc1, 
                 double ywloc1, 
                 double xwloc2, 
                 double ywloc2);
void worldmove ( double xwloc, 
                 double ywloc);
void worldpolyline ( float xwloc[], 
                     float ywloc[], 
                     int number);
void worldsector ( double xwcen, 
                   double ywcen, 
                   double radius, 
                   double deg1, 
                   double deg2, 
                   double degi);
void worldtovport ( double xwloc, 
                    double ywloc, 
                    float *xloc, 
                    float *yloc);
void xaxis ( char *type, 
             char *annot, 
             char *ticks, 
             char *label, 
             int label_s);
void xlinaxis ( int lbotax, 
                int ltopax, 
                int lbottc, 
                int ltoptc, 
                float *widbot, 
                float *widtop);
void xlogaxis ( int lbotax, 
                int ltopax, 
                int lbottc, 
                int ltoptc, 
                float *widbot, 
                float *widtop);
void yaxis ( char *type, 
             char *annot, 
             char *ticks, 
             char *label, 
             int label_s);
void ylinaxis ( int llefax, 
                int lrigax, 
                int lleftc, 
                int lrigtc, 
                float *widlef, 
                float *widrig);
void ylogaxis ( int llefax, 
                int lrigax, 
                int lleftc, 
                int lrigtc, 
                float *widlef, 
                float *widrig);

void symbol_single(display_t *out, float x, float y);

#endif /* _GTM_H_ */
