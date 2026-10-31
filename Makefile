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

run_test: test_protocol
	./test_protocol

clean:
	rm -f demo_tone test_protocol