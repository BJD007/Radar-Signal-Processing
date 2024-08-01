#include "doa.hpp"
#include <cmath>
#include <algorithm>
#include <Eigen/Dense>
#include <vector>

using namespace std;
using namespace Eigen;

vector<double> music_doa(const MatrixXcd& covariance_matrix, int num_sources, const vector<double>& angles) {
    // Step 1: Eigen decomposition of the covariance matrix
    SelfAdjointEigenSolver<MatrixXcd> eigen_solver(covariance_matrix);
    MatrixXcd eigen_vectors = eigen_solver.eigenvectors();
    VectorXd eigen_values = eigen_solver.eigenvalues();

    // Step 2: Sort eigenvalues and eigenvectors
    vector<pair<double, VectorXcd>> eigen_pairs;
    for (int i = 0; i < eigen_values.size(); ++i) {
        eigen_pairs.push_back(make_pair(eigen_values(i), eigen_vectors.col(i)));
    }
    sort(eigen_pairs.begin(), eigen_pairs.end(), [](const pair<double, VectorXcd>& a, const pair<double, VectorXcd>& b) {
        return a.first > b.first;  // Sort in descending order
    });

    // Step 3: Extract noise subspace
    MatrixXcd noise_subspace(covariance_matrix.rows(), covariance_matrix.cols() - num_sources);
    for (int i = num_sources; i < eigen_pairs.size(); ++i) {
        noise_subspace.col(i - num_sources) = eigen_pairs[i].second;
    }

    // Step 4: Compute MUSIC pseudo-spectrum
    vector<double> pseudo_spectrum(angles.size(), 0.0);
    for (size_t i = 0; i < angles.size(); ++i) {
        double angle = angles[i];
        VectorXcd steering_vector(covariance_matrix.rows());
        for (int j = 0; j < steering_vector.size(); ++j) {
            steering_vector(j) = exp(complex<double>(0, -2 * M_PI * j * sin(angle * M_PI / 180.0)));
        }
        auto projection = noise_subspace.adjoint() * steering_vector;
        pseudo_spectrum[i] = 1.0 / projection.squaredNorm();
    }

    // Step 5: Find peaks in the pseudo-spectrum
    vector<double> doa_estimates;
    for (int i = 1; i < pseudo_spectrum.size() - 1; ++i) {
        if (pseudo_spectrum[i] > pseudo_spectrum[i - 1] && pseudo_spectrum[i] > pseudo_spectrum[i + 1]) {
            doa_estimates.push_back(angles[i]);
        }
    }

    // Step 6: Sort and return the top DOA estimates
    sort(doa_estimates.begin(), doa_estimates.end(), [&](double a, double b) {
        return pseudo_spectrum[find(angles.begin(), angles.end(), a) - angles.begin()] >
               pseudo_spectrum[find(angles.begin(), angles.end(), b) - angles.begin()];
    });

    if (doa_estimates.size() > num_sources) {
        doa_estimates.resize(num_sources);
    }

    return doa_estimates;
}

