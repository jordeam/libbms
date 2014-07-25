/* 
   This prgram evaluates the zero of a function using the ``secant''
   numeric method 
*/

#define BOOL unsigned

/* Error codes */
#define NLSOL_NOERROR 0
#define NLSOL_ITEROVERFLOW 1
#define NLSOL_MAXBISECTREACH 3

int    nlsol_get_errcode();
void   nlsol_set_maxiter(int);
void   nlsol_clear_errcode();
void   nlsol_set_err(double);
double nlsol_sec(double(*)(double) , double, double);
double nlsol_bisect(double(*)(double) , double, double);
void   nlsol_set_maxbisect(int);
void   bip2(double(*)(double) , double, double, int);
