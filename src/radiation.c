#include <omp.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

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

void simulate(FILE *out, struct particle *par, double complex *epsilon_temp, double *output_data, double *freq, double *n, double start_freq, double end_freq, double dt, int num, int core_num) {
	#pragma omp parallel
		{
		int id = omp_get_thread_num();
		int start_idx = start_index(num, id, core_num);
		int end_idx = final_index(num, id, core_num);
		
		double complex epsilon_c[3], epsilon_final[3];
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
			#pragma omp barrier
			
			#pragma omp master
			{
				double complex res[3];
				complex_vec_zero(res);
				for(int j = 0; j < core_num; j++)
					complex_vec_add(res, res, &epsilon_temp[3 * j]);
				output_data[2 * i] = omega / (2.0 * pi);
				output_data[2 * i + 1] = complex_abs_squared(res) * (q * q * dt * dt) / (16.0 * pi * pi * pi * e0 * c);
			}
			#pragma omp barrier
		}
	}
	fwrite(output_data, sizeof(double), 2 * FREQ_NUM, out);
}

int main(int argc, char **argv) {
	srand(time(NULL));
	clock_t ti = clock();
	FILE *out = fopen(argv[3], "wb");
	
	check_arguments(argc, argv);
	int num = atoi(argv[1]), core_num = atoi(argv[2]);
	double freq[num], n[3] = {0.0, 1.0, 0.0};
	double start_freq = 50.0, end_freq = 2000.0, dt = compute_dt(num);
	omp_set_num_threads(core_num);
	set_frequency(freq, num);
	
	double *output_data = malloc(2 * FREQ_NUM * sizeof(double));
	struct particle *par = malloc(num * sizeof(struct particle));
	double complex *epsilon_temp = malloc(3 * core_num * sizeof(double complex));
	
	if(!output_data || !par || !epsilon_temp) { perror("Not enough memory."); abort(); }
	
	rotate(n, 60.0 * DEG_TO_RAD, 0.0);
	create_particles(par, num);
	
	printf("Simulation started.\n");
	simulate(out, par, epsilon_temp, output_data, freq, n, start_freq, end_freq, dt, num, core_num);	
	printf("Simulation ended.\n");
	
	printf("Time taken: %0.3fs.\n", (double)(clock() - ti) / (CLOCKS_PER_SEC * core_num));
	free(output_data); free(epsilon_temp);
	fclose(out);
	return 0;
}