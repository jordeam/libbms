#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "LElem.h"
#include "LList.h"

#define STRSIZE 1024

struct LList {
  struct Object _;              // super class
  void * first;                   // object attached
  void * current;               // next element
};

struct Messages msgLList [] = {
  connect_mess(new, LList),
  connect_mess(delete, LList),
  connect_mess(append, LList),
  connect_mess(top, LList),
  connect_mess(next, LList),
  connect_mess(current, LList),
  connect_mess(get_data, LList),
  connect_mess(get_last, LList),
  connect_mess(n, LList),
  connect_end
};

struct Class _LList = {
  { CLASS_IDENT, (void *) & _LList }, // MAGIC , self class
  "LList",                            // name
  & _Object,                              // super
  sizeof (struct LList),              // size
  (void *) & msgLList,                // messages table
};

const void * LList = & _LList;

void * methodd(new, LList) {
  const void * class = _self;
  struct LList * self = (void *) _self;

  //  printf("Inside ad new\n");

  assert(self = classmess(class, superOf(class), mess(new), app));

  self -> first = NULL;
  self -> current = NULL;

  return (void *) self;
}

void methodd(delete, LList) {
  struct LList * self = (void *) _self;
  void * next, * current;

  assert(self);

  if (self -> first) {
    next = self -> first;
    while ((current = next))
      next = ooc(current, mess(next));             // Message to LElem

    self -> first = NULL;
    self -> current = NULL;
  }

  classmess(self, superOf(self), mess(delete), NULL);
}

void * methodd(top, LList) {
   struct LList * self = (void *) _self;

   assert(self);
   self -> current = self -> first;
   return (void *) self -> current;
}

void * methodd(next, LList) {
   struct LList * self = (void *) _self;
   
   assert(self);

   if(self -> current)
     self -> current = ooc(self -> current, mess(next));
   return (void *) self -> current;
}

void * methodd(current, LList) {
   const struct LList * self = _self;

   assert(self);
   return (void *) self -> current;
}

int methodd(n, LList) {
   const struct LList * self = _self;
   void * current;
   int n = 0;

   assert(self);

   for(current = self -> first;
       current; 
       current = ooc(current, mess(next)))   // message to LElem
     n++;
   
   return n;
}

void * methodd(get_last, LList) {
  const struct LList * self = _self;
  void * up_last, * current;
  
  assert(self);
  
  
  for(up_last = current = self -> first;
      current; 
      current = ooc(current, mess(next)))
    up_last = current;
  //  printf("LList_get_last: up_last = 0x%08x\n", up_last);
  return up_last;
}

void * methodd(append, LList) {
  struct LList * self = (void *) _self;
  const void * p = va_arg(* app, const void *);
  void * last;
  void * lelem = ooc(LElem, mess(new), p);

  assert(self);
  
  if (! self -> first)
    last = self -> current = self -> first = lelem;
  else {
    last = ooc(self, mess(get_last));                      // último elemento
    ooc(last, mess(set_next), lelem); // insere p
  }
  
  return last;
}

void * methodd(get_data, LList) {
  const struct LList * self = _self;

  assert(self && self -> current);
    
  return ooc(self -> current, mess(data));
}
