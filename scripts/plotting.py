import os
import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent
PROJECT_ROOT = SCRIPT_DIR.parent
BIN_DIR = PROJECT_ROOT / "bin"
OUTPUT_DIR = PROJECT_ROOT / "output"
OUTPUT_IMAGE_DIR = PROJECT_ROOT / "output-image"

def plot_graph(i):
    output_filename = f"{OUTPUT_DIR}/out.txt"
    output_image = f"{OUTPUT_IMAGE_DIR}/out-{i}.png"
    
    data = np.fromfile(output_filename, dtype=np.float64).reshape(-1, 2)
    x = data[:, 0]
    y = data[:, 1]
    
    plt.figure(figsize=(10, 10))
    plt.xlabel(r"$\nu$ [a.u.]")
    plt.ylabel(r"$I$ [a.u.]")
    plt.plot(x, y, '-', linewidth=0.5)
    plt.savefig(output_image, dpi=150, bbox_inches='tight')
    plt.close()
    
    print(f"Created scatter plot of radiation spectra for N = {i}.")