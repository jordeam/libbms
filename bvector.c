#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>

#include "constant.h"
#include "sysconst.h"
#include "bvector.h"

// #define DEBUG

/*
  Identical methods
*/

/*
  Callback methods
*/

/*
  Get an element from data buffer.
*/
uint8_t bvec_get(bvector * self, int i) {
  uint8_t x;
  vvector_get(self, i, &x);
  return x;
}

/*
  Get an element as a word from data buffer.
*/
uint32_t bvec_getw(bvector * self, int i) {
  uint8_t xh = 0, xl = 0;
  if (self -> n > i) {
    vvector_get(self, i, & xh);
    if (self -> n > i + 1) 
      vvector_get(self, i + 1, & xl);
  }

  return (xh << 8) + xl;
}

/*
  Put an element in data
*/
int bvec_put(bvector * self, int i, uint8_t x) {
  /* Round i in n */
  if (i >=0 && i < self->n) {
    ((uint8_t *) self->data)[i] = x;
    return 1;
  }
  else
    /* else, do not put anything */
    return 0;
}

/*
  Append an element in data
*/
int bvec_append(bvector * self, uint8_t x) {
  int _r;
#ifdef DEBUG
  printf("bvec_append: %02x\n", x);
#endif
  _r = vvector_append(self, &x);
#ifdef DEBUG
  printf("bvec_append: end\n");
#endif
  return _r;
}

int bvec_appendw(bvector * self, uint32_t x) {
  uint8_t xl, xh;
  xl = x & 0xff;
  xh = (x >> 8) & 0xff;
  return vvector_append(self, &xh) + vvector_append(self, &xl);
}

/*+
  Print elements of a bvector in ascii format (binary values).
*/
void bvec_afputs(bvector * self, FILE * stream) {
  int i;
  char s[10];

  for (i = 0; i < bvec_n(self); i++) {
    sprintf(s, "%02x", bvec_get(self, i));
    fputs(s, stream);
    if (i < bvec_n(self)-1)
      fputs(" ", stream);
  }
}

/*+
  Print elements of a bvector in stream.
*/
void bvec_fputs(bvector * self, FILE * stream) {
  int i;

  for (i = 0; i < bvec_n(self); i++) {
#ifdef DEBUG
    printf("[%d]:0x%x\n", i, (int) bvec_get(self, i));
#endif
    putc((int) bvec_get(self, i), stream);
  }
}

/*+
  Read elements from a NULL terminated string and put it in a vector
  +*/
void bvec_getFromStr(bvector * self, const char * s) {
  int i;

  bvec_resize(self, 0);

  for (i = 0; i < strlen(s); i++) {
    bvec_append(self, s[i]);
  }
}

/*+
  Read elements from an ascii string and put it in a vector
  +*/
void bvec_getFromASCIIStr(bvector * self, const char * s) {
  int i, j, ic, ssize;
  char * so, * sn, * si, *so1;

  ssize = strlen(s);
  so = malloc(ssize+1);
  so1 = malloc(2*ssize+1);

  j = 0;
  for (i = 0; i < ssize; i++) {
    if (isxdigit(s[i]))
      so[j++] = s[i];
    else
      so[j++]=':';
  }
  
  so[j] = '\0';
  
#ifdef DEBUG
  printf("%s\n", so);
#endif

  bvec_clear(self);

  si = so;
  sn = strsep(&si, ":");
  *so1 = '\0';
  j = 0;
  while (sn) {
    if (strlen(sn)) {
#ifdef DEBUG
      printf("[%s]", sn);
#endif
      if (j) so1[j++] = ':';
      if (strlen(sn) % 2) {
	so1[j++] = '0';
	ic = 1;
      }
      else
	ic = 0;
      for (i = 0; i < strlen(sn); i++) {
	if (ic++ == 2) {
	  so1[j++] = ':';
	  ic = 1;
	}
	so1[j++] = sn[i];
      }
    }
    sn = strsep(&si, ":");
  }
  so1[j] = '\0';

#ifdef DEBUG
  printf("\n[%s]\n", so1);
#endif

  si = so1;
  while ((sn = strsep(&si, ":"))) {
    i = strtoul(sn, NULL, 16);
    if (strlen(sn)) {
#ifdef DEBUG
      printf("[%s.%02x]", sn, i);
#endif
      bvec_append(self, i);
    }
  }
#ifdef DEBUG
  printf("\n");
#endif

  free(so);
  free(so1);
}

/*+
  Copy object source into dest.
*/
bvector * bvec_copy(bvector * dest, bvector * source) {

  if (dest->data) {
    free(dest->data);
    dest->data = NULL;
  }

  memcpy(dest, source, sizeof(bvector));

  if (source->data) {
    dest->data = malloc(bvec_n(source)*sizeof(uint8_t));
    memcpy(dest->data, source->data, bvec_n(source)*sizeof(uint8_t));
  }

  return dest;
}

/*+
  Evaluates checksum for vector in 8 bit format
  +*/
int bvec_CKS8(bvector * self) {
  int soma = 0, i;

  for (i = 0; i < bvec_n(self); i++) {
    soma += bvec_get(self, i);
    soma &= 0xff;
  }

  return soma ^ 0xff;
}

/*+
  Evaluates checksum for vector in 16 bit format
  +*/
int bvec_CKS16(bvector * self) {
  int soma = 0, i;

  for (i = 0; i < bvec_n(self); i++) {
    soma += bvec_get(self, i);
    soma &= 0xffff;
  }

  return soma ^ 0xffff;
}

/*+
  Put the checksum in 8 bit format in the end of data buffer
  +*/
void bvec_putCKS8(bvector * self) {
  int chk;

  chk = bvec_CKS8(self);
  bvec_append(self, chk);
}

/*+
  Put the checksum in 16 bit format in the end of data buffer
  +*/
void bvec_putCKS16(bvector * self) {
  int chk;

  chk = bvec_CKS16(self);
  bvec_append(self, (chk >> 8) & 0xff);
  bvec_append(self, chk & 0xff);
}

/*+
  Verify if the checksum is ok (8 bit)
  +*/
int bvec_verCKS8(bvector * self) {
  int soma = 0, i;

  for (i = 0; i < bvec_n(self) - 1; i++) {
    soma += bvec_get(self, i);
    soma &= 0xff;
  }

  if (bvec_n(self) < 2)
    return 0;
  return (soma ^0xff) == bvec_get(self, bvec_n(self)-1);
}

/*+
  Verify if the checksum is ok (16 bit)
  +*/
int bvec_verCKS16(bvector * self) {
  int soma = 0, i;
  
  for (i = 0; i < bvec_n(self) - 1; i++) {
    soma += bvec_get(self, i);
    soma &= 0xffff;
  }
  
  if (bvec_n(self) < 3)
    return 0;
  return (soma ^ 0xffff) == 
    (bvec_get(self, bvec_n(self)-2) << 8) + bvec_get(self, bvec_n(self)-1);
}


