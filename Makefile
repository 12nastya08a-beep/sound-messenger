UNAME := $(shell uname)

CFLAGS := -Wall -Wextra -std=c11 -Ithird_party -Iinclude

ifeq ($(UNAME), Linux)
	LDFLAGS := -lpthread -lm -ldl
endif
ifeq ($(UNAME), Darwin)
	LDFLAGS := -framework CoreFoundation -framework CoreAudio -framework AudioToolbox -lpthread -lm
endif

demo_tone: demo/demo_tone.c
	cc $(CFLAGS) demo/demo_tone.c -o demo_tone $(LDFLAGS)

test_protocol: src/protocol.c tests/test_protocol.c
	cc $(CFLAGS) src/protocol.c tests/test_protocol.c -o test_protocol $(LDFLAGS)

test_encoder: src/protocol.c src/encoder.c tests/test_encoder.c
	cc $(CFLAGS) src/protocol.c src/encoder.c tests/test_encoder.c -o test_encoder $(LDFLAGS)

test_decoder_fft: third_party/kiss_fft.c src/decoder_fft.c tests/test_decoder_fft.c
	cc $(CFLAGS) third_party/kiss_fft.c src/decoder_fft.c tests/test_decoder_fft.c -o test_decoder_fft $(LDFLAGS)

test_decoder_assemble: src/protocol.c src/decoder_assemble.c tests/test_decoder_assemble.c
	cc $(CFLAGS) src/protocol.c src/decoder_assemble.c tests/test_decoder_assemble.c -o test_decoder_assemble $(LDFLAGS)

run_test: test_protocol test_encoder test_decoder_fft test_decoder_assemble
	./test_protocol
	./test_encoder
	./test_decoder_fft
	./test_decoder_assemble

clean:
	rm -f demo_tone demo_loopback test_protocol test_encoder test_decoder_fft test_decoder_assemble
demo_loopback: demo/demo_loopback.c src/audio_listen.c src/encoder.c src/protocol.c src/decoder_fft.c src/decoder_assemble.c third_party/kiss_fft.c
	cc $(CFLAGS) demo/demo_loopback.c src/audio_listen.c src/encoder.c src/protocol.c src/decoder_fft.c src/decoder_assemble.c third_party/kiss_fft.c -o demo_loopback $(LDFLAGS)
