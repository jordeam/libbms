#include <guile/gh.h>
#include "include/ondas.h"

SCM scm_trap_ab(SCM scm_theta, SCM scm_a, SCM scm_b) {
  double theta = gh_scm2double(scm_theta);
  double a = gh_scm2double(scm_a);
  double b = gh_scm2double(scm_b);

  return gh_double2scm(trap_ab(theta, a, b));
}

SCM scm_trap(SCM scm_theta) {
  double theta = gh_scm2double(scm_theta);
  return gh_double2scm(trap(theta));
}

SCM scm_trap60(SCM scm_theta) {
  double theta = gh_scm2double(scm_theta);
  return gh_double2scm(trap60(theta));
}

SCM scm_trap120(SCM scm_theta) {
  double theta = gh_scm2double(scm_theta);
  return gh_double2scm(trap120(theta));
}

SCM scm_trap90(SCM scm_theta) {
  double theta = gh_scm2double(scm_theta);
  return gh_double2scm(trap90(theta));
}

void ondas_init(void * unused) {
  gh_new_procedure("trap-ab", scm_trap_ab, 3, 0, 0);
  gh_new_procedure("trap", scm_trap, 1, 0, 0);
  gh_new_procedure("trap60", scm_trap60, 1, 0, 0);
  gh_new_procedure("trap90", scm_trap90, 1, 0, 0);
  gh_new_procedure("trap120", scm_trap120, 1, 0, 0);
}
