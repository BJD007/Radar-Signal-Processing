# Doppler

- FFTW Library: Uses the FFTW library for efficient FFT computation.
- Class-based Design: Encapsulates the Doppler processing in a DopplerProcessor class, allowing for better resource management and reusability.
- Window Function: Applies a Hamming window to reduce spectral leakage.
- Complex Input: Handles complex input data, which is typical in radar signal processing.
- Magnitude Computation: Computes the magnitude of the complex FFT output.

To compile and run this code, you'll need to link against the FFTW library. For example:

g++ -std=c++11 main.cpp doppler_processing.cpp -o doppler_program -lfftw3 -lm

Created on 2018-10-03