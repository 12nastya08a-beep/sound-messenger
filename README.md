# Sound Messenger

Data transfer between nearby devices over sound, with no network, Wi-Fi, or Bluetooth.

One device encodes a text message into a sequence of audio tones and plays it through its speaker. Another device listens with its microphone, recognizes the tones, and reconstructs the original text. The only channel is sound in the air.

## Problem

Sometimes two devices are close to each other but have no shared network: different Wi-Fi, Bluetooth that will not pair, or no internet at all. Sound Messenger lets them exchange short text messages using only a speaker and a microphone, which every laptop already has.

## How it works

1. The text is turned into bits (each character is 8 bits).
2. Each bit becomes a short tone: bit `0` is a low frequency, bit `1` is a high frequency (frequency-shift keying).
3. A short beacon (`1010`) is added at the start so the receiver knows when a message begins, and a checksum byte is added at the end to verify integrity.
4. All tones are joined into one audio signal and played, or saved to a `.wav` file.
5. The receiver records the sound, splits it into per-bit chunks, and uses a Fourier transform to detect which frequency sounded in each chunk, turning it back into bits.
6. The bits are reassembled into text and verified against the checksum.

## Technologies

- Language: C (C11)
- [miniaudio](https://github.com/mackron/miniaudio) for audio playback and capture
- [kissfft](https://github.com/mborgerding/kissfft) for frequency analysis (Fourier transform)
- Cross-platform: Windows, Linux, macOS

## Project structure

- `src/encoder.c` — text to tones (sender)
- `src/decoder_fft.c` — detect which frequency a chunk contains (one bit)
- `src/decoder_assemble.c` — bits back into text, checksum verification
- `src/wav.c` / `src/wav_reader.c` — write and read `.wav` files
- `src/audio_play.c` — play a sample buffer through the speaker (live)
- `src/audio_listen.c` — listen on the microphone and capture a message (live)
- `src/protocol.c` / `include/protocol.h` — shared constants (frequencies, bit duration, beacon, checksum)
- `src/send.c` / `src/receive.c` — the two command-line programs
- `third_party/` — miniaudio and kissfft

## Build

**macOS:**
```
clang -Iinclude -Ithird_party src/send.c src/encoder.c src/wav.c src/protocol.c src/audio_play.c src/miniaudio_impl.c -o send -lm
clang -Iinclude -Ithird_party src/receive.c src/wav_reader.c src/decoder_fft.c src/decoder_assemble.c src/protocol.c src/audio_listen.c src/encoder.c src/miniaudio_impl.c third_party/kiss_fft.c -o receive -lm
```

**Linux:**
```
cc -Iinclude -Ithird_party src/send.c src/encoder.c src/wav.c src/protocol.c src/audio_play.c src/miniaudio_impl.c -o send -lm -lpthread -ldl
cc -Iinclude -Ithird_party src/receive.c src/wav_reader.c src/decoder_fft.c src/decoder_assemble.c src/protocol.c src/audio_listen.c src/encoder.c src/miniaudio_impl.c third_party/kiss_fft.c -o receive -lm -lpthread -ldl
```

**Windows (MinGW):** sending works; see Limitations about receiving.
```
gcc -Iinclude -Ithird_party src/send.c src/encoder.c src/wav.c src/protocol.c src/audio_play.c src/miniaudio_impl.c -o send -lm -lole32 -loleaut32 -lwinmm
```

## Usage

**File mode** (encode to a `.wav`, then decode it back):
```
./send "HELLO WORLD"
./receive message.wav
```

**Live mode** (over the air). Start the receiver first, then the sender:
```
./receive --live
./send --live "HELLO WORLD"
```
The receiver prints the decoded message, saves it to `received.txt`, and opens that file automatically.

Tip: if a message contains `!`, use single quotes: `./send --live 'HELLO!'`

## Limitations

- Works over short distances only; the devices must be near each other.
- Sensitive to background noise; a quiet environment is needed for reliable decoding.
- Supports ASCII characters (letters, digits, punctuation).
- On Windows the receiver does not currently build due to a linker issue specific to the MinGW toolchain; sending works. Receiving is done on macOS or Linux.

## Team

- Daryna Bohaievska — Windows, sender, project integration
- Darya Miroshnichenko — macOS, microphone listening
- Anastasia Mykytiuk — Linux, repository owner, frequency analysis

NaUKMA, Procedural Programming, 2026
