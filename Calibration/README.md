# Calibration


1. Initialization (__init__ method): Initializes radar parameters from the configuration structure.
2. Loading Raw Data (load_raw_data method): Loads raw ADC data from a .mat file and verifies its shape.
3. RF Calibration (rf_calibration method):
    - DC Offset Removal: Removes the DC offset from the raw data.
    - I/Q Imbalance Correction: Corrects I/Q imbalance.
    - Phase Noise Correction: Placeholder for phase noise correction.
4. Range Calibration (range_calibration method):
    - Range FFT: Performs FFT along the range dimension.
    - Identify Peaks: Identifies peaks corresponding to known reference targets.
    - Correction Factor: Calculates and applies the range correction factor.
5. Doppler Calibration (doppler_calibration method):
    - Doppler FFT: Performs FFT along the Doppler dimension.
    - Zero-Doppler Reference: Uses stationary clutter for zero-Doppler reference.
    - Doppler Correction: Calculates and applies the Doppler correction.
6. Angle Calibration (angle_calibration method):
    - Ideal Antenna Positions: Defines ideal antenna positions.
    - Measure Phase Differences: Placeholder for measuring phase differences.
    - Position Errors: Calculates position errors and stores calibration data.
7. Plot Calibration Results (plot_calibration_results method): Plots the Range-Doppler map, Range profile, and Angle-Range map.

## Usage Example:
1. Define Radar Configuration: Specifies radar parameters.
2. Create Calibration Object: Initializes the RadarCalibration class with the configuration.
3. Load Raw Data: Loads raw data from a .mat file.
4. Perform Calibrations: Executes RF, range, Doppler, and angle calibrations.
5. Plot Results: Visualizes the calibration results.

This MATLAB code provides a structured approach to radar calibration, similar to the provided Python code, with detailed comments explaining each step.


Created on 2019-11-21