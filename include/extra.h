#ifndef EXTRA_H
#define EXTRA_H

#include <pthread.h>
#include <complex.h>

#define CORE_NUM 4
#define FREQ_NUM 1024
#define STEP_NUM 2048

#define DEG_TO_RAD (pi / 180.0)

extern const double q, e0, pi, m, c;
extern unsigned int num;

double rand_num(double min, double max);
void complex_vec_zero(double complex *uc);
void copy_complex(double complex *uc, double complex *ua);
void copy_vec(double *a, double *b);
void complex_vec_add(double complex *ua, double complex *ub, double complex *uc);
void complex_vec_euler(double complex *uc, double *u, double a);
double complex_abs_squared(double complex *uc);
double dot(double *a, double *b);
void cross(double *u, double *a, double *b);
double magnitude(double *a);
void vec_mult(double *a, double b);
void vec_sub(double *u, double *a, double *b);
void rotate(double *u, double phi, double theta);
int start_index(int n, unsigned int thread_num);
int final_index(int n, unsigned int thread_num);
void check_arguments(int argc, char **argv);

#endif