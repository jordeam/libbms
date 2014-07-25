#ifndef _CObject_h
#define _CObject_h

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

// Beginning of C Object Utilities

#define CLASS_IDENT  0xaafededa
#define OBJECT_IDENT 0xcedefeca

// Message handler
#define oox(obj, mess) classmess2(obj, obj, #mess)(obj
#define EOX )
#define ooxs(obj, super, mess) classmess2(obj, super, #mess)(obj
#define EOXS )

#define connect_mess(m, class) { #m , (void *) (* class ## _ ## m ) }
#define connect_end { NULL, NULL }
#define mess(message) #message

// Macros for method declarations automatization

#define DeclareMessages(class) const struct Messages msg ## class []

// The followinf macro needs to have SUPER defined
#define DeclareClassWOSuper(class) const struct CClass _ ## class = { \
  CLASS_IDENT, \
    (void *) & _ ## class, \
    #class, \
    NULL, \
    sizeof (struct class), \
    (void *) & msg ## class }; \
    const void * class = & _ ## class

#define DeclareClassWSuper(class, super) const struct CClass _ ## class = { \
  CLASS_IDENT, \
    (void *) & _ ## class, \
    #class, \
    (void *) & _ ## super, \
    sizeof (struct class), \
    (void *) & msg ## class }; \
    const void * class = & _ ## class

#define _NULL NULL // workaround for super = NULL

// Message handlers
void * (* classmess2(const void * _self, const void * _class
		  , const char * message))();

// utilities
void nameOf(const void * _self, FILE * stream);
void * classOf(const void * _self);
void * superOf(const void * _self);
size_t sizeOf(const void * _self);
int isObject(const void * self);
int isClass(const void * class);

// end of C Object Utilities

extern const struct CClass _CClass;
extern const struct CObject _CObject;

extern const void * CClass;
extern const void * CObject;

struct CObject {
  unsigned long magic;
  void * class;
};

extern const void * CObject;

struct Messages {
  char * name;
  void * (* method) (const void * _self, va_list * app);
};

struct CClass {
  //  struct CObject _; -> substuted by two lines, to supress warnings
  unsigned long magic;
  void * class;
  const char * name;
  void * super;
  int size;
  struct Messages * messages;
};

extern const void * CClass;

// Factory messages of CClass:
void * CClass_alloc(const void * _self);

// CObject messages:
void CClass_free(const void * _self);
void * CClass_clone(const void * _self);
int CClass_differ(const void * _self, void * b);
void CClass_puto(const void * _self, FILE * stream);
void * CClass_negotiate(const void * self, const char * message);
void * CClass_missing(const void * self);

#endif
