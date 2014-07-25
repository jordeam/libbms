#ifndef _LELEM_H
#define _LELEM_H

#include <stdlib.h>
#include <stdarg.h>

#include "objc/Object.h"

@interface LnElem : Object {
  id Data;                   // object attached
  id Next;                   // next element
}

- free;

- (id) init:(id) data;
- (id) put:(id) data;
- (id) data;
- (id) next;
- (id) setNext:(id) next;

@end

#endif
