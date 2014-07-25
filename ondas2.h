#ifndef _ONDAS2_H_
#define _ONDAS2_H_


struct dflux1 {
  double B1, theta0, theta1;
  };

typedef struct dflux1 DFLUX1;

void dflux1_init(DFLUX1 *self, double B1, double theta0, double theta1);
double dflux1_eval(DFLUX1 *self, double theta);

struct dflux2 {
  double theta0, theta1;
};

typedef struct dflux2 DFLUX2;

void dflux2_init(DFLUX2 *self, double theta0, double theta1);
double dflux2_eval(DFLUX2 *self, double theta);

#endif /* _ONDAS2_H_ */
