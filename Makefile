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

run_test: test_protocol test_encoder test_decoder_fft
	./test_protocol
	./test_encoder
	./test_decoder_fft

clean:
	rm -f demo_tone test_protocol test_encoder test_decoder_fft