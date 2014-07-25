#ifndef _func_h
#define _func_h

/* put i in the circular interval from 0 to n-1 */
#define CCR(i, n) { i%=n; if (i<0) i+=n; }

/* return the fractional part of real */
#define frac(x) ((x)-floor(x))

/* dummy square of a number */
#define sqr(x) ((x)*(x))

/* degrees and radians conversion */
#define rad2deg(x) ((x)*(180/M_PI))
#define deg2rad(x) ((x)*(M_PI/180))

#define maxof2(x,y) (((x) > (y)) ? x : y)

/* return value in circular interval of SIZE */
int inCicle(int i, int SIZE);

double in_range(double x, double angle);
double RMS(double *D, int N);
double media(double *D, int N);
double medabs(double *D, int N);

#define moduni(x) { (x)/=2*M_PI; (x)=frac(x); if ((x)<0) (x)+=1; }
#define moddup(x) { (x)/=2*M_PI; (x)=frac(x); if ((x)<0) (x)+=1; (x)*=2;}

/* return true if values have the same sign */
#define samesign(x, y) (((x)<0 && (y)<0) || ((x)>=0 && (y)>=0))

#endif
