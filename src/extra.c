#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>

#include "extra.h"

const double m = 1.0;
const double q = -1.0;
const double c = 137.036;
const double e0 = 0.07957747154;
const double pi = 3.14159265359;

unsigned int num;

double rand_num(double min, double max) {
	double s = rand() / (double)RAND_MAX;
	return min + s * (max - min);
}

void complex_vec_zero(double complex *uc) {
	for(int i = 0; i < 3; i++)
		uc[i] = 0.0 + 0.0 * I;
}

void copy_complex(double complex *uc, double complex *ua) {
	for(int i = 0; i < 3; i++)
		uc[i] = ua[i];
}

void copy_vec(double *a, double *b) {
	for(int i = 0; i < 3; i++)
		a[i] = b[i];
}

void complex_vec_add(double complex *ua, double complex *ub, double complex *uc) {
	for(int i = 0; i < 3; i++)
		ua[i] = ub[i] + uc[i];
}

void complex_vec_euler(double complex *uc, double *u, double a) {
	for(int i = 0; i < 3; i++)
		uc[i] = u[i] * cos(a) + u[i] * sin(a) * I;
}

double complex_abs_squared(double complex *uc) {
	double x = 0.0;
	for(int i = 0; i < 3; i++)
		x += cabs(uc[i]) * cabs(uc[i]);
	return x;
}

double dot(double *a, double *b) {
	double x = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	return x;
}

void cross(double *u, double *a, double *b) {
	u[0] = a[1] * b[2] - a[2] * b[1];
	u[1] = a[2] * b[0] - a[0] * b[2];
	u[2] = a[0] * b[1] - a[1] * b[0];
}

double magnitude(double *a) {
	double x = sqrt(dot(a, a));
	return x;
}

void vec_mult(double *a, double b) {
	for(int i = 0; i < 3; i++)
		a[i] *= b;
}

void vec_sub(double *u, double *a, double *b) {
	for(int i = 0; i < 3; i++)
		u[i] = a[i] - b[i];
}

void rotate(double *u, double phi, double theta) {
	double x, y, z;
	y = u[1];
	z = u[2];
	u[1] = y * cos(phi) - z * sin(phi);
	u[2] = y * sin(phi) + z * cos(phi);
	x = u[0];
	y = u[1];
	u[0] = x * cos(theta) - y * sin(theta);
	u[1] = x * sin(theta) + y * cos(theta);
}

int start_index(int n, unsigned int thread_num) {
	int index = n * thread_num / CORE_NUM;
	return index;
}

int final_index(int n, unsigned int thread_num) {
	int index = n * (thread_num + 1) / CORE_NUM;
	return index;
}

void check_arguments(int argc, char **argv) {
	if(argc < 3) {
		printf("This is a program that simulates the emitted radiation spectra of accelerated electrons.\n");
		printf("Usage: %s <num_particles>", argv[0]);
		abort();
	}
}