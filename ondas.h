#ifndef _ondas_h
#define _ondas_h

#ifdef __cplusplus
extern "C" {
#endif

double quad120(double);
double quad180(double);
double square_pulse(double theta, double theta_on, double theta_off);
double trap(double);
double trapezoidal(double);
double dis3(double IM, double theta);
double mvpwm(double, double);
double fluxotrap(double);
double trap60(double);
double trap90(double);
double trap120(double);
double flux_open(double);
double semitrap(double);
double trap120d(double theta, double d);
double trap_ab(double theta, double a, double b);
double flux_rad(double Bf, double theta1, double theta2, double theta);
double pwmdiss1(double M, double ang);
double pwmdiss2(double M, double ang);
double pwmdiss3(double M, double ang);
double pwmdiss4(double M, double ang);
double mcos_w_sat(double M, double ang);

#ifdef __cplusplus
}
#endif

#endif
