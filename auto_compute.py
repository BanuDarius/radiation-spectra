import matplotlib.pyplot as plt

import scripts.plotting as plots
import scripts.programs as programs

if __name__ == "__main__":
    for i in range(1, 10):
        programs.run_program(i)
        plots.plot_graph(i)
        
    print("Finished program execution.\a")