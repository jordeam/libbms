#ifndef _LELEM_H
#define _LELEM_H

#include <stdlib.h>
#include <stdarg.h>

#include "BObject.h"

// factory meethods
void * methodd(new, LElem);

// common methods
void methodd(delete, LElem);

void * methodd(put, LElem);
void * methodd(data, LElem);
void * methodd(next, LElem);
void * methodd(set_next, LElem);

extern const void * LElem;

#endif
