#ifndef _CVECTOR_H
#define _CVECTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <complex.h>

#include "wav.h"
#include "vvector.h"

/* 
   Defining type 'rvec':
   maxn is the maximum number of elements of data. It is evaluated from
   CVECTOR_BLKSIZE, wich defines the size of each BLOCK;
   n is the actual number of elements in data;
*/

/* 
   The son has an identical structure
*/
typedef vvector_t cvector_t;

/* Identical methods */

#define cvector_alloc vvector_alloc
#define cvector_init(self, n) vvector_init(self, n, sizeof(complex))
#define cvector_new(n) cvector_init(cvector_alloc(), n);
#define cvector_destroy(self) vvector_destroy(self)
#define cvector_free(self) vvector_destroy(self)
#define cvector_resize(self, n) vvector_resize(self, n)
#define cvector_clear(self) vvector_resize(self, 0)
#define cvector_setDebug(debug_mode) vvector_setDebug(debug_mode)
#define cvector_getDebug() vvector_getDebug()

/* raw access, for spedd purposes */

#define CVECTOR(self,i) (*((complex *)(self->data) + i))

/* return a copy */
cvector_t * cvector_dup(cvector_t * self);

/* 
   Accessing elements
*/
#define cvector_n(self) vvector_n(self)
#define cvector_maxn(self) vvector_maxn(self)
// #define cvector_size(self) vvector_size(self) // not used 
#define cvector_data(self) vvector_data(self)

/* Callback methods */

complex cvector_get(cvector_t * self, int i);
int cvector_put(cvector_t * self, int i, complex x);
int cvector_append(cvector_t * self, complex x);

/* Modifiers */
void cvector_mulk(cvector_t * self, complex k);
void cvector_swap(cvector_t * self);

#endif
