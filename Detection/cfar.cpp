#include "cfar.hpp"
#include <cmath>
#include <algorithm>

vector<bool> ca_cfar(const vector<double>& signal, int num_guard_cells, int num_training_cells, double pfa) {
    vector<bool> detections(signal.size(), false);
    for (size_t i = 0; i < signal.size(); ++i) {
        int start = max(0, static_cast<int>(i) - num_training_cells - num_guard_cells);
        int end = min(static_cast<int>(signal.size()), static_cast<int>(i) + num_training_cells + num_guard_cells);
        
        vector<double> training_cells;
        for (int j = start; j < static_cast<int>(i) - num_guard_cells; ++j) {
            training_cells.push_back(signal[j]);
        }
        for (int j = static_cast<int>(i) + num_guard_cells; j < end; ++j) {
            training_cells.push_back(signal[j]);
        }
        
        double noise_level = 0;
        for (double cell : training_cells) {
            noise_level += cell;
        }
        noise_level /= training_cells.size();
        
        double threshold = noise_level * (-log(pfa) * (2 * num_training_cells));
        detections[i] = signal[i] > threshold;
    }
    return detections;
}
