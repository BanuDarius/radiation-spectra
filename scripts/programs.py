import os
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent
PROJECT_ROOT = SCRIPT_DIR.parent
BIN_DIR = PROJECT_ROOT / "bin"
OUTPUT_DIR = PROJECT_ROOT / "output"
OUTPUT_IMAGE_DIR = PROJECT_ROOT / "output-image"

def run_program(i):
    program_name = f"{BIN_DIR}/radiation"
    output_filename = f"{OUTPUT_DIR}/out.txt"
    
    os.system(f"{program_name} {i} {output_filename}")