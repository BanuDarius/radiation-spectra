import matplotlib.pyplot as plt

import scripts.plotting as plots
import scripts.programs as programs

core_num = 4

if __name__ == "__main__":
    for i in range(1, 16):
        programs.run_program(i, core_num)
        plots.plot_graph(i)
        
    print("Finished program execution.\a")