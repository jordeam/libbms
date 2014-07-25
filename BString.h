#ifndef _BSTRING_H
#define _BSTRING_H

#include <stdlib.h>
#include <stdarg.h>

#include "BObject.h"

struct BString {
  struct Object _;
  char * s;
};

void * BString_ctor(const void * _class, va_list * app);
void * BString_dtor(const void * self);
int BString_differ(const void * _self, const void * _b);
void * BString_clone(const void * self);

// accessed from messages:
static void BString_puts(const void * _class, va_list * app);

extern const void * BString;

#endif
