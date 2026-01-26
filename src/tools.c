#include <math.h>
#include <stdlib.h>
#include <complex.h>
#include <pthread.h>

#include "tools.h"
#include "extra.h"

void create_particles(struct particle *par, unsigned int num) {
	for(unsigned int i = 0; i < num; i++)
		for(int j = 0; j < 3; j++) {
			par[i].betaAccel[j] = 0.0;
			par[i].beta[j] = 0.0;
			par[i].x[j] = 0.0;
		}
}

void ak(double *ak, struct particle *par, double *n) {
	double t1[3], t2[3], t3[3];
	vec_sub(t1, n, par->beta);
	cross(t2, t1, par->betaAccel);
	cross(t3, n, t2);
	vec_mult(t3, 1.0 / pow(1.0 - dot(n, par->beta), 2.0));
	copy_vec(ak, t3);
}

void epsilon(double complex *epsilon_temp, struct particle *par, double omega, double *n, int i, double dt) {
	double ak_temp[3];
	ak(ak_temp, par, n);
	double arg = omega * ((double)i * dt - dot(n, par->x) / c);
	complex_vec_euler(epsilon_temp, ak_temp, arg);
}

double compute_omega(double start_freq, double end_freq, int i) {
	double omega = 2.0 * pi * (start_freq + i * (end_freq - start_freq) / FREQ_NUM);
	return omega;
}

double compute_dt(int num) {
	double dt = 1.0 / (2000.0 * num);
	return dt;
}

double compute_frequency(int i) {
	double freq = 100.0 + 100.0 * i;
	return freq;
}

void create_shared_data(pthread_barrier_t *barrier_compute, pthread_barrier_t *barrier_sync, struct shared_data *shared_data, double *output_data, double complex *epsilon_temp, double *freq, struct particle *par, int num, FILE *out) {
	for(int i = 0; i < num; i++) {
		freq[i] = compute_frequency(i);
	}
	for(int i = 0; i < CORE_NUM; i++) {
		shared_data[i].id = i;
		shared_data[i].num = num;
		shared_data[i].out = out;
		shared_data[i].par = par;
		shared_data[i].freq = freq;
		shared_data[i].output_data = output_data;
		shared_data[i].epsilon_temp = epsilon_temp;
		shared_data[i].barrier_compute = barrier_compute;
		shared_data[i].barrier_sync = barrier_sync;
	}
}

void create_parameters(struct shared_data *shared_data, double start_freq, double end_freq, double dt, double *n) {
	for(int i = 0; i < CORE_NUM; i++) {
		shared_data[i].n = n;
		shared_data[i].dt = dt;
		shared_data[i].start_freq = start_freq;
		shared_data[i].end_freq = end_freq;
	}
}