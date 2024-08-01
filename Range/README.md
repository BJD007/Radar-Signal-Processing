# Range
- FFTW Usage: We use the FFTW library to perform the Fast Fourier Transform efficiently.
- Complex Input Handling: The function now properly handles complex input signals.
- Hann Window: The Hann window is applied to the complex input signal.
- FFT Computation: The FFT is computed using FFTW, which is highly optimized for performance.
- Magnitude Calculation: We calculate the magnitude of the complex FFT output.
- Normalization: The range profile is normalized by dividing by its maximum value.

## To use this function, you'll need to:
- Install the FFTW library on your system.
- Link against the FFTW library when compiling your code.

## How you might compile this code:
g++ -std=c++11 your_main_file.cpp range_processing.cpp -o range_processing_program -lfftw3 -lm

Created on 2018-09-11