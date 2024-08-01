function range_profile = range_processing(rx_signal, num_samples)
    % RANGE_PROCESSING Perform range processing on received signal
    %   range_profile = RANGE_PROCESSING(rx_signal, num_samples)
    %
    %   Inputs:
    %       rx_signal: Complex received signal (1 x num_samples)
    %       num_samples: Number of samples in the received signal
    %
    %   Output:
    %       range_profile: Magnitude of the FFT of the windowed signal

    % Apply Hann window
    hann_window = 0.5 * (1 - cos(2 * pi * (0:num_samples-1) / (num_samples-1)));
    windowed_signal = hann_window .* rx_signal;

    % Perform FFT
    fft_result = fft(windowed_signal, num_samples);

    % Compute magnitude of FFT output
    range_profile = abs(fft_result);

    % Normalize range profile
    range_profile = range_profile / max(range_profile);
end

% Example usage
num_samples = 1024;

% Generate a sample received signal (replace with actual radar data)
t = (0:num_samples-1) / num_samples;
rx_signal = exp(1j * 2 * pi * 10 * t); % Example: 10 Hz complex sinusoid

% Process the range data
range_profile = range_processing(rx_signal, num_samples);

% Plot the range profile
figure;
plot(range_profile);
xlabel('Range Bin');
ylabel('Magnitude');
title('Range Profile');
grid on;

% Display the first few values of the range profile
disp('Range Profile (first 10 values):');
disp(range_profile(1:10));
