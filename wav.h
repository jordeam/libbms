#ifndef _WAV_H
#define _WAV_H

double wav_get(double * data, int n, int i);
void wav_put(double * data, int n, int i, double x);
void wav_cutpeak(double * data, int n, double dmax);
double wav_deriv(double * data, int n, int i);
double wav_integ(double * data, int n, int i1, int i2);
void wav_normalize(double * data, int n);
double wav_getmax(double * data, int n);

/*
  get the minimum value of 'data' in 'data';
*/
double wav_getmin(double *data, int n);

/*
  get a value of 'data' in the range [0.0,1.0[ using t, this range correspons to [0,'n'[
*/
double wav_getin(double * data, int n, double t);

void wav_putin(double * data, int n, double t, double x);

#endif
