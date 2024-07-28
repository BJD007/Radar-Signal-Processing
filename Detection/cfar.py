import numpy as np

def ca_cfar(signal, num_guard_cells, num_training_cells, pfa):
    N = len(signal)
    threshold = np.zeros(N)
    
    for i in range(N):
        start = max(0, i - num_training_cells - num_guard_cells)
        end = min(N, i + num_training_cells + num_guard_cells)
        
        training_cells = np.concatenate((signal[start:i-num_guard_cells], 
                                         signal[i+num_guard_cells:end]))
        
        noise_level = np.mean(training_cells)
        threshold[i] = noise_level * (-np.log(pfa) * (2 * num_training_cells))
    
    detections = signal > threshold
    return detections, threshold
