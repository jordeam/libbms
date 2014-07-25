#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "LElem.h"

#define STRSIZE 1024

struct LElem {
  struct Object _;           // super class
  void * p;                  // object attached
  void * next;               // next element
};

struct Messages msgLElem [] = {
  connect_mess(new, LElem),
  connect_mess(delete, LElem),
  connect_mess(put, LElem),
  connect_mess(data, LElem),
  connect_mess(next, LElem),
  connect_mess(set_next, LElem),
  connect_end
};

struct Class _LElem = {
  { CLASS_IDENT, (void *) & _LElem }, // MAGIC , self class
  "LElem",                            // name
  & _Object,                              // super
  sizeof (struct LElem),              // size
  (void *) & msgLElem,                // messages table
};

const void * LElem = & _LElem;

void * methodd(new, LElem) {
  const void * class = _self;
  const void * p = va_arg(* app, const void *);

  struct LElem * self;

  assert(self = classmess(class, superOf(class), mess(new), app));

  self -> p = (void *) p;
  self -> next = NULL;

  return (void *) self;
}

void methodd(delete, LElem) {
  struct LElem * self = (void *) _self;

  assert(self);

  if (self -> p)
    ooc(self -> p, mess(delete));
  self -> p = NULL;
  self -> next = NULL;

  classmess(self, superOf(self), mess(delete), NULL);
}

void * methodd(put, LElem) {
  struct LElem * self = (void *) _self;
  const void * p = va_arg(* app, const void *);

  assert(self);

  if (self -> p)
    ooc(self -> p, mess(delete));
  self -> p = (void *) p;

  return (void *) self;
}

void * methodd(set_next, LElem) {
  struct LElem * self = (void *) _self;
  const void * next = va_arg(* app, const void *);

  assert(self);

  self -> next = (void *) next;

  return (void *) self;
}

void * methodd(next, LElem) {
   const struct LElem * self = _self;
   
   assert(self);
   return (void *) self -> next;
}

void * methodd(data, LElem) {
   const struct LElem * self = _self;
   
   assert(self);
   return self -> p;
}
