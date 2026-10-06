import wave
import struct
import math
import random
import os

SAMPLE_RATE = 44100

def write_wav(filepath, samples):
    os.makedirs(os.path.dirname(filepath), exist_ok=True)
    with wave.open(filepath, 'w') as wav_file:
        wav_file.setnchannels(1)  # Mono
        wav_file.setsampwidth(2)  # 16-bit
        wav_file.setframerate(SAMPLE_RATE)
        
        packed_frames = bytearray()
        for s in samples:
            clamped = max(-1.0, min(1.0, s))
            int_val = int(clamped * 32760.0)
            packed_frames.extend(struct.pack('<h', int_val))
        wav_file.writeframes(packed_frames)
    print(f"Generated: {filepath} ({len(samples)} samples)")

def generate_musical_note(freq, duration=0.45):
    num_samples = int(SAMPLE_RATE * duration)
    samples = []
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        env = t / 0.005 if t < 0.005 else math.exp(-3.5 * (t - 0.005))
        val = (1.00 * math.sin(2 * math.pi * freq * t) +
               0.35 * math.sin(2 * math.pi * freq * 2 * t) +
               0.15 * math.sin(2 * math.pi * freq * 3 * t) +
               0.08 * math.sin(2 * math.pi * freq * 4 * t)) * 0.7
        samples.append(val * env)
    return samples

def generate_fire_spell(duration=0.6):
    num_samples = int(SAMPLE_RATE * duration)
    samples = []
    random.seed(42)
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        env = math.sin(math.pi * (t / duration)) ** 0.8
        noise = (random.random() * 2.0 - 1.0) * 0.6
        sweep_freq = 400.0 * (1.0 - 0.6 * (t / duration))
        sweep = math.sin(2 * math.pi * sweep_freq * t)
        val = (0.5 * noise + 0.5 * sweep) * env
        samples.append(val * 0.8)
    return samples

def generate_heal_spell(duration=0.8):
    notes = [523.25, 659.25, 783.99, 1046.50]
    total_samples = int(SAMPLE_RATE * duration)
    samples = [0.0] * total_samples
    note_duration = 0.35
    note_step = 0.12
    for idx, freq in enumerate(notes):
        start_time = idx * note_step
        start_idx = int(start_time * SAMPLE_RATE)
        for i in range(int(note_duration * SAMPLE_RATE)):
            sample_idx = start_idx + i
            if sample_idx >= total_samples: break
            t = i / SAMPLE_RATE
            env = math.exp(-6.0 * t) if t > 0.01 else t / 0.01
            tone = (math.sin(2 * math.pi * freq * t) + 0.3 * math.sin(2 * math.pi * freq * 2 * t))
            samples[sample_idx] += tone * env * 0.25
    return samples

def generate_shield_spell(duration=0.7):
    num_samples = int(SAMPLE_RATE * duration)
    samples = []
    freq = 349.23
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        env = (1.0 - t / duration) * (1.0 - math.exp(-30.0 * t))
        shimmer = 1.0 + 0.3 * math.sin(2 * math.pi * 12.0 * t)
        tone = (math.sin(2 * math.pi * freq * t) + 
                0.5 * math.sin(2 * math.pi * (freq * 1.5) * t) +
                0.3 * math.sin(2 * math.pi * (freq * 2.0) * t)) * shimmer * 0.5
        samples.append(tone * env * 0.7)
    return samples

def generate_dark_spell(duration=0.65):
    num_samples = int(SAMPLE_RATE * duration)
    samples = []
    random.seed(99)
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        env = (1.0 - (t / duration) ** 1.5) * min(1.0, t / 0.02)
        base = 110.0
        tone1 = math.sin(2 * math.pi * base * t)
        tone2 = math.sin(2 * math.pi * (base * 1.03) * t)
        grit = (random.random() * 2.0 - 1.0) * 0.25 * (1.0 - t / duration)
        samples.append((tone1 * 0.4 + tone2 * 0.4 + grit) * env * 0.8)
    return samples

def generate_fizzle(duration=0.4):
    num_samples = int(SAMPLE_RATE * duration)
    samples = []
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        env = math.exp(-5.0 * t)
        f = 350.0 - 200.0 * (t / duration)
        val = math.sin(2 * math.pi * f * t)
        val = max(-0.5, min(0.5, val * 2.0))
        samples.append(val * env * 0.6)
    return samples

def generate_undo(duration=0.35):
    num_samples = int(SAMPLE_RATE * duration)
    samples = []
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        env = math.exp(-6.0 * t) if t > 0.005 else t / 0.005
        f = 440.0 + 440.0 * (t / duration)
        val = math.sin(2 * math.pi * f * t) + 0.3 * math.sin(4 * math.pi * f * t)
        samples.append(val * env * 0.6)
    return samples

def generate_enemy_detect(duration=0.45):
    num_samples = int(SAMPLE_RATE * duration)
    samples = []
    random.seed(123)
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        env = (1.0 - t / duration) * min(1.0, t / 0.01)
        freq = 280.0 + 150.0 * math.sin(2 * math.pi * 8.0 * t)
        tone = math.sin(2 * math.pi * freq * t)
        noise = (random.random() * 2.0 - 1.0) * 0.3
        samples.append((tone * 0.6 + noise) * env * 0.7)
    return samples

def generate_enemy_attack(duration=0.3):
    num_samples = int(SAMPLE_RATE * duration)
    samples = []
    random.seed(77)
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        env = math.sin(math.pi * (t / duration)) ** 0.5
        noise = (random.random() * 2.0 - 1.0)
        samples.append(noise * env * 0.7)
    return samples

def generate_enemy_hit(duration=0.25):
    num_samples = int(SAMPLE_RATE * duration)
    samples = []
    random.seed(55)
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        env = math.exp(-14.0 * t)
        f = 160.0 * (1.0 - t / duration)
        thud = math.sin(2 * math.pi * f * t)
        noise = (random.random() * 2.0 - 1.0) * 0.4
        samples.append((thud * 0.7 + noise) * env * 0.8)
    return samples

def generate_enemy_death(duration=0.7):
    num_samples = int(SAMPLE_RATE * duration)
    samples = []
    random.seed(33)
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        env = (1.0 - (t / duration) ** 2) * min(1.0, t / 0.03)
        f = 180.0 - 100.0 * (t / duration)
        tone = math.sin(2 * math.pi * f * t)
        noise = (random.random() * 2.0 - 1.0) * 0.4
        samples.append((tone * 0.6 + noise) * env * 0.7)
    return samples

def generate_crystal_restore(duration=1.2):
    chord = [523.25, 659.25, 783.99, 987.77, 1046.50]
    num_samples = int(SAMPLE_RATE * duration)
    samples = [0.0] * num_samples
    for freq in chord:
        for i in range(num_samples):
            t = i / SAMPLE_RATE
            env = (1.0 - t / duration) * min(1.0, t / 0.05)
            val = math.sin(2 * math.pi * freq * t) + 0.2 * math.sin(4 * math.pi * freq * t)
            samples[i] += val * env * (0.5 / len(chord))
    return samples

def generate_level_up(duration=0.9):
    fanfare = [392.00, 523.25, 659.25, 783.99]
    total_samples = int(SAMPLE_RATE * duration)
    samples = [0.0] * total_samples
    step = 0.15
    for idx, f in enumerate(fanfare):
        start_idx = int(idx * step * SAMPLE_RATE)
        dur = 0.45 if idx == len(fanfare) - 1 else 0.25
        for i in range(int(dur * SAMPLE_RATE)):
            pos = start_idx + i
            if pos >= total_samples: break
            t = i / SAMPLE_RATE
            env = math.exp(-4.0 * t) if t > 0.01 else t / 0.01
            val = math.sin(2 * math.pi * f * t) + 0.3 * math.sin(4 * math.pi * f * t)
            samples[pos] += val * env * 0.3
    return samples

def generate_forest_ambience(duration=4.0):
    num_samples = int(SAMPLE_RATE * duration)
    samples = []
    random.seed(101)
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        wind = math.sin(2 * math.pi * 0.2 * t) * 0.5 + 0.5
        noise = (random.random() * 2.0 - 1.0) * 0.06 * wind
        hum = math.sin(2 * math.pi * 130.81 * t) * 0.08
        hum2 = math.sin(2 * math.pi * 196.00 * t) * 0.05
        samples.append(noise + hum + hum2)
    return samples

def generate_forest_theme(duration=6.0):
    melody_notes = [
        (261.63, 0.0, 1.0),
        (329.63, 1.0, 1.0),
        (392.00, 2.0, 1.0),
        (440.00, 3.0, 1.0),
        (392.00, 4.0, 1.0),
        (523.25, 5.0, 1.0),
    ]
    total_samples = int(SAMPLE_RATE * duration)
    samples = [0.0] * total_samples
    for i in range(total_samples):
        t = i / SAMPLE_RATE
        drone = math.sin(2 * math.pi * 130.81 * t) * 0.07 + math.sin(2 * math.pi * 196.00 * t) * 0.05
        samples[i] += drone
    for freq, start_sec, dur_sec in melody_notes:
        start_idx = int(start_sec * SAMPLE_RATE)
        for i in range(int(dur_sec * SAMPLE_RATE)):
            pos = start_idx + i
            if pos >= total_samples: break
            t = i / SAMPLE_RATE
            env = math.exp(-2.5 * t) if t > 0.02 else t / 0.02
            tone = (math.sin(2 * math.pi * freq * t) + 0.25 * math.sin(2 * math.pi * freq * 2 * t))
            samples[pos] += tone * env * 0.18
    return samples

# ================= PHASE 4 NEW SOUNDS =================

def generate_cave_ambience(duration=4.5):
    # Deep subterranean wind + echoing water drips
    num_samples = int(SAMPLE_RATE * duration)
    samples = [0.0] * num_samples
    random.seed(202)
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        sub_bass = math.sin(2 * math.pi * 55.0 * t) * 0.15 # 55Hz low hum
        hollow = math.sin(2 * math.pi * 165.0 * t) * 0.06
        noise = (random.random() * 2.0 - 1.0) * 0.04
        samples[i] = (sub_bass + hollow + noise) * 0.5

    # Water drip spikes at 1.0s, 2.3s, 3.7s
    drips = [1.0, 2.3, 3.7]
    for d in drips:
        d_idx = int(d * SAMPLE_RATE)
        for i in range(int(0.25 * SAMPLE_RATE)):
            idx = d_idx + i
            if idx >= num_samples: break
            t = i / SAMPLE_RATE
            f = 1200.0 - 400.0 * (t / 0.25)
            env = math.exp(-25.0 * t)
            samples[idx] += math.sin(2 * math.pi * f * t) * env * 0.35
    return samples

def generate_cave_theme(duration=6.5):
    # Mysterious minor pentatonic echoing cave theme (D minor: D3, F3, G3, A3, C4, D4)
    melody_notes = [
        (146.83, 0.0, 1.2),  # D3
        (174.61, 1.2, 1.0),  # F3
        (220.00, 2.2, 1.2),  # A3
        (261.63, 3.4, 0.9),  # C4
        (293.66, 4.3, 1.2),  # D4
        (220.00, 5.5, 1.0),  # A3
    ]
    total_samples = int(SAMPLE_RATE * duration)
    samples = [0.0] * total_samples
    for i in range(total_samples):
        t = i / SAMPLE_RATE
        drone = math.sin(2 * math.pi * 73.42 * t) * 0.08 + math.sin(2 * math.pi * 110.0 * t) * 0.05
        samples[i] += drone

    for freq, start_sec, dur_sec in melody_notes:
        start_idx = int(start_sec * SAMPLE_RATE)
        for i in range(int(dur_sec * SAMPLE_RATE)):
            pos = start_idx + i
            if pos >= total_samples: break
            t = i / SAMPLE_RATE
            env = math.exp(-2.2 * t) if t > 0.03 else t / 0.03
            # Shimmer harmonic
            tone = (math.sin(2 * math.pi * freq * t) + 
                    0.3 * math.sin(2 * math.pi * freq * 2 * t) +
                    0.15 * math.sin(2 * math.pi * freq * 3 * t))
            samples[pos] += tone * env * 0.22
            # Simulated cave echo (delayed by 0.22s)
            echo_pos = pos + int(0.22 * SAMPLE_RATE)
            if echo_pos < total_samples:
                samples[echo_pos] += tone * env * 0.09
    return samples

def generate_chain_move(duration=0.4):
    num_samples = int(SAMPLE_RATE * duration)
    samples = []
    random.seed(88)
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        env = math.exp(-7.0 * t)
        metallic = math.sin(2 * math.pi * 880.0 * t) * 0.3 + math.sin(2 * math.pi * 1400.0 * t) * 0.2
        rattle = (random.random() * 2.0 - 1.0) * 0.4
        samples.append((metallic + rattle) * env * 0.6)
    return samples

def generate_chain_link(duration=0.5):
    # Electric harmonic chain beam sound
    num_samples = int(SAMPLE_RATE * duration)
    samples = []
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        env = (1.0 - t / duration) * min(1.0, t / 0.01)
        hum = math.sin(2 * math.pi * 440.0 * t) * 0.4
        buzz = math.sin(2 * math.pi * 880.0 * t) * 0.2
        zap = math.sin(2 * math.pi * (660.0 + 300.0 * math.sin(2 * math.pi * 35.0 * t)) * t) * 0.3
        samples.append((hum + buzz + zap) * env * 0.7)
    return samples

def generate_queue_surge(duration=0.55):
    # Rapid ascending burst pulses (FIFO)
    notes = [329.63, 440.00, 587.33, 783.99]
    total_samples = int(SAMPLE_RATE * duration)
    samples = [0.0] * total_samples
    step = 0.10
    for idx, f in enumerate(notes):
        start_idx = int(idx * step * SAMPLE_RATE)
        for i in range(int(0.2 * SAMPLE_RATE)):
            pos = start_idx + i
            if pos >= total_samples: break
            t = i / SAMPLE_RATE
            env = math.exp(-9.0 * t) if t > 0.005 else t / 0.005
            tone = math.sin(2 * math.pi * f * t) + 0.3 * math.sin(4 * math.pi * f * t)
            samples[pos] += tone * env * 0.28
    return samples

def generate_bat_screech(duration=0.35):
    num_samples = int(SAMPLE_RATE * duration)
    samples = []
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        env = math.sin(math.pi * (t / duration)) ** 0.5
        freq = 1600.0 + 900.0 * math.sin(2 * math.pi * 14.0 * t)
        val = math.sin(2 * math.pi * freq * t) * 0.6
        samples.append(val * env * 0.7)
    return samples

def generate_crawler_slam(duration=0.4):
    num_samples = int(SAMPLE_RATE * duration)
    samples = []
    random.seed(44)
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        env = math.exp(-12.0 * t)
        thud = math.sin(2 * math.pi * (110.0 - 50.0 * (t / duration)) * t) * 0.7
        crunch = (random.random() * 2.0 - 1.0) * 0.4
        samples.append((thud + crunch) * env * 0.8)
    return samples

def generate_guardian_roar(duration=0.9):
    num_samples = int(SAMPLE_RATE * duration)
    samples = []
    random.seed(66)
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        env = (1.0 - (t / duration) ** 2) * min(1.0, t / 0.05)
        growl = math.sin(2 * math.pi * (130.0 + 40.0 * math.sin(2 * math.pi * 6.0 * t)) * t) * 0.6
        grit = (random.random() * 2.0 - 1.0) * 0.3
        samples.append((growl + grit) * env * 0.85)
    return samples

def generate_guardian_vulnerable(duration=0.75):
    # Crystal shield shattering resonance
    chord = [783.99, 987.77, 1318.51]
    num_samples = int(SAMPLE_RATE * duration)
    samples = [0.0] * num_samples
    for f in chord:
        for i in range(num_samples):
            t = i / SAMPLE_RATE
            env = math.exp(-6.0 * t)
            samples[i] += math.sin(2 * math.pi * f * t) * env * 0.25
    return samples

def generate_linked_list_insert(duration=0.3):
    num_samples = int(SAMPLE_RATE * duration)
    samples = []
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        env = math.exp(-8.0 * t) if t > 0.005 else t / 0.005
        tone = math.sin(2 * math.pi * 523.25 * t) + 0.3 * math.sin(2 * math.pi * 1046.5 * t)
        samples.append(tone * env * 0.5)
    return samples

def generate_linked_list_reverse(duration=0.45):
    num_samples = int(SAMPLE_RATE * duration)
    samples = []
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        env = math.exp(-5.0 * t)
        f = 880.0 - 440.0 * (t / duration) # Descending to indicate reversal
        tone = math.sin(2 * math.pi * f * t)
        samples.append(tone * env * 0.55)
    return samples

def generate_queue_enqueue(duration=0.25):
    num_samples = int(SAMPLE_RATE * duration)
    samples = []
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        env = math.exp(-10.0 * t)
        tone = math.sin(2 * math.pi * 440.0 * t) + 0.3 * math.sin(2 * math.pi * 660.0 * t)
        samples.append(tone * env * 0.45)
    return samples

def generate_queue_dequeue(duration=0.3):
    num_samples = int(SAMPLE_RATE * duration)
    samples = []
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        env = math.exp(-9.0 * t)
        tone = math.sin(2 * math.pi * 660.0 * t) + 0.3 * math.sin(2 * math.pi * 880.0 * t)
        samples.append(tone * env * 0.45)
    return samples

def main():
    print("Synthesizing Audio Wizard audio assets (Phase 3 + Phase 4)...")
    
    # 1. Notes
    notes = {
        "note_c": 261.63, "note_d": 293.66, "note_e": 329.63,
        "note_f": 349.23, "note_g": 392.00, "note_a": 440.00, "note_b": 493.88,
    }
    for name, freq in notes.items():
        write_wav(f"assets/audio/sfx/{name}.wav", generate_musical_note(freq))
    
    # 2. Spells
    write_wav("assets/audio/spells/fire.wav", generate_fire_spell())
    write_wav("assets/audio/spells/heal.wav", generate_heal_spell())
    write_wav("assets/audio/spells/shield.wav", generate_shield_spell())
    write_wav("assets/audio/spells/dark.wav", generate_dark_spell())
    write_wav("assets/audio/spells/fizzle.wav", generate_fizzle())
    write_wav("assets/audio/spells/undo.wav", generate_undo())
    
    # 3. Combat / SFX
    write_wav("assets/audio/sfx/enemy_detect.wav", generate_enemy_detect())
    write_wav("assets/audio/sfx/enemy_attack.wav", generate_enemy_attack())
    write_wav("assets/audio/sfx/enemy_hit.wav", generate_enemy_hit())
    write_wav("assets/audio/sfx/enemy_death.wav", generate_enemy_death())
    write_wav("assets/audio/sfx/crystal_restore.wav", generate_crystal_restore())
    write_wav("assets/audio/sfx/level_up.wav", generate_level_up())
    
    # 4. Ambience & Music (Phase 3)
    write_wav("assets/audio/ambience/forest_ambience.wav", generate_forest_ambience())
    write_wav("assets/audio/music/forest_theme.wav", generate_forest_theme())

    # 5. Phase 4 - Chained Cave Audio
    write_wav("assets/audio/ambience/cave_ambience.wav", generate_cave_ambience())
    write_wav("assets/audio/music/cave_theme.wav", generate_cave_theme())
    write_wav("assets/audio/sfx/chain_move.wav", generate_chain_move())
    write_wav("assets/audio/sfx/chain_link.wav", generate_chain_link())
    write_wav("assets/audio/sfx/queue_surge.wav", generate_queue_surge())
    write_wav("assets/audio/sfx/bat_screech.wav", generate_bat_screech())
    write_wav("assets/audio/sfx/crawler_slam.wav", generate_crawler_slam())
    write_wav("assets/audio/sfx/guardian_roar.wav", generate_guardian_roar())
    write_wav("assets/audio/sfx/guardian_vulnerable.wav", generate_guardian_vulnerable())
    write_wav("assets/audio/sfx/linked_list_insert.wav", generate_linked_list_insert())
    write_wav("assets/audio/sfx/linked_list_reverse.wav", generate_linked_list_reverse())
    write_wav("assets/audio/sfx/queue_enqueue.wav", generate_queue_enqueue())
    write_wav("assets/audio/sfx/queue_dequeue.wav", generate_queue_dequeue())
    
    print("All audio assets synthesized successfully!")

if __name__ == '__main__':
    main()
