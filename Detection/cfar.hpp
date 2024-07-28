#pragma once
#include <vector>

using namespace std;

vector<bool> ca_cfar(const vector<double>& signal, int num_guard_cells, int num_training_cells, double pfa);
