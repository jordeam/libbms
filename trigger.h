#ifndef _trigger_h
#define _trigger_h

#ifdef __cplusplus
extern "C" {
#endif

/* Trigger */
struct trigger {
  /* Crossing time */
  double tx;
  /* time after crossing to consider a */
  double dt;
  /* the crossing value */
  double x;
  /* variable to observe */
  double *pdata;
  /* if a trigger occurred, must be cleared after take the value */
  int trig;
  /* inside a trigger event -> waiting for t - t1 > dt */
  int in_trig;
  /* was already trigger, it will wait until x<level */
  int hold;
  /* type 0->falling edge, 1->rising edge */
  int type;
  /* counter */
  int cnt;
};

typedef struct trigger trigger_t;

trigger_t * trigger_new(double value, double dt, int type, double * pdata);
void trigger_clear(trigger_t * trig);
int trigger_eval(trigger_t * trig, double value, double t);
void trigger_reset(trigger_t *trig);
int trigger_is_set(trigger_t *trig);

#ifdef __cplusplus
}
#endif

#endif
