function [doppler_data] = doppler_processing(range_data, prf, window_type)
% DOPPLER_PROCESSING Perform Doppler processing on range data
%   doppler_data = DOPPLER_PROCESSING(range_data, prf, window_type)
%
%   Inputs:
%       range_data: Matrix of complex range data (num_pulses x num_range_bins)
%       prf: Pulse Repetition Frequency in Hz
%       window_type: String specifying the window type ('hamming', 'hanning', 'blackman', or 'none')
%
%   Output:
%       doppler_data: Matrix of Doppler processed data (num_range_bins x num_pulses)

    [num_pulses, num_range_bins] = size(range_data);
    
    % Create window function
    switch lower(window_type)
        case 'hamming'
            window = hamming(num_pulses);
        case 'hanning'
            window = hanning(num_pulses);
        case 'blackman'
            window = blackman(num_pulses);
        case 'none'
            window = ones(num_pulses, 1);
        otherwise
            error('Unsupported window type');
    end
    
    % Apply window function to each range bin
    windowed_data = range_data .* repmat(window, 1, num_range_bins);
    
    % Perform FFT along pulse dimension
    doppler_data = fftshift(fft(windowed_data, [], 1), 1);
    
    % Compute magnitude
    doppler_data = abs(doppler_data);
    
    % Transpose to have range bins as rows and Doppler bins as columns
    doppler_data = doppler_data.';
end

% Example usage and visualization
% Set radar parameters
num_pulses = 128;
num_range_bins = 256;
prf = 1000; % Pulse Repetition Frequency in Hz
c = 3e8; % Speed of light in m/s
fc = 77e9; % Carrier frequency (77 GHz)

% Create simulated range data with a moving target
range_data = zeros(num_pulses, num_range_bins);
target_range_bin = 100;
target_velocity = 20; % m/s

for pulse = 1:num_pulses
    % Simulate target echo
    target_phase = 4 * pi * fc * target_velocity * (pulse - 1) / (c * prf);
    range_data(pulse, target_range_bin) = exp(1j * target_phase);
end

% Add some noise
noise_level = 0.1;
range_data = range_data + noise_level * (randn(size(range_data)) + 1j * randn(size(range_data)));

% Perform Doppler processing
doppler_data = doppler_processing(range_data, prf, 'hamming');

% Calculate Doppler frequency axis
doppler_axis = linspace(-prf/2, prf/2, num_pulses);

% Calculate range axis (assuming 15m range resolution)
range_resolution = 15;
range_axis = (0:num_range_bins-1) * range_resolution;

% Plot Range-Doppler map
figure;
imagesc(doppler_axis, range_axis, 20*log10(doppler_data));
xlabel('Doppler Frequency (Hz)');
ylabel('Range (m)');
title('Range-Doppler Map');
colorbar;
axis xy;

% Plot Doppler profile at the target range bin
figure;
plot(doppler_axis, 20*log10(doppler_data(target_range_bin, :)));
xlabel('Doppler Frequency (Hz)');
ylabel('Magnitude (dB)');
title(['Doppler Profile at Range Bin ', num2str(target_range_bin)]);
grid on;

% Estimate target velocity
[~, max_doppler_bin] = max(doppler_data(target_range_bin, :));
estimated_doppler_freq = doppler_axis(max_doppler_bin);
estimated_velocity = estimated_doppler_freq * c / (2 * fc);

disp(['Estimated target velocity: ', num2str(estimated_velocity), ' m/s']);
