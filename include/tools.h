#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <complex.h>

struct particle {
	double betaAccel[3], beta[3], x[3];
};

void create_particles(struct particle *par, unsigned int num);
void ak(double *ak, struct particle *par, double *n);
void epsilon(double complex *epsilon_temp, struct particle *par, double omega, double *n, int i, double dt);
double compute_omega(double start_freq, double end_freq, int i);
double compute_dt(int num);
void set_frequency(double *freq, int i);

#endif