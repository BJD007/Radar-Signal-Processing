function [doa_estimates, pseudo_spectrum] = music_doa(covariance_matrix, num_sources, angles)
% MUSIC_DOA Estimates Direction of Arrival using MUSIC algorithm
%   [doa_estimates, pseudo_spectrum] = MUSIC_DOA(covariance_matrix, num_sources, angles)
%
%   Inputs:
%       covariance_matrix: Spatial covariance matrix of the received signals
%       num_sources: Number of sources to estimate
%       angles: Vector of angles (in degrees) to evaluate
%
%   Outputs:
%       doa_estimates: Estimated directions of arrival (in degrees)
%       pseudo_spectrum: MUSIC pseudo-spectrum for all evaluated angles

    % Step 1: Eigen decomposition of the covariance matrix
    [eigen_vectors, eigen_values] = eig(covariance_matrix);
    [eigen_values, idx] = sort(diag(eigen_values), 'descend');
    eigen_vectors = eigen_vectors(:, idx);

    % Step 2: Extract noise subspace
    noise_subspace = eigen_vectors(:, num_sources+1:end);

    % Step 3: Compute MUSIC pseudo-spectrum
    num_angles = length(angles);
    pseudo_spectrum = zeros(1, num_angles);
    
    for i = 1:num_angles
        angle = angles(i);
        a = exp(-1j * 2 * pi * (0:size(covariance_matrix,1)-1)' * sin(angle * pi / 180));
        pseudo_spectrum(i) = 1 / (a' * (noise_subspace * noise_subspace') * a);
    end

    % Step 4: Find peaks in the pseudo-spectrum
    [~, peak_indices] = findpeaks(pseudo_spectrum);
    peak_angles = angles(peak_indices);

    % Step 5: Sort and return the top DOA estimates
    [~, sorted_indices] = sort(pseudo_spectrum(peak_indices), 'descend');
    doa_estimates = peak_angles(sorted_indices(1:min(num_sources, length(peak_angles))));
end

% Example usage
% Set up parameters
num_sensors = 8;  % Number of sensors in the array
num_sources = 2;  % Number of sources to estimate
snr_db = 10;      % Signal-to-Noise Ratio in dB

% True DOAs
true_doas = [-30, 45];  % in degrees

% Generate steering vectors
angles = -90:0.5:90;  % Angles to evaluate
steering_vectors = exp(-1j * 2 * pi * (0:num_sensors-1)' * sin(angles * pi / 180));

% Generate signals
num_samples = 1000;
signals = randn(num_sources, num_samples) + 1j * randn(num_sources, num_samples);
noise = (randn(num_sensors, num_samples) + 1j * randn(num_sensors, num_samples)) / sqrt(2);

% Combine signals with steering vectors
true_steering = steering_vectors(:, ismember(angles, true_doas));
received = true_steering * signals;

% Add noise
received = received + 10^(-snr_db/20) * noise;

% Estimate covariance matrix
covariance_matrix = received * received' / num_samples;

% Perform MUSIC DOA estimation
[doa_estimates, pseudo_spectrum] = music_doa(covariance_matrix, num_sources, angles);

% Plot results
figure;
plot(angles, 10*log10(pseudo_spectrum));
hold on;
plot(doa_estimates, 10*log10(pseudo_spectrum(ismember(angles, doa_estimates))), 'ro', 'MarkerSize', 10);
plot(true_doas, 10*log10(pseudo_spectrum(ismember(angles, true_doas))), 'g*', 'MarkerSize', 10);
xlabel('Angle (degrees)');
ylabel('Pseudo-spectrum (dB)');
title('MUSIC Algorithm: DOA Estimation');
legend('Pseudo-spectrum', 'Estimated DOAs', 'True DOAs');
grid on;

% Display results
disp('True DOAs:');
disp(true_doas);
disp('Estimated DOAs:');
disp(doa_estimates);

