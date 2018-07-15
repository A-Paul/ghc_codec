CC = $(shell which gcc)

CFLAGS_INCLUDES = -I./include
CFLAGS_DEBUG = -g -DDEBUG=0
CFLAGS_CC = -std=c11 -Wall
CFLAGS_EX = -pedantic -Wextra $(CFLAGS_DEBUG)
CFLAGS += $(CFLAGS_CC) $(CFLAGS_INCLUDES) $(CFLAGS_EX)

TARGETS = check_ghc_codec
check_ghc_codec_SOURCES = check_ghc_codec.c src/ghc_decompress.c \
src/ghc_compress-stub.c
check_ghc_codec_OBJECTS = $(check_ghc_codec_SOURCES:.c=.o)

LDFLAGS +=

.PHONY: all dep clean

all: $(TARGETS)

dep:
	$(CC) $(CFLAGS_INCLUDES) -E -MMD $(check_ghc_codec_SOURCES) > /dev/null

-include *.d

check_ghc_codec: $(check_ghc_codec_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	$(RM) $(TARGETS) $(check_ghc_codec_OBJECTS)
