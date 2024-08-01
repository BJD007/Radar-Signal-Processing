#include "range_processing.hpp"
#include <cmath>
#include <algorithm>
#include <fftw3.h>

vector<double> range_processing(const vector<complex<double>>& rx_signal, int num_samples) {
    // Apply Hann window
    vector<complex<double>> windowed_signal(num_samples);
    for (int i = 0; i < num_samples; ++i) {
        double hann = 0.5 * (1 - cos(2 * M_PI * i / (num_samples - 1)));
        windowed_signal[i] = hann * rx_signal[i];
    }
    
    // Prepare for FFT
    fftw_complex *in, *out;
    fftw_plan p;
    
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * num_samples);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * num_samples);

    // Copy windowed signal to FFTW input
    for (int i = 0; i < num_samples; ++i) {
        in[i][0] = windowed_signal[i].real();
        in[i][1] = windowed_signal[i].imag();
    }

    // Create FFT plan and execute
    p = fftw_plan_dft_1d(num_samples, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    // Compute magnitude of FFT output
    vector<double> range_profile(num_samples);
    for (int i = 0; i < num_samples; ++i) {
        double real = out[i][0];
        double imag = out[i][1];
        range_profile[i] = sqrt(real*real + imag*imag);
    }

    // Normalize
    double max_val = *max_element(range_profile.begin(), range_profile.end());
    for (int i = 0; i < num_samples; ++i) {
        range_profile[i] /= max_val;
    }

    // Clean up FFTW
    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);

    return range_profile;
}
