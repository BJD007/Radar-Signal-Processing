#include <iostream>
#include <Eigen/Dense>
#include <vector>

using namespace std;
using namespace Eigen;

int main() {
    // Example covariance matrix (replace with actual data)
    MatrixXcd covariance_matrix(4, 4);
    covariance_matrix << 1, 0.5, 0.2, 0.1,
                         0.5, 1, 0.3, 0.2,
                         0.2, 0.3, 1, 0.4,
                         0.1, 0.2, 0.4, 1;

    // Number of sources to estimate
    int num_sources = 2;

    // Angles to evaluate (in degrees)
    vector<double> angles = {-90, -60, -30, 0, 30, 60, 90};

    // Perform MUSIC DOA estimation
    vector<double> doa_estimates = music_doa(covariance_matrix, num_sources, angles);

    // Print DOA estimates
    for (double doa : doa_estimates) {
        cout << "Estimated DOA: " << doa << " degrees" << endl;
    }

    return 0;
}
