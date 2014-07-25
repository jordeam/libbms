#ifndef _LLIST_H
#define _LLIST_H

#include <stdlib.h>
#include <stdarg.h>

#include "BObject.h"

// factory methods
void * methodd(new, LList);

// common methods
void methodd(delete, LList);

void * methodd(append, LList);
void * methodd(top, LList);
void * methodd(next, LList);
void * methodd(current, LList);
void * methodd(get_last, LList);
void * methodd(get_data, LList);
int methodd(n, LList);

extern const void * LList;

#endif
