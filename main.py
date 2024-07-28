# main_radar_processing.py

import numpy as np
from range_processing import range_processing
from doppler_processing import doppler_processing
from cfar import ca_cfar
from doa import music_doa
from tracking import initialize_tracker, update_tracks
from classification import classify_target

def acquire_radar_data():
    # Placeholder function - replace with actual data acquisition
    return np.random.randn(2, 8, 128, 512) + 1j * np.random.randn(2, 8, 128, 512)

def main_radar_processing():
    # System parameters
    num_rx_antennas = 8
    num_tx_antennas = 2
    num_range_samples = 512
    num_chirps = 128
    num_guard_cells = 4
    num_training_cells = 16
    pfa = 1e-6
    prf_list = [1000, 1100, 1200]
    angles = np.linspace(-90, 90, 181)

    # Acquire raw radar data
    raw_data = acquire_radar_data()

    # Range and Doppler processing
    range_doppler_data = np.zeros_like(raw_data)
    for tx in range(num_tx_antennas):
        for rx in range(num_rx_antennas):
            range_data = range_processing(raw_data[tx, rx], num_range_samples)
            range_doppler_data[tx, rx] = doppler_processing(range_data)

    # CFAR detection
    detections = np.zeros_like(range_doppler_data, dtype=bool)
    for tx in range(num_tx_antennas):
        for rx in range(num_rx_antennas):
            detections[tx, rx], _ = ca_cfar(np.abs(range_doppler_data[tx, rx]), 
                                            num_guard_cells, num_training_cells, pfa)

    # Process detections
    targets = []
    for tx in range(num_tx_antennas):
        for rx in range(num_rx_antennas):
            range_indices, doppler_indices = np.where(detections[tx, rx])
            for range_idx, doppler_idx in zip(range_indices, doppler_indices):
                range_val = range_idx * (3e8 / (2 * 1e9))  # Assuming 1 GHz bandwidth
                doppler_val = doppler_idx * (prf_list[0] / num_chirps)
                
                unambiguous_doppler = resolve_doppler_ambiguity([doppler_val], prf_list)[0]
                
                targets.append({
                    'range': range_val,
                    'doppler': unambiguous_doppler,
                    'tx': tx,
                    'rx': rx,
                    'magnitude': np.abs(range_doppler_data[tx, rx, range_idx, doppler_idx])
                })

    # Direction of Arrival Estimation
    for target in targets:
        signal_vector = range_doppler_data[:, :, target['range'], target['doppler']]
        cov_matrix = np.cov(signal_vector)
        doa_estimates = music_doa(cov_matrix, num_sources=1, angles=angles)
        target['azimuth'] = doa_estimates[0]

    # Classification and Tracking
    tracker = initialize_tracker()
    for target in targets:
        features = [target['range'], target['doppler'], target['magnitude'], target['azimuth']]
        target['class'] = classify_target(features)
        
        measurement = np.array([target['range'], target['azimuth']])
        target['track'] = update_tracks(tracker, measurement)

    return targets

if __name__ == "__main__":
    detected_targets = main_radar_processing()
    
    for i, target in enumerate(detected_targets):
        print(f"Target {i+1}:")
        print(f"  Range: {target['range']:.2f} m")
        print(f"  Doppler: {target['doppler']:.2f} m/s")
        print(f"  Azimuth: {target['azimuth']:.2f} degrees")
        print(f"  Class: {target['class']}")
        print(f"  Track: {target['track']}")
        print()
