import numpy as np

def doppler_processing(range_data):
    doppler_fft = np.fft.fft(range_data, axis=0)
    return np.abs(doppler_fft)
