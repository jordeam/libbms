#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "LnElem.h"

#ifndef STRSIZE
#define STRSIZE 1024
#endif

@implementation LnElem

- (id) init:(id) data {

  Data = data;
  Next = NULL;

  return self;
}

- free {
  if (Data)
    [Data free];
  Data = NULL;
  Next = NULL;

  [super free];
  return self;
}

- (id) put:(id) data {

  if (Data)
    [Data free];
  Data = data;

  return self;
}

- (id) setNext:(id) next {

  Next = next;

  return self;
}

- (id) next {
  return Next;
}

- (id) data {
  return Data;
}

@end
