import numpy as np

def range_processing(rx_signal, num_samples):
    windowed_signal = np.hanning(len(rx_signal)) * rx_signal
    range_fft = np.fft.fft(windowed_signal, num_samples)
    return np.abs(range_fft)
