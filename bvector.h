#ifndef _bvector_h
#define _bvector_h

#include <stdio.h>
#include <stdint.h>

// #include "wav.h"
#include "vvector.h"

/*+ 
   Defining type 'bvec':
   maxn is the maximum number of elements of data. It is evaluated from
   RVECTOR_BLKSIZE, wich defines the size of each BLOCK;
   n is the actual number of elements in data;
*/

/*
  Primary data type is uint8_t defined in stdint.h
*/

/*+
   The son has an identical structure
*/
typedef vvector_t bvector;

/*+
  Identical methods 
*/
#define bvec_new(n) vvector_init(vvector_alloc(), n, sizeof(uint8_t))

#define bvec_alloc vvector_alloc
#define bvec_init(self, n) vvector_init(self, n, sizeof(uint8_t))

#define bvec_destroy(self) vvector_destroy(self)

#define bvec_resize(self, n) vvector_resize(self, n)
#define bvec_clear(self) vvector_resize(self, 0)

#define bvec_dup(self) vvector_dup(self)

/*+
   Accessing elements
*/
#define bvec_n(self) vvector_n(self)
#define bvec_size(self) vvector_n(self)
#define bvec_maxn(self) vvector_maxn(self)
#define bvec_data(self) vvector_data(self)

/*+ Callback methods */

uint8_t bvec_get(bvector * self, int i);
uint32_t bvec_getw(bvector * self, int i);
int bvec_put(bvector * self, int i, uint8_t x);
int bvec_append(bvector * self, uint8_t x);
int bvec_appendw(bvector * self, uint32_t x);

/*+
  New methods
*/
void bvec_fputs(bvector * self, FILE * stream);
void bvec_afputs(bvector * self, FILE * stream);
void bvec_getFromStr(bvector * self, const char * s);
void bvec_getFromASCIIStr(bvector * self, const char * s);

int bvec_CKS8(bvector * self);
int bvec_CKS6(bvector * self);

void bvec_putCKS8(bvector * self);
void bvec_putCKS16(bvector * self);

int bvec_verCKS8(bvector * self);
int bvec_verCKS16(bvector * self);

/*+
  Operation between two bvector objects
*/
bvector * bvec_copy(bvector * dest, bvector * source);

#endif
