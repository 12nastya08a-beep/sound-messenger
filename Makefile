UNAME := $(shell uname)

CFLAGS := -Wall -Wextra -std=c11 -Ithird_party

ifeq ($(UNAME), Linux)
	LDFLAGS := -lpthread -lm -ldl
endif
ifeq ($(UNAME), Darwin)
	LDFLAGS := -framework CoreFoundation -framework CoreAudio -framework AudioToolbox -lpthread -lm
endif

demo_tone: demo/demo_tone.c
	cc $(CFLAGS) demo/demo_tone.c -o demo_tone $(LDFLAGS)

clean:
	rm -f demo_tone