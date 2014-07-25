#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include <BObject.h>

// Class structure

const struct Object _Object = {
   OBJECT_IDENT, 
   (void *) & _Class
};
  
const struct Messages msgClass [] = {
  connect_mess(new, Class),
  connect_mess(delete, Class),
  connect_mess(differ, Class),
  connect_mess(clone, Class),
  connect_mess(puto, Class),
  connect_end
};

const struct Class _Class = {
  { CLASS_IDENT, 
   (void *) & _Class  },             // self class
  "Class",                   // name
  NULL,                      // super
  sizeof (struct Class),     // size
  (void *) & msgClass,       // messages
};

const void * Class = & _Class;
const void * Object = & _Object;

// Class method definitions

void * methodd(new, Class) {
  const struct Object * self = _self;
  struct Class * class = self -> class;

  void * p = calloc(1, class -> size);
  assert(p);
#ifdef DEBUG
  printf ("Class_new: Object size = %d, Class size = %d\n", class -> size, sizeOf(Class));
#endif
  ((struct Object *) p) -> magic = OBJECT_IDENT;
  ((struct Object *) p) -> class = (void *) class;
  //  printf("Class_new called.\n");
  return p;
}

void methodd(delete, Class) {
  const struct Object * self = _self;
  //  const struct Class * cp = (struct Class *) self -> class;
  
  //  if (self && cp && cp -> dtor)
  //    self = cp -> dtor(self);
  free((void *) self);
}

int methodd(differ, Class) {
  const struct Object * self = _self;
  argva(void *, b);

  return (int) self - (int) b;
}

void * methodd(clone, Class) {
  const struct Object * self = _self;
  assert(self);
  return ooc(self, mess(new));
}

void methodd(puto, Class) {
  const struct Object * self = _self;
  argva(FILE *, stream);

  nameOf(self, stream);
  fprintf(stream, " at 0%08x", (unsigned int) self);
}


// Utilities:

size_t sizeOf(const void * _self) {
  const struct Object * self = _self;
  const struct Class * cp = (struct Class *) self -> class;
  assert(self && cp);
  return cp -> size;
}

void nameOf(const void * _self, FILE * stream) {
  const struct Object * self = _self;
  const struct Class * class = self -> class;
  assert(self && class && class -> name);
  fprintf(stream, "%s", class -> name);
}

void * superOf(const void * _self) {
  const struct Object * self = _self;
  const struct Class * class = self -> class;
  return (void *) class -> super;
}

void * classOf(const void * _self) {
  const struct Object * self = _self;
  return self -> class;
}

int isObject(const void * self) {
  if (self)
    return * (int *) self == OBJECT_IDENT;
  return 0;
}

int isClass(const void * class) {
  if (class)
    return * (int *) class == CLASS_IDENT;
  return 0;
}

// Message handlers

void * omess(const void * _self, const char * message, ...) {
  const struct Object * self = _self;
  const struct Class * class = (struct Class *) self -> class;
  void * object = NULL;
  //  printf("Aqui?\n");
  if (self && class) {
    va_list ap;
    va_start(ap, message);
    //      printf("self = %08x name = ", (unsigned int) self);
    //      nameOf(self, stdout);
    //      printf("\n");
    object = classmess(self, self, message, &ap);
    //    printf("Aqui 2 ...\n");
    va_end(ap);
  }
  return object;
}

void * ooc(const void * _self, const char * message, ...) {
  const struct Object * self = _self;
  const struct Class * class = (struct Class *) self -> class;
  void * object = NULL;
  if (self && class) {
    va_list ap;
    va_start(ap, message);
    object = classmess(self, self, message, &ap);
    va_end(ap);
  }
  return object;
}

void * classmess(const void * _self, const void * _super, const char * message, va_list * app) {
  const struct Object * self = _self;
  const struct Object * super = _super;
  const struct Class * class;
  //  const void * object = NULL;
  struct Messages * mess;

#ifdef DEBUG
  printf("classmess: self = "); nameOf(self, stdout); 
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
      if (mess -> method)
	return mess -> method(self, app);
#ifdef DEBUG
      printf("classmess: message %s not found.\n", message);
#endif
      return NULL;
    }
  }
#ifdef DEBUG
  printf("calssmess: calling super.\n");
#endif
  if (super && superOf(super)) 
    return classmess(self, superOf(super), message, app);
  return NULL;
}

