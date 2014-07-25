#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "constant.h"
#include "sysconst.h"
#include "wav.h"

/*
  Get a member from data
*/
double wav_get(double * data, int n, int i) {
  i %= n;
  if (i < 0)
    i += n;
  return data[i];
}

/*
  Put a member in data
*/
void wav_put(double * data, int n, int i, double x) {
  if (!n)
    return;
  i %= n;
  if (i < 0)
    i += n;
  data[i] = x;
}

/*
  cut off noise in 'data';
*/
void wav_cutpeak(double * data, int n, double dmax)
{
  int i, i0=0, j=0, k;
  double wm;

  while (fabs(wav_deriv(data, n, i0)) > dmax && i0 < n) 
    i0++;
  if (i0==n) 
    return;

  for (i=i0; i < n + i0; i++) 
    if (fabs(wav_deriv(data, n, i)) > dmax) {
      j=i+1;
      while ((fabs(wav_deriv(data, n, j++)) > dmax || 
	      fabs(wav_deriv(data, n, j)) > dmax) && 
	     j <= n + i0);
      wm = (wav_get(data, n, j) - wav_get(data, n, i))/(j-i);
      for (k=i+1; k < j; k++) 
	wav_put(data, n, k, wm*(k-i) + wav_get(data, n, i));
      i=j;
    }   
}

/*
  derive 'data' of 'data' in point 'i'
*/
double wav_deriv(double * data, int n, int i)
{
  i %= n;
  if (i < 0)
    i += n;
/*   printf("wav_deriv: i = %d, n = %d, data[i+1]-data[i] = %g\n", i, n */
/* 	 , data[i+1]-data[i]); */
  if (i == n - 1)
    return (data[0] - data[i]) * n;
  else
    return (data[i + 1] - data[i]) * n;
}

/*
  eval the integral of 'data data' from 'i1' to 'i2'
*/
double wav_integ(double *data, int n, int i1, int i2)
 {
  int i, j1, j2;
  double sum;

  j1 = i1 % n;
  if (j1 < 0)
    j1 += n;

  j2 = i2 % n;
  if (j2 < 0)
    j2 += n;

  sum = (data[j1] + data[j2]) / 2;
  if (j2 > j1) { 
    for (i = j1 + 1; i < j2; i++)
      sum += data[i];
    return sum / n;
    }
  else {
    for (i = j2 + 1; i < j1; i++)
      sum += data[i];
    return -sum/n;
    }
  }

/*
  makes the maximum value of 'data data' be 1 and minimum be -1;
*/
void wav_normalize(double  * data, int n)
{
  double max, min, norm, offset;
  int i;
  max = wav_getmax(data, n);
  min = wav_getmin(data, n);
  norm = (max-min)/2;
  offset = (max+min)/2;
  for (i = 0; i < n; i++) {
    data[i] -= offset;
    data[i] /= norm;
  }
}

/*
  get the maximum value of 'data' of 'data';
*/
double wav_getmax(double * data, int n) {
  double max;
  int i;
  if (!n)
    return 0;
  max=data[0];
  for (i = 1; i < n; i++)
    if  (max < data[i])
      max = data[i];
  return max;
}

/*
  get the minimum value of 'data' in 'data';
*/
double wav_getmin(double * data, int n) {
  double min;
  int i;
  if (!n)
    return 0;
  min=data[0];
  for (i = 1; i < n; i++)
    if  (min > data[i])
      min = data[i];
  return min;
}

/*
  get a value of 'data' in the range [0,1[, this range correspons to [0,'n'[
*/
double wav_getin(double * data, int n, double t)
{
  int i;
  if (!n)
    return 0;
  i=(int)(t*n);
  i %= n;
  if (i < 0)
    i += n;
  return data[i];
}

/*
  put a value in data in the range [0,1[, this range correspons to [0,n[
*/
void wav_putin(double * data, int n, double t, double x)
{
  int i;
  if (!n)
    return;
  i=(int)(t*n);
  i %= n;
  if (i < 0)
    i += n;
  data[i] = x;
}
