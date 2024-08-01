#include "doppler_processing.hpp"
#include <iostream>

int main() {
    int num_range_bins = 256;
    int num_pulses = 128;

    // Create sample range data (replace with actual radar data)
    std::vector<std::vector<std::complex<double>>> range_data(num_pulses, std::vector<std::complex<double>>(num_range_bins));
    for (int i = 0; i < num_pulses; ++i) {
        for (int j = 0; j < num_range_bins; ++j) {
            range_data[i][j] = std::complex<double>(rand() / (double)RAND_MAX, rand() / (double)RAND_MAX);
        }
    }

    // Create and use the Doppler processor
    DopplerProcessor processor(num_range_bins, num_pulses);
    std::vector<std::vector<double>> doppler_data = processor.process(range_data);

    // Print a sample of the result
    std::cout << "Doppler data for first range bin:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        std::cout << doppler_data[0][i] << " ";
    }
    std::cout << "..." << std::endl;

    return 0;
}
