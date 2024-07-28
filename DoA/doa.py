import numpy as np
from scipy.signal import find_peaks

def music_doa(covariance_matrix, num_sources, angles):
    eigenvalues, eigenvectors = np.linalg.eig(covariance_matrix)
    sorted_indices = np.argsort(eigenvalues)[::-1]
    
    noise_subspace = eigenvectors[:, sorted_indices[num_sources:]]
    
    music_spectrum = np.zeros_like(angles)
    
    for i, angle in enumerate(angles):
        a = np.exp(-1j * 2 * np.pi * np.arange(covariance_matrix.shape[0]) * np.sin(np.radians(angle)))
        music_spectrum[i] = 1 / np.abs(a.conj().T @ noise_subspace @ noise_subspace.conj().T @ a)
    
    peaks, _ = find_peaks(music_spectrum)
    return angles[peaks[:num_sources]]
