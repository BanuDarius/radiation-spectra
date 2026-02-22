import matplotlib.pyplot as plt

import scripts.plotting as plots
import scripts.programs as programs

core_num = 4
max_particles = 16

if __name__ == "__main__":
    for i in range(1, max_particles):
        programs.run_program(i, core_num)
        plots.plot_graph(i)
        print(f"Ended parameter sweep step {i}/{max_particles}.")
        
    print("Finished program execution.\a")