#include "range_processing.hpp"
#include <cmath>
#include <algorithm>

vector<double> range_processing(const vector<complex<double>>& rx_signal, int num_samples) {
    vector<double> windowed_signal(rx_signal.size());
    for (size_t i = 0; i < rx_signal.size(); ++i) {
        double hann = 0.5 * (1 - cos(2 * M_PI * i / (rx_signal.size() - 1)));
        windowed_signal[i] = hann * abs(rx_signal[i]);
    }
    
    // Placeholder for FFT
    return windowed_signal;
}
