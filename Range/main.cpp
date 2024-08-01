#include "range_processing.hpp"
#include <iostream>
#include <vector>
#include <complex>

int main() {
    int num_samples = 1024;
    std::vector<std::complex<double>> rx_signal(num_samples);

    // Generate a sample received signal (replace with actual radar data)
    for (int i = 0; i < num_samples; ++i) {
        double t = static_cast<double>(i) / num_samples;
        rx_signal[i] = std::exp(std::complex<double>(0, 2 * M_PI * 10 * t)); // Example: 10 Hz complex sinusoid
    }

    // Process the range data
    std::vector<double> range_profile = range_processing(rx_signal, num_samples);

    // Print the first few values of the range profile
    std::cout << "Range Profile (first 10 values):" << std::endl;
    for (int i = 0; i < 10; ++i) {
        std::cout << range_profile[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
