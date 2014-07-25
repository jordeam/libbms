/*
 * Definicao do tipo complex
 */

typedef struct {
  double r, i;
} COMPLEX;

COMPLEX complex(double real_part, double imag_part);
COMPLEX c_add(COMPLEX z1, COMPLEX z2);
COMPLEX c_sub(COMPLEX z1, COMPLEX z2);
COMPLEX c_mul(COMPLEX z1, COMPLEX z2);
COMPLEX c_div(COMPLEX z1, COMPLEX z2);
COMPLEX c_exp(COMPLEX z1);
double c_abs(COMPLEX z);
