#include <stdlib.h>
#include <string.h>

#include "AsmLine.h"
#include "strss.h"

// #define DEBUG

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

@implementation AsmLine 

- (id) init: (const char *) s SmartLabel: (int) smart {
  SmartLabel = smart;
  return [self put: s];
}

- (id) put:(const char *) s {
  int c_pos, i_end, i;
  char * ss;

  // initializing fields
  [self clear];
  
  // comment
  c_pos = comment_pos(s, ';');
  //  printf("%d len(s)=%d\n", c_pos, strlen(s));
  Comment = strdup(s + c_pos);
  chomp(Comment);

#ifdef DEBUG
  printf("Comment = %s\n", Comment);
#endif

  // elimining comment
  ss = (char *) strndup(s, c_pos);
  //  printf("ss = [%s]\n", ss);
  chomp(ss);

#ifdef DEBUG
  printf("Without Comment = %s\n", ss);
#endif

  // label
  i_end = strcspn(ss, " \t\n\r");
  if (i_end > strlen(ss))
    goto returning;
  Label = (char *) strndup(ss, i_end);
  i = i_end;
#ifdef DEBUG
  printf("After Label, i = %d\n", i);
#endif
  if (SmartLabel) {
    if(!strlen(Label)) {
      i += strspn(ss+i, " \t\n\r");
#ifdef DEBUG
      printf("Before empty Label i = %d\n", i);
#endif      
      i_end = strcspn(ss+i, " \t\n\r");
      
      
      if (i_end >= strlen(ss))
	goto returning;
      Label = (char *) strndup(ss+i, i_end);
      i += i_end;
#ifdef DEBUG
      printf("After empty Label, i = %d\n", i);
#endif
    }
    // labels with ':' in the end
    if (strlen(Label) && Label[strlen(Label)-1] != ':') {
      Opcode = Label;
      Label = strdup("");
#ifdef DEBUG
      printf("After Label to Opcode i = %d\n", i);
#endif
    }
  }

  // opcode
  if (!Opcode) {
    i_end = strspn(ss+i, " \t\n\r");
    i += i_end; 
    //  printf("{%d,%d,[%s]}\n", i, i_end, ss+i);
    i_end = strcspn(ss+i, " \t\n\r");

    if (i_end >= strlen(ss))
      goto returning;
    Opcode = (char *) strndup(ss+i, i_end);
    i += i_end;
  }

#ifdef DEBUG
  printf("After opcode i_end = %d, i = %d\n", i_end, i);
#endif
  // args
  //  i += i_end; //= strspn(ss+i+i_end, " \t\n\r");
  if (i >= strlen(ss))
    goto returning;
  Args = strdup(ss+i);
  //  printf("Before strtrim = [%s]\n", Args);
  strtrim(Args);
  //  printf("After  strtrim = [%s]\n", Args);
  

 returning:
  //  printf("Appending:\nLabel   [%s]\nOpcode  [%s]\nArgs    [%s]\nComment [%s]\n", Label, Opcode, Args, Comment);

  if (!Label) Label = strdup("");
  if (!Opcode) Opcode = strdup("");
  if (!Args) Args = strdup("");
  if (!Comment) Comment = strdup("");
  free(ss);
  return self;
}
    
- (int) getArg:(int) n_arg :(char *) sout Max:(int) maxsize {
  int n = 0, pos = 0, opos = 0, correct = 0;
  char * ss;
  int len;
  if (!strlen(Args))
    return 0;
  while (n_arg != n && pos < strlen(Args)) {
    pos += comment_pos(Args+pos, ',');
    //    printf("pos = %d, char = %c\n", pos, *(Args+pos));
    pos++;
    n++;
  }
  opos = pos;
  if (strlen(Args+pos) > 0) {
    pos += comment_pos(Args+pos, ',');
    if (pos != opos)
      correct = 1;
    if (*(Args+pos) == ',')
      pos--;
  }
  else pos = strlen(Args);

  if (maxsize > 0 && pos - opos > maxsize)
    return 0;
  if (n_arg != n) {
    //    printf("n_arg != n\n");
    return 0;
  }

  if (pos < opos) {
    //    printf ("pos < opos\n");
    pos = opos;
  }

  ss = (char *) strndup(Args+opos, pos - opos + correct);
  strtrim(ss);

  strcpy(sout, ss);
#ifdef DEBUG
  printf("getArg: n = %d, narg = %d -- pos = %d, opos = %d, [%s]\n", n, n_arg, pos, opos, ss);
#endif
  len = strlen(ss);
  free(ss);
  return len;
}

- (int) getLabel:(char *) sout Max:(int) maxsize {
  if (Label && strlen(Label) < maxsize) {
    strcpy(sout, Label);
    return strlen(Label);
  }
  strcpy(sout, "");
  return 0;
}

- (int) getLabel:(char *) sout {
  if (Label) {
    strcpy(sout, Label);
    return strlen(Label);
  }
  strcpy(sout, "");
  return 0;
}

- (void) putLabel:(char *) sin {
  if (Label)
    free(Label);
  Label = strdup(sin);
}

- (char *) getOpcode {
  return Opcode;
}

- (int) getOpcode:(char *) sout Max:(int) maxsize {
  if (Opcode && strlen(Opcode) < maxsize) {
    strcpy(sout, Opcode);
    return strlen(Opcode);
  }
  strcpy(sout, "");
  return 0;
}

- (int) getOpcode:(char *) sout {
  if (Opcode) {
    strcpy(sout, Opcode);
    return strlen(Opcode);
  }
  strcpy(sout, "");
  return 0;
}

- (int) getArgs:(char *) sout :(int) maxsize {
  if (Args && strlen(Args) < maxsize) {
    strcpy(sout, Args);
    return strlen(Args);
  }
  strcpy(sout, "");
  return 0;
}

- (int) getArgs:(char *) sout {
  if (Args) {
    strcpy(sout, Args);
    return strlen(Args);
  }
  strcpy(sout, "");
  return 0;
}

- (char *) getArgs {
  return Args;
}

- (id) putArgs:(char *) sin {
  if (Args)
    free(Args);
  Args = strdup(sin);
  return self;
}

- (int) NArgs {
  int n = 1, pos = 0;
  if (!strlen(Args))
    return 0;
  while (pos < strlen(Args)) {
    pos += comment_pos(Args+pos, ',');
    //    printf("pos = %d, char = %c\n", pos, *(Args+pos));
    if(*(Args+pos) == ',')
      n++;
    pos++;
  }
  return n;
}

- (int) getComment:(char *) sout :(int) maxsize {
  if (Comment && strlen(Comment) < maxsize) {
    strcpy(sout, Comment);
    return strlen(Comment);
  }
  strcpy(sout, "");
  return 0;
}

- (int) getComment:(char *) sout {
  if (Comment) {
    strcpy(sout, Comment);
    return strlen(Comment);
  }
  strcpy(sout, "");
  return 0;
}

- (id) clear {
  if (Label) free(Label), Label = NULL;
  if (Opcode) free(Opcode), Opcode = NULL;
  if (Args) free(Args), Args = NULL;
  if (Comment) free(Comment), Comment = NULL;

  return self;
}

- free {
  [self clear];
  [super free];
  return self;
}

- (char *) Label {
  return Label;
}

- (char *) Opcode {
  return Opcode;
}

- (char *) Args {
  return Args;
}

- (char *) Comment {
  return Comment;
}

@end
