#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <complex.h>
#include <pthread.h>

struct particle {
	double betaAccel[3], beta[3], x[3];
};

struct shared_data {
	FILE *out;
	int num, id;
	struct particle *par;
	double start_freq, end_freq, dt;
	double complex *epsilon_temp;
	double *output_data, *comp_intensity, *comp_freq, *freq, *n;
	pthread_barrier_t *barrier_sync, *barrier_compute;
};

void create_particles(struct particle *par, unsigned int num);
void ak(double *ak, struct particle *par, double *n);
void epsilon(double complex *epsilon_temp, struct particle *par, double omega, double *n, int i, double dt);
double compute_omega(double start_freq, double end_freq, int i);
double compute_dt(int num);
double compute_frequency(int i);
void create_shared_data(pthread_barrier_t *barrier_compute, pthread_barrier_t *barrier_sync, struct shared_data *shared_data, double *output_data, double complex *epsilon_temp, double *freq, struct particle *par, int num, FILE *out);
void create_parameters(struct shared_data *shared_data, double start_freq, double end_freq, double dt, double *n);

#endif