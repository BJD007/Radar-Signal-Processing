#pragma once
#include <vector>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

vector<double> music_doa(const MatrixXcd& covariance_matrix, int num_sources, const vector<double>& angles);
