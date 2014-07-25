#include <math.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "strbms.h"

char * fformat(double f, int ndig, int ndec, int put_plus) {
  char * s;
  char * s1;
  double x, decs, ints, fact;
  int i;

  if (f < 0) {
    s = strdup("-");
    f = -f;
  }
  else if (put_plus)
    s = strdup("+");
  else
    s = strdup("");

  fact = pow(10, ndec);

  x = rint(f * fact);
/*   printf("x = %g\n", x); */

  x /= fact;
/*   printf("x = %g\n", x); */

  ints = floor(x);
  decs = (x - ints) * fact;
/*   printf("decs = %g\n", decs); */
/*   printf("floor(decs) = %g\n", floor(decs)); */
/*   printf("rint(decs) = %g\n", rint(decs)); */
  asprintf(&s1, "%d", (int) ints);
  strcatdup(&s, s1);
/*   printf("ints s1 = %s\n", s1); */
  free(s1);

  if (ndec >= 0)
    strcatdup(&s, ",");

  asprintf(&s1, "%d", (int) decs);
  i = 0;
  while (strlen(s1) + i < ndec) {
    strcatdup(&s, "0");
    i++;
  }
/*   printf("decs s1 = %s\n", s1); */

  strcatdup(&s, s1);
  free(s1);

  // filling with spaces
  if (strlen(s) < ndig) {
    s1 = malloc(ndig);
    for (i = 0; i < ndig - strlen(s); i++)
      s1[i] = ' ';
    s1[i] = '\0';
    strcatdup(&s1, s);
    free(s);
    return s1;
  }
  else
    return s;
}


