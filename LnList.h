#ifndef _LLIST_H
#define _LLIST_H

#include <stdlib.h>
#include <stdarg.h>

#include "objc/Object.h"

@interface LnList : Object {
  id First;                   // object attached
  id Current;               // next element
}

- (id) init;
- (void) free;
- (id) top;
- (id) next;
- (id) current;
- (int) n;
- (id) getLast;
- (id) append:(id) data;
- (id) getData;

@end

#endif
