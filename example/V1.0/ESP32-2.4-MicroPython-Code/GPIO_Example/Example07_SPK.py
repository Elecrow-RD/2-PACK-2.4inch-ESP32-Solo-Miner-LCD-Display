import math
import time
from machine import Pin, DAC

# ===== 1. Note frequency definition (Common notes from C4 to A5) =====
NOTE_REST = 0       # Rest note
NOTE_C4 = 262
NOTE_D4 = 294
NOTE_E4 = 330
NOTE_F4 = 349
NOTE_G4 = 392
NOTE_A4 = 440
NOTE_B4 = 494
NOTE_C5 = 523
NOTE_D5 = 587
NOTE_E5 = 659
NOTE_F5 = 698
NOTE_G5 = 784
NOTE_A5 = 880

# ===== 2. "Happy Birthday to You" melody + beats =====
# Melody list
HAPPY_BIRTHDAY = [
    NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4, NOTE_REST,
    NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4, NOTE_REST,
    NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_REST,
    NOTE_A4, NOTE_A4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_F4
]
# Beat duration list (Play time of each note in ms, one-to-one with melody)
BEATS = [
    300, 300, 600, 600, 600, 1200, 300,
    300, 300, 600, 600, 600, 1200, 300,
    300, 300, 600, 600, 600, 600, 1200, 300,
    600, 300, 600, 600, 600, 1200
]

# ===== 3. Hardware configuration =====
DAC_PIN = 26       # DAC output pin, adjust according to hardware
dac = DAC(Pin(DAC_PIN))
SAMPLE_RATE = 16000  # Sampling rate 16kHz, higher rate means better sound quality (hardware-supported)
AMP = 127          # Amplitude (0-127), avoid clipping distortion
OFFSET = 128       # DC offset, keep waveform in 0-255 range

# ===== 4. Core playback functions =====
def play_note(freq, duration_ms):
    """Play a single note, optimized timing to reduce noise"""
    if freq == NOTE_REST:
        time.sleep_ms(duration_ms)
        return
    
    # Calculate step size for each sample (sine wave phase increment)
    step = 2 * math.pi * freq / SAMPLE_RATE
    total_samples = int(SAMPLE_RATE * duration_ms / 1000)  # Total number of samples
    
    # Generate waveform and output at once, reduce loop overhead
    for i in range(total_samples):
        sample = int(OFFSET + AMP * math.sin(i * step))
        dac.write(sample)

def play_melody(melody, beats, interval_ms=50):
    """Play complete melody with interval between notes"""
    for note, beat in zip(melody, beats):
        play_note(note, beat)
        time.sleep_ms(interval_ms)  # Interval between notes to avoid adhesion

# ===== 5. Main program =====
def main():
    print("Playing Happy Birthday to You...")
    play_melody(HAPPY_BIRTHDAY, BEATS)
    print("Play finished!")

if __name__ == "__main__":
    main()
    while True:
        pass
