

#include "gdm.h"

void begindevice1 ( int *nerr){ }
void beginframe1 ( int *nerr){ }
void beginwindow1 ( int number, 
                    int *nerr){ }
void createwindow1 ( int number, 
                     double xwinmn, 
                     double xwinmx, 
                     double ywinmn, 
                     double ywinmx, 
                     int *nerr){ }
void cursor1 ( float *xloc, 
               float *yloc, 
               char *kchar){ }
void cursortext1 ( double xloc, 
                   double yloc, 
                   char *ktext, 
                   int ktext_s){ }
void draw1 ( float *xloc, 
             float *yloc){ }
void enddevice1 ( int *nerr){ }
void endframe1 ( int *nerr){ }
void erase1 (void){ }
void flushbuffer1 ( int *nerr){ }
void getalphainfo1 ( int *nlines, 
                     char *erase, 
                     int erase_s){ }
void getdevicerat1 ( float *ratio){ }
void getratio1 ( float *ratio){ }
void getwindowstat1 ( int number, 
                      int *exists){ }
void hardwaretext1 ( char *ktext, 
                     int ktext_s, 
                     int *ntext){ }
void move1 ( float *xloc, 
             float *yloc){ }
void setcolor1 ( int index){ }
void setctable1 ( int iwindow, 
                  int nentry, 
                  double red, 
                  double green, 
                  double blue){ }
void setlinestyle1 ( int iline){ }
void settextsize1 ( double width, 
                    double height){ }
void setwidth1 ( int index){ }

