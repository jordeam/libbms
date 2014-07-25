#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <stdint.h>
#include <string.h>

#include "vvector.h"
#include "strbms.h"

static int Debug = 0;

/*
  Allocates dynamic memory for an rvector object
*/
vvector_t * vvector_alloc(void) {
  return (vvector_t *) calloc(1, sizeof(vvector_t));
}

/*
  Alocates dynamic memory for data of an vvector object.
  Initializes data of an vvector object with 0.
*/
vvector_t * vvector_init(vvector_t * self, int n, int elm_size) {
  int blocks;

  if (!self) 
    return NULL;
  
  blocks = n / VVECTOR_BLKSIZE; 
  blocks++; // minimum of 1 block

  //  printf("blocks = %d\n", blocks);
  self -> n = n;
  self -> maxn = blocks * VVECTOR_BLKSIZE;
  self -> elm_size = elm_size;
  
  //  printf("elm_size = %d    maxn = %d    n = %d\n", elm_size, self->maxn, n);
  if (!(self -> data = (void *) calloc(self -> maxn, self -> elm_size))) {
    free(self);
    return NULL;
  }
  //  printf(" elm_size = %d\n", elm_size);
  return self;
}

/*
  Desallocates dynamic memory for an rvector object
*/
void vvector_destroy(vvector_t * self) {
#ifdef DEBUG
  int mem1;
  mem1 = mallinfo().arena;
#endif
  if (self->data) {
#ifdef DEBUG
    printf("destroying self->data %x\n", (int) self->data);
#endif
    free(self->data);
  }
#ifdef DEBUG
  printf("destroying self %x\n", (int) self);
#endif
  free(self);
#ifdef DEBUG
  printf("vvector_destroy: %d\n", mallinfo().arena - mem1);
#endif
}

/* returns a newly created vvector that is equals to self */
vvector_t * vvector_dup(vvector_t * self) {
  vvector_t * clone = vvector_alloc();
  memcpy(clone, self, sizeof(vvector_t));
  clone->data = malloc(self->n * self->elm_size);
  memcpy(clone->data, self->data, self->n * self->elm_size);
  return clone;
}

/* 
   Resize 'data'; 
   Reallocates 'data' accordling to new n.
   If the number of blocks must change, it realocates new buffer 
   and copies the older buffer to that.
*/
int vvector_resize(vvector_t *self, int newn) {
  int blocks, newblocks;
  int newmaxn, minn; // , i;
  void * newdata;

  if (!self) return 0;

  blocks = self->maxn/VVECTOR_BLKSIZE;
  newblocks = (newn+VVECTOR_BLKSIZE-1)/VVECTOR_BLKSIZE;
  if (!newblocks)
    newblocks = 1;
  newmaxn = newblocks*VVECTOR_BLKSIZE;

  minn = (newn > self->n) ? self->n : newn;
 
  //  printf("\nblocks %d, newblocks %d, n %d, newn %d, maxn %d, newmaxn %d, minn %d, data %x\n\n", blocks, newblocks, self->n, newn, self->maxn, newmaxn, minn, (int) self->data);

  if (blocks == newblocks) {
    self->n = newn;
    return 1;
  }
 
  if (!newmaxn || !(newdata = (void *) calloc(newmaxn, self->elm_size)))
    return 0;

  if (self->data && self->n) {
    memcpy(newdata, self->data, minn*self->elm_size);
    free(self->data);
  }

  self->data = newdata;

  self->n = newn;
  self->maxn = newmaxn;

  return 1;
}

/*
  Get a member from data and put it in &p.
  It returns the address of dest var.
*/
void * vvector_get(vvector_t * self, int iv, void * p) {
  int i = iv;
  void * pout;
  if (Debug) {
    printf("vvector_get: i = %d, Debug = %d\n", i, Debug);
  }
  i %= self->n;
  if (i < 0)
    i += self->n;
  if (Debug & 2)
    printf("vvector_get: i = %d\n", i);
  i *= self->elm_size;
  if (Debug & 2)
    printf("vvector_get: i * elm_size = %d\n", i * self -> elm_size);  
  pout = memcpy(p, self->data + i, self->elm_size);
  if (Debug & 4) {
    printf(" size of data = %d\n", self -> elm_size);
    if (self -> elm_size == sizeof(double))
      printf(" double value = %g\n", * (double *) pout);
    newgetline(stdin);
  }
  return pout;
}

/*
  Put a member from &p and put it in data
  It returns the addres of element in data buffer.
  It returns NULL in case of error.
*/
void * vvector_put(vvector_t * self, int iv, void * p) {
  int i = iv;
  if (Debug) {
    if (Debug & 2) {
      printf("vvector_put: Debug = %d -> i = %d\n", Debug, i);
    }      
    else if (i < 0 || i >= self -> n)
      printf("vvector_put: Debug = %d -> i = %d\n", Debug, i);
  }

  if (!self->n)
    return NULL;
  i %= self->n;
  if (i < 0)
    i += self->n;
  if (Debug & 2)
    printf("vvector_put: i = %d", i);
  i *= self->elm_size;
  if (Debug & 2)
    printf(", * elm_size: i = %d", i);
  if (Debug & 4) {
    switch (self -> elm_size) {
      case 1 :
	printf(", value = %u", * (uint8_t *) p);
	break;
      case 2 :
	printf(", value = %u", * (uint16_t *) p);
	break;
      case 4 :
	printf(", value = %u", * (uint32_t *) p);
	break;
      case 8 :
	fprintf(stdout, " double value = %g", * (double *) p);
	break;
    }
  }
  if (Debug)
    printf("\n");
  
  return (void *) memcpy(self->data + i, p, self->elm_size);
}

 
/*
  Append one more value in 'data' in an rvector object;
  increments 'n';
  Return 1 on success, 0 on error.
*/
int vvector_append(vvector_t * self, void * p) {
  if (self->n >= self->maxn) {
    if (!vvector_resize(self, self->n+1))
      return 0;
  }
  else
    self->n++;
  vvector_put(self, self->n-1, p);
  return 1;
}

/*
  Debug Facilities
*/
void vvector_setDebug(int debug_mode) {
  Debug = debug_mode;
}

int vvector_getDebug(void) {
  return Debug;
}
