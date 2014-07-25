#ifndef _BOUtils_h
#define _BOUtils_h

#include <stdlib.h>
#include <stdarg.h>

#define CLASS_IDENT  0xcacaface
#define OBJECT_IDENT 0xb0b0face

#define connect_mess(m, class) { #m , (void *) (* ## class ## _ ## m ## ) }
#define connect_end { NULL, NULL }
#define mess(message) #message

// Macros for method declarations automatization

#define methodd(message, class) class ## _ ## message ## \
                                (const void * _self, va_list * app)
#define argva(type, var) type ## var ## = va_arg(* app, ## type ## )


// Message handlers
void * omess(const void * _self, const char * message, ...);
void * ooc(const void * _self, const char * message, ...);
void * classmess(const void * _self, const void * _class,
		 const char * message, va_list * app);

// utilities
void nameOf(const void * _self, FILE * stream);
void * classOf(const void * _self);
void * superOf(const void * _self);
size_t sizeOf(const void * _self);
int isObject(const void * self);
int isClass(const void * class);

#endif
