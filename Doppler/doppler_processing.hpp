// doppler_processing.hpp
#ifndef DOPPLER_PROCESSING_HPP
#define DOPPLER_PROCESSING_HPP

#include <vector>
#include <complex>
#include <fftw3.h>

class DopplerProcessor {
public:
    DopplerProcessor(int num_range_bins, int num_pulses);
    ~DopplerProcessor();

    std::vector<std::vector<double>> process(const std::vector<std::vector<std::complex<double>>>& range_data);

private:
    int num_range_bins;
    int num_pulses;
    fftw_complex *in, *out;
    fftw_plan plan;

    void applyWindowFunction(std::vector<std::complex<double>>& data);
    std::vector<double> computeMagnitude(fftw_complex* fft_result);
};

#endif // DOPPLER_PROCESSING_HPP
