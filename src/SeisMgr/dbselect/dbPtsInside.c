#include <float.h>
#ifndef FALSE
#       define FALSE 0
#       define TRUE !FALSE
#endif FALSE
#include "dbPtsInside.h"



/* Private module function prototypes */

static int ccw(struct point p0, struct point p1, struct point p2);

static int intersect(struct line l1, struct line l2);

static int  CheckVertex( struct point *p, struct line lp, 
                         struct line lt, int count, int *i, int N);

/* --------------------------------------------------------------------------- */






  /* Given 3 points p0, p1, p2 traveling from the 1st to the 2nd to the 3rd, are
     we going counter clockwise or clockwise?
     1  => ccw
     -1 => cw
     0  => points colinear and p2 between p0 and p1
  */
static int ccw(struct point p0, struct point p1, struct point p2)
{

   float dx1, dx2, dy1, dy2;
   
   dx1 = p1.x - p0.x;
   dy1 = p1.y - p0.y;
   dx2 = p2.x - p0.x;
   dy2 = p2.y - p0.y;
   if( dx1 * dy2 > dy1 * dx2 ) return 1;
   if( dx1 * dy2 < dy1 * dx2 ) return -1;
   if( (dx1 * dy2 < 0 ) || (dy1 * dy2 < 0 ) ) return -1;
   if( (dx1*dx1 + dy1*dy1) < (dx2*dx2 + dy2*dy2) ) return 1;
   return 0;
}
/* --------------------------------------------------------------------------- */







/* returns 1 if l1 intersects l2, 0 otherwise. */
static int intersect(struct line l1, struct line l2)
{

   return (( ccw(l1.p1, l1.p2, l2.p1) *
	   ccw(l1.p1, l1.p2, l2.p2) ) <= 0) &&
          (( ccw(l2.p1, l2.p2, l1.p1) *
	  ccw( l2.p1, l2.p2, l1.p2) ) <= 0);
}
/* --------------------------------------------------------------------------- */





/* If intersection was not with a vertex just increment count and return. Otherwise:
   check the first endpoint of the current line segment (lp.p1.y) and the 2nd
   endpoint of the following line segment (lp2.p2.y). If the y value of the current
   point is between them then increment i and count and return. Otherwise just
   increment i and return. 
*/
static int  CheckVertex( struct point *p, struct line lp, 
                         struct line lt, int count, int *i, int N)
{
   struct line lp2;

   if(lt.p1.y != lp.p2.y)return(count + 1);

   (*i)++;
   if(*i < N-1){   
      lp2.p1 = *(p + (*i));
      lp2.p2 = *(p + (*i) + 1);
   }
   else{
      lp2.p1 = lp.p2;
      lp2.p2 = p[0];      
   }
   if(lt.p1.y > lp.p1.y && lt.p1.y < lp2.p2.y)return(count + 1);
   if(lt.p1.y < lp.p1.y && lt.p1.y > lp2.p2.y)return(count + 1);
}
/* --------------------------------------------------------------------------- */




/* return true if point t is inside the polygon represented by the array of
   points in p. The number of points in p is N. 
*/
int inside(struct point t, struct point *p, int N)
{
   int i;
   int count = 0;
   int j = 0;
   struct line lt, lp, lp2;

   /* build the test line */
   lt.p1 = t;
   lt.p2 = t;
   lt.p2.x = FLT_MAX;
   i=0;
   while(i < N-1){
      lp.p1 = *(p + i);
      lp.p2 = *(p + i + 1);
      if( intersect(lp,lt) ){
         count = CheckVertex(p, lp, lt, count, &i, N);
      }
      i++;
   }
   if(i == N) return count % 2; /* intersection was on last segment of polygon */
   lp.p1 = lp.p2;
   lp.p2 = p[0];      
   if( intersect(lp,lt))count = CheckVertex(p, lp, lt, count, &i, N);
   return count % 2;
}
/* --------------------------------------------------------------------------- */

