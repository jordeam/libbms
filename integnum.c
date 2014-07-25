#include "func.h"
#include "integnum.h"

double integnum(int  i1, int i2, double *buff, int SIZE) {
  double soma=0;
  int i;
  i1=inCicle(i1, SIZE);
  i2=inCicle(i2, SIZE);
  if (i2>i1) {
    for (i=i1+1; i<i2; i++)
      soma+=*(buff+i);
    soma+=(*(buff+i1)+*(buff+i2))/2;
    }
  else {
    for (i=0; i<i2; i++)
      soma+=*(buff+i);
    for (i=i1+1; i<SIZE; i++)
      soma+=*(buff+i);
    soma+=(*(buff+i1)+*(buff+i2))/2;
    }
  return soma/SIZE;
  }    

