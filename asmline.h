#ifndef _ASMLINE_H
#define _ASMLINE_H

typedef struct {
  char * label, * opcode, * args, * comment;
} asmline;

asmline * asmline_alloc(void);

#define asmline_init asmline_put
#define asmline_new(s) asmline_init(asmline_alloc(), s)
void asmline_destroy(asmline * self);

int asmline_get_label(asmline * self, char * sout, int maxsize);
int asmline_get_opcode(asmline * self, char * sout, int maxsize);

int asmline_get_args(asmline * self, char * sout, int maxsize);
int asmline_get_arg(asmline * self, int n_arg, char * sout, int maxsize);
int asmline_nargs(asmline *self);

int asmline_get_comment(asmline * self, char * sout, int maxsize);

asmline * asmline_put(asmline * self, const char * s);
asmline * asmline_clear(asmline * self);
#endif
