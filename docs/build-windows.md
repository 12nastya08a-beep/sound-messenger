# Build on Windows

Environment: Windows 11, MSYS2 (MinGW-w64), Git Bash.
Tools: gcc (MinGW), mingw32-make.

## Sender (send)

From the project root:

```
gcc -Iinclude -Ithird_party src/send.c src/encoder.c src/wav.c src/protocol.c src/audio_play.c src/miniaudio_impl.c -o send -lm -lole32 -loleaut32 -lwinmm
```

This produces `send.exe`. Run it:

```
./send "HELLO WORLD"          # save to message.wav
./send --live "HELLO WORLD"   # play through the speaker (live)
```

## Receiver (receive)

On Windows the receiver does not currently build: the MinGW linker fails
on the microphone-capture part of miniaudio (used by audio_listen.c).
Sending works. Receiving is done on macOS or Linux.

## Notes

- If a message contains `!`, use single quotes: `./send --live 'HELLO!'`
- The Windows audio libraries `-lole32 -loleaut32 -lwinmm` are required for miniaudio.
