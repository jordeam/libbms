#include <publib.h>
#include <stdlib.h>
#include <string.h>


#include "asmline.h"
#include "strbms.h"

static int comment_pos(const char * s, char sep) {
  int i;
  int inside_dquotes = 0, inside_squote = 0, was_backslash = 0;
  for (i = 0; i < strlen(s); i++) {
    if (s[i] == '\\') {
      was_backslash ^= 1;
      continue;
    }
    if (s[i] == sep && !inside_dquotes && !inside_squote && !was_backslash)
      break;
    if (s[i] == '\'' && !was_backslash && !inside_dquotes)
      inside_squote ^= 1;
    if (s[i] == '\"' && !was_backslash && !inside_squote)
      inside_dquotes ^= 1;
    was_backslash = 0;
  }
  return i;
}

asmline * asmline_alloc(void) {
  return (asmline *) calloc(sizeof(asmline), 1);
}

asmline * asmline_put(asmline * self, const char * s) {
  int c_pos, i_end, i;
  char * ss;

  // initializing fields
  asmline_clear(self);
  
  // comment
  c_pos = comment_pos(s, ';');
  //  printf("%d len(s)=%d\n", c_pos, strlen(s));
  self->comment = strdup(s + c_pos);
  chomp(self->comment);

  // elimining comment
  ss = strndup(s, c_pos);
  //  printf("ss = [%s]\n", ss);

  // label
  i_end = strcspn(ss, " \t\n");
  if (i_end > strlen(ss))
    goto returning;
  self->label = strndup(ss, i_end);

  // opcode
  i = strspn(ss+i_end, " \t\n");
  i += i_end; 
  //  printf("{%d,%d,[%s]}\n", i, i_end, ss+i);
  i_end = strcspn(ss+i, " \t\n");

  if (i_end > strlen(ss))
    goto returning;
  self->opcode = strndup(ss+i, i_end);

  // args
  i += i_end; //= strspn(ss+i+i_end, " \t\n");
  if (i > strlen(ss))
    goto returning;
  self->args = strdup(ss+i);
  //  printf("Before strtrim = [%s]\n", self->args);
  strtrim(self->args);
  //  printf("After  strtrim = [%s]\n", self->args);
  

 returning:
  free(ss);
  return self;
}
    
int asmline_get_arg(asmline * self, int n_arg, char * sout, int maxsize) {
  int n = 0, pos = 0, opos = 0, correct = 0;
  char * ss;

  if (!strlen(self->args))
    return 0;
  while (n_arg != n && pos < strlen(self->args)) {
    pos += comment_pos(self->args+pos, ',');
    //    printf("pos = %d, char = %c\n", pos, *(self->args+pos));
    pos++;
    n++;
  }
  opos = pos;
  if (strlen(self->args+pos) > 0) {
    pos += comment_pos(self->args+pos, ',');
    if (pos != opos)
      correct = 1;
    if (*(self->args+pos) == ',')
      pos--;
  }
  else pos = strlen(self->args);

  if (pos - opos > maxsize)
    return 0;
  if (n_arg != n) {
    //    printf("n_arg != n\n");
    return 0;
  }

  if (pos < opos) {
    //    printf ("pos < opos\n");
    pos = opos;
  }

  ss = strndup(self->args+opos, pos - opos + correct);
  strtrim(ss);

  strcpy(sout, ss);

  //  printf("n = %d, narg = %d -- pos = %d, opos = %d, [%s]\n", n, n_arg, pos, opos, ss);

  free(ss);
  return 1;
}

int asmline_get_label(asmline * self, char * sout, int maxsize) {
  if (self->label && strlen(self->label) < maxsize) {
    strcpy(sout, self->label);
    return strlen(self->label);
  }
  else
    return 0;
}

int asmline_get_opcode(asmline * self, char * sout, int maxsize) {
  if (self->opcode && strlen(self->opcode) < maxsize) {
    strcpy(sout, self->opcode);
    return strlen(self->opcode);
  }
  else
    return 0;
}

int asmline_get_args(asmline * self, char * sout, int maxsize) {
  if (self->args && strlen(self->args) < maxsize) {
    strcpy(sout, self->args);
    return strlen(self->args);
  }
  else
    return 0;
}

int asmline_nargs(asmline * self) {
  int n = 1, pos = 0;
  if (!strlen(self->args))
    return 0;
  while (pos < strlen(self->args)) {
    pos += comment_pos(self->args+pos, ',');
    //    printf("pos = %d, char = %c\n", pos, *(self->args+pos));
    if(*(self->args+pos) == ',')
      n++;
    pos++;
  }
  return n;
}

int asmline_get_comment(asmline * self, char * sout, int maxsize) {
  if (self->comment && strlen(self->comment) < maxsize) {
    strcpy(sout, self->comment);
    return strlen(self->comment);
  }
  else
    return 0;
}

asmline * asmline_clear(asmline * self) {
  if (self->label) free(self->label);
  if (self->opcode) free(self->opcode);
  if (self->args) free(self->args);
  if (self->comment) free(self->comment);

  self->label = NULL;
  self->opcode = NULL;
  self->args = NULL;
  self->comment = NULL;
  return self;
}

void asmline_destroy(asmline * self) {
  asmline_clear(self);
  free(self);
}
   
