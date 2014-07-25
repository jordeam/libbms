#ifndef _BOBJECT_H
#define _BOBJECT_H

#include <stdlib.h>
#include <stdarg.h>

#include "BOUtils.h"

extern const struct Class _Class;
extern const struct Object _Object;

extern const void * Class;
extern const void * Object;

struct Object {
  unsigned long magic;
  void * class;
};

struct Messages {
  char * name;
  void * (* method) (const void * _self, va_list * app);
};

struct Class {
  struct Object _;
  const char * name;
  const void * super;
  int size;
  struct Messages * messages;
};

extern const void * Class;
extern const void * Object;

// Factory messages of Class:
void * methodd(new, Class);

// Object messages:
void methodd(delete, Class);
void * methodd(clone, Class);
int methodd(differ, Class);
void methodd(puto, Class);

#endif
