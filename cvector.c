#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "constant.h"
#include "sysconst.h"
#include "vvector.h"
#include "strbms.h"

#include "cvector.h"

/*
  Get an element from data.
*/
complex cvector_get(cvector_t * self, int i) {
  complex x;
  vvector_get(self, i, &x);
  return x;
}

/*
  Put en element in data
*/
int cvector_put(cvector_t * self, int i, complex xin) {
  complex x = xin;
  if (vvector_getDebug() & 4) {
    printf("cvector_put %d (%g,%g)\n", i, creal(x), cimag(x));
  }
  return (int) vvector_put(self, i, &x);
}

/*
  Append an element in data
*/
int cvector_append(cvector_t * self, complex xin) {
  complex x = xin;
  return vvector_append(self, &x);
}

/*
  Creates an identical cvector with malloc and returns it.
*/
cvector_t * cvector_dup(cvector_t * src) {
  cvector_t * clone = cvector_new(src->n);
  memcpy(clone->data, src->data, src->n * sizeof(complex));
  return clone;
}

/*
  swap the first N/2 data with the last N/2
*/
void cvector_swap(cvector_t * self) {
  complex tmp;
  size_t n2 = self->n >> 1, i;

  for (i = 0; i < n2; i++) {
    tmp = CVECTOR(self, n2 + i);
    CVECTOR(self, n2 + i) = CVECTOR(self, i);
    CVECTOR(self, i) = tmp;
  }
}

/*
  multiply all elements by a constant
*/
void cvector_mulk(cvector_t * self, complex k) {
  size_t i;
  for (i = 0; i < self->n; i++) {
    CVECTOR(self, i) *= k;
  }
}

