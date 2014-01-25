#include <math.h>


static double dot_(double *v1, double *v2)
{
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
} 




static void CoordConvert(float *latc, float *lonc, float *depc, float *lat, 
                 float *lon, float *dep, float *xx, float *yy, 
                 float *zz, int iaction)
{
    /* System generated locals */
    double d__1, d__2;

    /* Builtin functions */
/*    double sin(), cos(), sqrt(), atan2(), asin(); */

    /* Local variables */
    static double xhat[3], yhat[3], zhat[3], temp[3], xlat, zold, xold, 
	    yold, xlon;
    static int j;
    static double r, x, y, z, xdepc, depth, xlatc, xlonc, radius, den;
    static double xlatrad, xlonrad;

/* equatorial radius */
/* POLAR RADIUS */
    xlatc = (double) (*latc);
    xlonc = (double) (*lonc);
    xdepc = (double) (*depc);
    xlat = (double) (*lat);
    xlon = (double) (*lon);
    depth = (double) (*dep);
/* 	   FIRST EXPRESS THE CENTER OF THE NEW COORDINATE SYSTEM IN TERMS */
/* 	   OF A RIGHT-HAND COORDINATE SYSTEM (ORIGIN AT CENTER OF EARTH, */
/* 	   X AXIS POINTS TO 0 DEGREES LONGITUDE, Z POINTS TO NORTH POLE */
/* 	   CORRECTING FOR THE ELLIPTICITY OF THE EARTH */
    xlatrad = xlatc * 3.141592654 / (float)180.;
    xlonrad = xlonc * 3.141592654 / (float)180.;
/* Computing 2nd power */
    d__1 = sin(xlatrad) * 6378.163;
/* Computing 2nd power */
    d__2 = cos(xlatrad) * 6356.778;
    r = 40544566.238813996 / sqrt(d__1 * d__1 + d__2 * d__2) - xdepc;
    zold = r * sin(xlatrad);
    xold = r * cos(xlatrad) * cos(xlonrad);
    yold = r * cos(xlatrad) * sin(xlonrad);
/* 	   THE NEW COORDINATE SYSTEM WILL HAVE X POINTING TO THE NORTH */
/* 	   AND NORMAL TO THE VECTOR POINTING FROM THE CENTER OF THE */
/* 	   EARTH TO THE ORIGIN OF THE NEW COORDINATES, SO THE COORDINATES */
/* 	   OF THE NEW X AXIS ARE: */
    xhat[2] = cos(xlatrad);
    xhat[0] = sin(xlatrad) * cos(xlonrad + 3.141592654);
    xhat[1] = sin(xlatrad) * sin(xlonrad + 3.141592654);
/* 	   THE Z AXIS IN THE NEW COORDINATE SYSTEM POINTS IN THE OPPOSITE */
/* 	   DIRECTION TO THE VECTOR FROM THE CENTER OF THE EARTH TO THE */
/* 	   ORIGIN OF COORDINATES SO IT IS GIVEN BY: */
    den = sqrt(xold * xold + yold * yold + zold * zold);
    zhat[0] = -xold / den;
    zhat[1] = -yold / den;
    zhat[2] = -zold / den;
/* 	   THE Y AXIS IN THE NEW COORDINATE SYSTEM IS THE CROSS-PRODUCT */
/* 	   OF ZHAT WITH XHAT */
    yhat[0] = zhat[1] * xhat[2] - xhat[1] * zhat[2];
    yhat[1] = xhat[0] * zhat[2] - zhat[0] * xhat[2];
    yhat[2] = zhat[0] * xhat[1] - xhat[0] * zhat[1];
    if (iaction == 0) {
/* 	   TO EXPRESS ANY POINT IN THE NEW COORDINATE SYSTEM WE FIRST */
/* 	   TRANSFORM IT INTO THE COORDINATE SYSTEM CENTERED AT */
/* 	   THE EARTH CENTER, THEN SHIFT IT BY -XOLD,-YOLD,-ZOLD, THEN */
/* 	   DOT THE SHIFTED COORDINATES WITH THE AXIS VECTORS IN THE ROTATE
D */
/* 	   COORDINATE SYSTEM. */
/* CONVERT FROM LAT AND LON TO X,Y */
	xlatrad = xlat * 3.141592654 / (float)180.;
	xlonrad = xlon * 3.141592654 / (float)180.;
/* Computing 2nd power */
	d__1 = sin(xlatrad) * 6378.163;
/* Computing 2nd power */
	d__2 = cos(xlatrad) * 6356.778;
	r = 40544566.238813996 / sqrt(d__1 * d__1 + d__2 * d__2) - depth;
	temp[2] = r * sin(xlatrad) - zold;
	temp[0] = r * cos(xlatrad) * cos(xlonrad) - xold;
	temp[1] = r * cos(xlatrad) * sin(xlonrad) - yold;
	x = dot_(temp, xhat);
	y = dot_(temp, yhat);
	z = dot_(temp, zhat);
	*xx = x;
	*yy = y;
	*zz = z;
    } else {
/* CONVERT FROM X,Y,Z TO LATITUDE,LONGITUDE,DEPT */
	x = (double) (*xx);
	y = (double) (*yy);
	z = (double) (*zz);
	for (j = 0; j < 3; ++j) {
	    temp[j] = x * xhat[j] + y * yhat[j] + z * zhat[j];

	}
	temp[0] += xold;
	temp[1] += yold;
	temp[2] += zold;
	radius = (float)0.;
	for (j = 1; j <= 3; ++j) {
/* Computing 2nd power */
	    d__1 = temp[j - 1];
	    radius += d__1 * d__1;
	}
	radius = sqrt(radius);
	xlon = atan2(temp[1], temp[0]);
	xlat = asin(temp[2] / radius);
/* Computing 2nd power */
	d__1 = sin(xlat) * 6378.163;
/* Computing 2nd power */
	d__2 = cos(xlat) * 6356.778;
	depth = 40544566.238813996 / sqrt(d__1 * d__1 + d__2 * d__2) - radius;

	xlat = xlat * (float)180. / 3.141592654;
	xlon = xlon * (float)180. / 3.141592654;
	*dep = depth;
	*lat = xlat;
	*lon = xlon;
    }
    return;
} 



void dbGetEnclosingBox(float latc, float lonc, float radius, float *minLat,
                     float *maxLat, float *minLon, float *maxLon)
{
   float depc = 0.0;
   float lat,lon,dep;
   float xx,yy,zz;
   int j,k;
   
   zz= 0.0;
   *minLon = 1000;
   *minLat = 1000;
   *maxLon = -1000;
   *maxLat = -1000;
   lat = lon = dep = 0.0;
   for(j=0;j<2;j++){
      xx = 2*j * radius -radius;
      for(k=0;k<2;k++){
         yy = 2*k * radius -radius;
         CoordConvert(&latc, &lonc, &depc, &lat, &lon, &dep, 
                      &xx, &yy, &zz, 1);
         *minLat = (*minLat < lat) ? *minLat:lat;
         *minLon = (*minLon < lon) ? *minLon:lon;
         *maxLat = (*maxLat > lat) ? *maxLat:lat;
         *maxLon = (*maxLon > lon) ? *maxLon:lon;
      }
   }
}






void dbDelaz(float *slat, float *slon, float *elat, float *elon, 
           float *delt, float *dist, float *azim, float *bazim)
{
    /* Builtin functions */
/*    double tan(), atan(), sin(), cos(), acos(), sqrt(); */

    /* Local variables */
    static double  a, b, c__, z__, celon, elatr, cslon, elonr, selon, a1, 
	    b1, c1, slatr, slonr, sslon, cd, ceclat, bz, cz, eclatr, seclat, 
	    csclat, sclatr, ssclat, aaa, cbz;

/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  This subroutine calculates the four quantities that depend on the  c */
/*  relative locations on the globe of epicenter and station.          c */
/*  Output: delt(angular separation id degrees), dist(geodesic distancec */
/*  in kilometers), azim(epicenter-to-station azimuth, in degrees N    c */
/*  thru E), and bazim(station-to-epicenter azimuth, in degrees N thru E */
/*  Input: slat(station latitude), slon(station longitude), elat(epi-  c */
/*  center latitude), and elon(epicenter longitude).  Latitudes(which  c */
/*  are input as geographic latitudes) are to be given from 0 to 90    c */
/*  degrees, +indicating North, and - indicating South.  Longitudes    c */
/*  are to be given from 0 to 180 degrees, + indicating East and -     c */
/*  indicating West, with  0=Greenwich Meridian.                       c */
/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */

/*  converting input angles from degrees to radians */
/*  (suffix r always denotes radian measure) */
    slatr = *slat * (float).01745329;
    slonr = *slon * (float).01745329;
    elatr = *elat * (float).01745329;
    elonr = *elon * (float).01745329;
/*  converting geographic latitudes to geocentric latitudes */
    aaa = tan(slatr) * (float).996647;
    slatr = atan(aaa);
    aaa = tan(elatr) * (float).996647;
    elatr = atan(aaa);
/*  At this point latitudes are converted into colatitudes, which run  c 
*/
/*  from 0 at North Pole to 180 at South Pole.  Also, longitudes are   c 
*/
/*  re-evaluated so that they run from 0 to 360 from Greenwich Eastward */
/*  sclatr=station colatitude   eclatr=epicenter colatitude  (radians) */
    sclatr = (float)1.570796327 - slatr;
    if (slonr >= 0.) {
	goto L20;
    } else {
	goto L10;
    }
L10:
    slonr += (float)6.283185307;
L20:
    eclatr = (float)1.570796327 - elatr;
    if (elonr >= 0.) {
	goto L40;
    } else {
	goto L30;
    }
L30:
    elonr += (float)6.283185307;
L40:
    seclat = sin(eclatr);
    ceclat = cos(eclatr);
    selon = sin(elonr);
    celon = cos(elonr);
    ssclat = sin(sclatr);
    csclat = cos(sclatr);
    sslon = sin(slonr);
    cslon = cos(slonr);
/*  calculation of direction cosines of station (a1,b1,c1) and of      c 
*/
/*  epicenter (a,b,c). For this calculation, x-axis is thru Greenwich  c 
*/
/*  Meridian, y-axis at 90E longitude, and z-axis thru North Pole.     c 
*/
    a = seclat * celon;
    b = seclat * selon;
    c__ = ceclat;
    a1 = ssclat * cslon;
    b1 = ssclat * sslon;
    c1 = csclat;
/*  Now you have all the info necessary to compute delt, dist, azim, */
/*  and bazim.  For reference:  cosine of delta=cd, cosine of azimuth=cz 
*/
/*  azim. in radians=z, cosine of backazimuth=cbz, and back-azimuth */
/*  in radians=bz. */
    cd = a * a1 + b * b1 + c__ * c1;
    *delt = acos(cd) * (float)57.29577951;
    *dist = *delt * (float).01745329252 * (float)6371.;
/*  computation of cz and z.  The following formula is derivable */
/*  from Bullen or via analytic geometry. */
    cz = (seclat * csclat - ceclat * ssclat * (celon * cslon + selon * sslon))
	     / sqrt((float)1. - cd * cd);
    z__ = acos(cz);
/*  The following test determines whether z should be > or < 180 degrees. 
*/
    if (sslon * celon - cslon * selon < (float)0.) {
	z__ = (float)6.283185307 - z__;
    }
    *azim = z__ * (float)57.29577951;
/*  computation of cbz and bz.  This is accomplished by switching the */
/*  roles of station and epicenter in the previous formula. */
    cbz = (ssclat * ceclat - csclat * seclat * (cslon * celon + sslon * selon)
	    ) / sqrt((float)1. - cd * cd);
    bz = acos(cbz);
/*  The following test determines whether bz should be > or < 180 degrees.
 */
    if (selon * cslon - celon * sslon < (float)0.) {
	bz = (float)6.283185307 - bz;
    }
    *bazim = bz * (float)57.29577951;
}

