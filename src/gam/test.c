#include <stdio.h>
#include <math.h>

main()
{
     double test;
     double gpnorm();

     test = gpnorm();
     printf("Returns: %lf\n",test);
}

double gpnorm()
{
    int i;
    double tsq, tnorm;

    tsq = 0.8;
    tnorm = 0.0;
    for(i=0; i<10; i++)
	{
	tsq = sqrt(tsq);
	tnorm += (tsq*tsq);
	printf("Loop %d: %lf\n",i,tnorm);
	}
    return(tnorm);
}
