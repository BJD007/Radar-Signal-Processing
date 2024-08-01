// doppler_processing.cpp
#include "doppler_processing.hpp"
#include <cmath>
#include <algorithm>

DopplerProcessor::DopplerProcessor(int num_range_bins, int num_pulses)
    : num_range_bins(num_range_bins), num_pulses(num_pulses) {
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * num_pulses);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * num_pulses);
    plan = fftw_plan_dft_1d(num_pulses, in, out, FFTW_FORWARD, FFTW_MEASURE);
}

DopplerProcessor::~DopplerProcessor() {
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);
}

std::vector<std::vector<double>> DopplerProcessor::process(const std::vector<std::vector<std::complex<double>>>& range_data) {
    std::vector<std::vector<double>> doppler_data(num_range_bins, std::vector<double>(num_pulses));

    for (int range_bin = 0; range_bin < num_range_bins; ++range_bin) {
        std::vector<std::complex<double>> pulse_data(num_pulses);
        for (int pulse = 0; pulse < num_pulses; ++pulse) {
            pulse_data[pulse] = range_data[pulse][range_bin];
        }

        applyWindowFunction(pulse_data);

        for (int i = 0; i < num_pulses; ++i) {
            in[i][0] = pulse_data[i].real();
            in[i][1] = pulse_data[i].imag();
        }

        fftw_execute(plan);

        doppler_data[range_bin] = computeMagnitude(out);
    }

    return doppler_data;
}

void DopplerProcessor::applyWindowFunction(std::vector<std::complex<double>>& data) {
    // Hamming window
    for (int i = 0; i < num_pulses; ++i) {
        double window = 0.54 - 0.46 * std::cos(2 * M_PI * i / (num_pulses - 1));
        data[i] *= window;
    }
}

std::vector<double> DopplerProcessor::computeMagnitude(fftw_complex* fft_result) {
    std::vector<double> magnitude(num_pulses);
    for (int i = 0; i < num_pulses; ++i) {
        double real = fft_result[i][0];
        double imag = fft_result[i][1];
        magnitude[i] = std::sqrt(real * real + imag * imag);
    }
    return magnitude;
}
