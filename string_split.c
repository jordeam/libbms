#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>

#include "strbms.h"

/*
  Split string str, according to a set of chars given in sep.  Split
  n_sep separators only.  Return a GList of dinamically allocated
  strings, which must be freed by string_split_free.
*/
GList * string_split_n(const char * str, const char * sep, int n_sep) {
  char * sc, * sn, * si;
  GList * slist = NULL;
  int i = 0;

  if (str == NULL)
    return NULL;

  /* make a copy */
  sn = sc = strdup(str);

  while ((n_sep < 0 || i < n_sep) && sn) {
    si = strsep(& sn, sep);
    if (si && strlen(si) != 0) {
/*       printf("si = %s\n", si); */
      slist = g_list_append(slist, strdup(si));
      i++;
    }
  }

  if (n_sep > 0 && sn && strlen(strtrim(sn))) {
    slist = g_list_append(slist, strdup(sn));
  }
  
  /* free copy string */
  free(sc);
  return slist;
}

/*
  Split string str, according to a set of chars given in sep.  Split
  all separators.  Return a GList of dinamically allocated strings,
  which must be freed by string_split_free.
*/
GList * string_split(const char * str, const char * sep) {
  return string_split_n(str, sep, -1);
}

static void string_split_free1(gpointer data, gpointer user_data) {
  if (data)
    free(data);
}

void string_split_free(GList * slist) {
  if (slist) {
    g_list_foreach(slist, (GFunc) string_split_free1, NULL);
    g_list_free(slist);
  }
}

#ifdef TEST

static void print_element(char * str, FILE * stream) {
  fprintf((FILE *) stream, "->[%s]<-\n", str);
}

int main(int argc, char * argv[]) {
  char * s = " \r \t this is a \r \n string \r \n \t test!!, hello world! \r\n";

  GList * slist = string_split_n(s, " \r\n\t", 0);
  printf("Initial string beginning in the next line:\n%s\n", s);

  printf("--> splitted to:\n");
  g_list_foreach(slist, (GFunc) print_element, stdout);
  string_split_free(slist);

  return 0;
}

#endif
