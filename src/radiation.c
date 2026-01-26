#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <pthread.h>

#include "extra.h"
#include "tools.h"

void advance(struct particle *par, double freq, int i, double dt) {
	double amp = 1e-5;
	double accel = 0.0;
	double t = (double)i * dt;
	double omega = 2.0 * pi * freq;
	
	par->x[2] = amp * sin(omega * t);
	par->beta[2] = amp * omega * cos(omega * t) / c;
	par->betaAccel[2] = -amp * omega * omega * sin(omega * t) / c;
	
	par->x[0] = 0.5 * accel * t * t;
	par->beta[0] = accel * t / c;
	par->betaAccel[0] = accel / c;
	
	if(magnitude(par->beta) > 1.0) {
		printf("OVER LIGHTSPEED!!! %f\n", magnitude(par->beta));
		abort();
	}
}

void *simulate(void *data) {
	struct shared_data *shared_data = (struct shared_data*)data;
	double complex epsilon_c[3], epsilon_final[3];
	
	int id = shared_data->id;
	double *n = shared_data->n;
	double dt = shared_data->dt;
	FILE *out = shared_data->out;
	double *freq = shared_data->freq;
	struct particle *par = shared_data->par;
	double *output_data = shared_data->output_data;
	double complex *epsilon_temp = shared_data->epsilon_temp;
	int start_idx = start_index(num, id), end_idx = final_index(num, id);
	double start_freq = shared_data->start_freq, end_freq = shared_data->end_freq;
	
	pthread_barrier_t *barrier_sync = shared_data->barrier_sync;
	pthread_barrier_t *barrier_compute = shared_data->barrier_compute;
	
	pthread_barrier_wait(barrier_compute);
	for(int i = 0; i < FREQ_NUM; i++) {
		complex_vec_zero(epsilon_final);
		double omega = compute_omega(start_freq, end_freq, i);
		
		for(int step = 0; step < STEP_NUM; step++) {
			for(int p = start_idx; p < end_idx; p++) {
				advance(&par[p], freq[p], step, dt);
				epsilon(epsilon_c, &par[p], omega, n, step, dt);
				complex_vec_add(epsilon_final, epsilon_final, epsilon_c);
			}
		}
		
		copy_complex(&epsilon_temp[3 * id], epsilon_final);
		pthread_barrier_wait(barrier_compute);
		if(id == 0) {
			double complex res[3];
			complex_vec_zero(res);
			for(int j = 0; j < CORE_NUM; j++)
				complex_vec_add(res, res, &epsilon_temp[3 * j]);
			output_data[2 * i] = omega / (2 * pi);
			output_data[2 * i + 1] = complex_abs_squared(res) * (q * q * dt * dt) / (16.0 * pi * pi * pi * e0 * c);
		}
		pthread_barrier_wait(barrier_sync);
	}
	if(id == 0) {
		fwrite(output_data, sizeof(double), 2 * FREQ_NUM, out);
	}
	return NULL;
}

int main(int argc, char **argv) {
	srand(time(NULL));
	clock_t ti = clock();
	pthread_t thread[CORE_NUM];
	FILE *out = fopen(argv[2], "wb");
	pthread_barrier_t barrier_sync, barrier_compute;
	pthread_barrier_init(&barrier_sync, NULL, CORE_NUM);
	pthread_barrier_init(&barrier_compute, NULL, CORE_NUM);
	
	check_arguments(argc, argv);
	num = atoi(argv[1]);
	double freq[num], n[3] = {0.0, 1.0, 0.0};
	
	double *output_data = malloc(2 * FREQ_NUM * sizeof(double));
	struct particle *par = malloc(num * sizeof(struct particle));
	struct shared_data *shared_data = malloc(CORE_NUM * sizeof(struct shared_data));
	double complex *epsilon_temp = malloc(3 * CORE_NUM * sizeof(double complex));
	
	if(!output_data || !par || !epsilon_temp) { perror("Not enough memory."); abort(); }
	
	rotate(n, 60.0 * DEG_TO_RAD, 0.0);
	create_particles(par, num);
	create_parameters(shared_data, 50, 2000, compute_dt(num), n);
	create_shared_data(&barrier_sync, &barrier_compute, shared_data, output_data, epsilon_temp, freq, par, num, out);
	
	printf("Simulation started.\n");
	for(int i = 0; i < CORE_NUM; i++)
		pthread_create(&thread[i], NULL, simulate, (void*)&shared_data[i]);
	for(int i = 0; i < CORE_NUM; i++)
		pthread_join(thread[i], NULL);
	
	printf("Simulation ended.\n");
	printf("Time taken: %0.3fs.\n", (double)(clock() - ti) / (CLOCKS_PER_SEC * CORE_NUM));
	pthread_barrier_destroy(&barrier_sync);
	pthread_barrier_destroy(&barrier_compute);
	free(output_data); free(shared_data); free(epsilon_temp);
	fclose(out);
	return 0;
}