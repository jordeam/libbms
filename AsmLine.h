#ifndef _ASMLINE_H
#define _ASMLINE_H

#include "objc/Object.h"

@interface AsmLine : Object {
  char * Label, * Opcode, * Args, * Comment;
  int SmartLabel;
}

- (id) put:(const char *) s;
- (id) init: (const char *) s SmartLabel: (int) smart;
- free;
- (int) getLabel:(char *) sout Max:(int) maxsize;
- (int) getLabel:(char *) sout;
- (int) getOpcode:(char *) sout Max:(int) maxsize;
- (int) getOpcode:(char *) sout;
- (char *) getOpcode;
// - (int) getArgs:(char *) sout Max:(int) maxsize;
- (int) getArgs:(char *) sout;
- (char *) getArgs;
- (id) putArgs:(char *) sin;
- (int) getArg:(int) n_arg :(char *) sout Max:(int) maxsize;
// - (int) getArg:(int) n_arg :(char *) sout;
- (int) NArgs;
- (int) getComment:(char *) sout :(int) maxsize;
- (int) getComment:(char *) sout;
- (id) clear;

- (char *) Label;
- (char *) Opcode;
- (char *) Args;
- (char *) Comment;

@end

#endif
