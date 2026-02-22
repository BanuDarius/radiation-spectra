## Description

This is a C and OpenMP implementation of the frequency-domain algorithm described by Kan Yi-Kai et al. (2022), whose main function is calculating the radiation spectra emitted by accelerated particles such as electrons.

## Usage
```
git clone https://github.com/BanuDarius/radiation-spectra
cd radiation-spectra/
make
python3 auto_compute.py
```
This will automatically create a quick parameter sweep, starting with one and adding of a few more electrons oscillating in a sinusoidal pattern, with increasing frequency.

## Example output
<img width="1271" height="1273" alt="out-15" src="https://github.com/user-attachments/assets/4c77e78d-f33e-4af6-b2ed-38d6c332741e" />

## References
[1] Kan, Yi-Kai, et al. "Parallel computation of Inverse Compton Scattering radiation spectra based on Liénard-Wiechert potentials." Journal of computational physics 473 (2023): 111724.
