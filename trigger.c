#include <stdlib.h>

#include "trigger.h"


void
trigger_clear(trigger_t * trig)
{
  trig->tx = 0;
  trig->trig = 0;
  trig->in_trig=0;
  trig->hold = 0;
  trig->cnt = 0;
}

trigger_t * trigger_new(double value, double dt, int type, double * pdata) {
  trigger_t * trig = malloc(sizeof(trigger_t));
  trig->dt = dt;
  trig->x = value;
  trig->type = type;
  trig->pdata = pdata;
  trigger_clear(trig);
  return trig;
}

/* analise trigger and return TRUE (1) if a trigger occurred */
int trigger_eval(trigger_t * trig, double value, double t) {
  if ((trig->type == 1 && value >= trig->x) || (trig->type == 0 && value <= trig->x)){
    if (trig->trig || trig->hold)
      /* if the trigger was already set, do nothing */
      return 0;
    if (trig->in_trig == 0) {
      /* setting this time as the crossing time */
      trig->tx = t;
      /* show we are in a trigger process */
      trig->in_trig = 1;
      if (t - trig->tx >= trig->dt) {
        /* this is a valid trigger */
        trig->trig = 1;
        trig->hold = 1;
        trig->in_trig = 0;
        trig->cnt++;
        return 1;
      }
    }
    else {
      /* it is a trigger process, it must wait for t - tx >= dt */
      if (t - trig->tx >= trig->dt) {
        /* this is a valid trigger */
        trig->trig = 1;
        trig->hold = 1;
        trig->in_trig = 0;
        trig->cnt++;
        return 1;
      }
    }
  }
  else {
    /* if in_trig was set, it must be cleared */
    /* printf("in_trig=0\n"); */
    trig->in_trig = 0;
    trig->hold = 0;
  }
  return 0;
}

int
trigger_is_set(trigger_t *trig) {
  return trig->trig;
}

void trigger_reset(trigger_t *trig) {
  trig->trig = 0;
}

