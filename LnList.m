#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "LnElem.h"
#include "LnList.h"

#ifndef STRSIZE
#define STRSIZE 1024
#endif

@implementation LnList 

- (id) init {

  First = NULL;
  Current = NULL;

  return self;
}

- (void) free {
  id next, current;

  if (First) {
    next = First;
    while ((current = next))
      next = [current next];             // Message to LElem

    First = NULL;
    Current = NULL;
  }
  [super free];
}

- (id) top {
  Current = First;
  return Current;
}

- (id) next {

   if(Current)
     Current = [Current next];
   return Current;
}

- (id) current {
  return Current;
}

- (int) n {
  id current;
  int n = 0;

  for(current = First;
      current; 
      current = [current next])   // message to LElem
    n++;
  return n;
}

- (id) getLast {
  id up_last, current;
  
  for(up_last = current = First;
      current; 
      current = [current next])
    up_last = current;
  return up_last;
}

- (id) append:(id) data {
  id last;
  id lelem = [[LnElem alloc] init:data];

  if (!First)
    last = Current = First = lelem;
  else {
    last = [self getLast];                      // último elemento
    [last setNext:lelem];                       // insere data
  }
  return last;
}

- (id) getData {    
  return [Current data];
}

@end
