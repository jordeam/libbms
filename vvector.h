#ifndef _VVECTOR_H
#define _VVECTOR_H

/* 
   Defining type 'rvec':
   maxn is the maximum number of elements of data. It is evaluated from
   VVECTOR_BLKSIZE, wich defines the size of each BLOCK;
   n is the actual number of elements in data;
*/

#define VVECTOR_BLKSIZE 1020

struct vvec {
  int n, maxn;
  void *data;
  int elm_size;
};

typedef struct vvec vvector_t;

/*
  Initialization and destruction
*/
vvector_t * vvector_alloc(void);
vvector_t * vvector_init(vvector_t * self, int n, int elm_size);
void vvector_destroy(vvector_t * self);
vvector_t * vvector_dup(vvector_t * self);

/*
  Accesing elements
*/
#define vvector_n(self) (self->n)
#define vvector_maxn(self) (self->maxn)
#define vvector_elm_size(self) (self->elm_size)
#define vvector_data(self) (self->data) /* rarely used... data is not accessed this way... */

/*
  Manipulating format and datas
*/
int vvector_resize(vvector_t *self, int n);
void * vvector_get(vvector_t * self, int i, void * p);
void * vvector_put(vvector_t * self, int i, void * p);
int vvector_append(vvector_t * self, void * p);

/*
  Debug
*/
void vvector_setDebug(int debug_mode);
int vvector_getDebug(void);

#endif
