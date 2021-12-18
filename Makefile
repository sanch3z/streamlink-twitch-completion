CC = gcc
MKDIR = mkdir -p

_CFLAGS := -Wall
_CFLAGS += `pkg-config --cflags --libs libcurl json-c`
CFLAGS ?= $(_CFLAGS)

OUTDIR ?= build
EXECUTABLE = twitch-live

SOURCE = $(wildcard src/*.c)

release: $(SOURCE)
	$(MKDIR) $(OUTDIR)/
	$(CC) -O2 -o $(OUTDIR)/$(EXECUTABLE) $^ $(CFLAGS)

debug: $(SOURCE)
	$(MKDIR) $(OUTDIR)/debug/
	$(CC) -O0 -g -D_DEBUG -o $(OUTDIR)/debug/$(EXECUTABLE) $^ $(CFLAGS)

clean:
	$(RM) -r $(OUTDIR)/

test:
	echo $(ARGS)
