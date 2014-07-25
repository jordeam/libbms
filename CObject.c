#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "CObject.h"

// #define DEBUG

// Object structure

const struct CObject _CObject = {
   OBJECT_IDENT, 
   (void *) & _CClass
};

const void * CObject = & _CObject;
  
// CClass structure

DeclareMessages(CClass) = {
  connect_mess(alloc, CClass),
  connect_mess(free, CClass),
  connect_mess(differ, CClass),
  connect_mess(clone, CClass),
  connect_mess(puto, CClass),
  connect_mess(negotiate, CClass),
  connect_mess(missing, CClass),
  connect_end
};

DeclareClassWOSuper(CClass);

// CClass method definitions

void * CClass_alloc(const void * self) {
  struct CClass * class = ((struct CObject *) self) -> class;

  void * p = calloc(1, class -> size);
  assert(p);
#ifdef DEBUG
  printf ("CClass_alloc: CObject size = %d, CClass size = %d\n", class -> size, sizeOf(CClass));
#endif
  ((struct CObject *) p) -> magic = OBJECT_IDENT;
  ((struct CObject *) p) -> class = (void *) class;
  //  printf("CClass_new called.\n");
  return p;
}

void CClass_free(const void * self) {
  free((void *) self);
}

int CClass_differ(const void * _self, void * b) {
  const struct CObject * self = _self;
  return (int) self - (int) b;
}

void * CClass_clone(const void * _self) {
  const struct CObject * self = _self;
  assert(self);
  return oox(self, new) EOX;
}

void CClass_puto(const void * _self, FILE * stream) {
  const struct CObject * self = _self;
  nameOf(self, stream);
  fprintf(stream, " at 0%08x", (unsigned int) self);
}

void * CClass_negotiate(const void * self, const char * message) {
  fprintf(stdout, "negotiate: Class ");
  nameOf(self, stdout); 
  fprintf(stdout, " does not respond to mess(%s)\n", message);
  return (void *) self;
}

void * CClass_missing(const void * self) {
  return (void *) self;
}

// Utilities:

size_t sizeOf(const void * _self) {
  const struct CObject * self = _self;
  const struct CClass * cp = (struct CClass *) self -> class;
  assert(self && cp);
  return cp -> size;
}

void nameOf(const void * _self, FILE * stream) {
  const struct CObject * self = _self;
  const struct CClass * class = self -> class;
  assert(self && class && class -> name);
  fprintf(stream, "%s", class -> name);
}

void * superOf(const void * _self) {
  const struct CObject * self = _self;
  const struct CClass * class = self -> class;
  return (void *) class -> super;
}

void * classOf(const void * _self) {
  const struct CObject * self = _self;
  return self -> class;
}

int isCObject(const void * self) {
  if (self)
    return * (int *) self == OBJECT_IDENT;
  return 0;
}

int isCClass(const void * class) {
  if (class)
    return * (int *) class == CLASS_IDENT;
  return 0;
}

// Message handlers

void * (* classmess2(const void * _self, const void * _super, const char * message))() {
  const struct CObject * self = _self;
  const struct CObject * super = _super;
  const struct CClass * class;
  struct Messages * mess;

#ifdef DEBUG
  printf("classmess2: self = "); nameOf(self, stdout); 
  if (super) {
    printf(" -- super: "); nameOf(super, stdout);
  }
  printf(" w/ message: %s\n", message);
#endif

  if (self && super && (class  = super -> class) && class -> messages)
    mess = class -> messages;
  else
    return NULL;

  while (mess->name) {
    if (strcmp(message, mess->name))
      mess++;
    else {
#ifdef DEBUG
      if (!mess -> method)
	printf("classmess2: message %s not found.\n", message);
      else
	printf("classmess2: message %s found.\n", message);
#endif
      
      return mess -> method;
    }
  }
#ifdef DEBUG
  printf("classmess2: looking for super.\n");
#endif
  if (super && superOf(super)) 
    return classmess2(self, superOf(super), message);
#ifdef DEBUG
#endif
  classmess2(self, self, mess(negotiate))(self, message);
  return classmess2(self, self, mess(missing));
}
