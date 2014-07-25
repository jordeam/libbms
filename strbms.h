#ifndef _strbms_h
#define _strbms_h

#include <stdio.h>
#include <glib.h>

#ifndef STRSIZE
#define STRSIZE 8192
#endif

GList * string_split(const char * str, const char * sep);
void string_split_free(GList * slist);
GList * string_split_n(const char * str, const char * sep, int n_sep);

char * chomp(char * s);
size_t strrspn(const char * s, const char * skipset);
char * strtrim(char * s);
char * strsubst(char * s, char * pat, char * subs);
char * strnsubst(char * s, char * pat, char * subs, int n);
char * newgetline(FILE * stream);
char * strcatdup(char ** ps, char * sa);
/* char * strcasestr(char * str, char * substr); */

#endif
