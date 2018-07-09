CC = $(shell which gcc)

CFLAGS_INCLUDES = -I./include
CFLAGS_DEBUG = -g -DDEBUG=0
CFLAGS_CC = -std=c11 -Wall
CFLAGS_EX = -pedantic -Wextra $(CFLAGS_DEBUG)
CFLAGS += $(CFLAGS_CC) $(CFLAGS_INCLUDES) $(CFLAGS_EX)

TARGETS = check_ghc_codec
SOURCES_check_ghc_codec = check_ghc_codec.c src/ghc_codec.c
OBJECTS_check_ghc_codec = $(SOURCES_check_ghc_codec:%.c=%.o)

LDFLAGS +=

all: $(TARGETS)

check_ghc_codec: $(OBJECTS_check_ghc_codec)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	$(RM) $(TARGETS) $(OBJECTS_check_ghc_codec)
