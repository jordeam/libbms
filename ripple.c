#include <stdio.h>

#include "sysconst.h"
#include "func.h"
#include "ripple.h"

double ripple_file(int colun, double t1, double t2, char *name) {
  FILE * f;
  char * s, c[STRSIZE];
  double data[10], mean = 0, value, max = 0, min = 0;
  int n = 0;

  f=fopen(name,"r");

  s=fgets(c, STRSIZE, f);
  while (s != NULL) {
    sscanf(c, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
           & data[0], & data[1], & data[2], & data[3], & data[4],
	   & data[5], & data[6], & data[7], & data[8], & data[9]);
    s = fgets(c, STRSIZE, f);
    if (data[0] >= t1 && data[0] <= t2) {
      value = data[colun];
      if (n == 0) max = min = mean = value;
      else {
        mean += value;
	if (value > max) max = value;
	if (value < min) min = value;
	}
      n++;
      }
    }
  if (n)
    mean /= n;

  fclose(f);
  return (max - min) / mean;
}

double ripple(double *data, int n, int i1, int i2)
{
  int i;
  double mean = 0, max, min, value;
  if (!n)
    return 0;
  min = max = * data;
  i1 %= n; if (i1 < 0) i1 += n;
  i2 %= n; if (i2 < 0) i2 += n;
  if (i1 == i2)
    return 0;
  if (i1>i2) {
    i=i1;
    i1=i2; i2=i;
  }
  for (i = i1; i <= i2; i++) {
    value = data[i];
    mean += value;
    if (value > max) max = value;
    if (value < min) min = value;
  }
  mean /= (i1 - i2);
  return (max - min) / mean;
} 
