CC = $(shell which gcc)

CFLAGS_INCLUDES = -I./include
CFLAGS_DEBUG = -g -DDEBUG=0
CFLAGS_CC = -std=c11 -Wall
CFLAGS_EX = -pedantic -Wextra $(CFLAGS_DEBUG)
CFLAGS += $(CFLAGS_CC) $(CFLAGS_INCLUDES) $(CFLAGS_EX)

LDFLAGS +=

DEPS_DIR = .deps
DEPS_DEBUG := $(shell mkdir ./$(DEPS_DIR) 1>/dev/null 2>&1)

# Induce dependency creation into implicit "%.o: %.c" target.
#OUTPUT_OPTION = -Wp,-MMD,$(DEPS_DIR)/$(*F).P $(OUTPUT_OPTION)
COMPILE.c += -Wp,-MMD,$(DEPS_DIR)/$(*F).P

# Choose default compressor implementation if
TYPE_CODER ?= naive

TARGETS = test_ghc_decode test_ghc_encode

test_ghc_decode_SOURCES := test_ghc_decode.c src/ghc_decode.c
test_ghc_decode_OBJECTS = $(test_ghc_decode_SOURCES:.c=.o)

test_ghc_encode_SOURCES := test_ghc_encode.c src/ghc_decode.c \
	src/ghc_encode-$(TYPE_CODER).c
test_ghc_encode_OBJECTS = $(test_ghc_encode_SOURCES:.c=.o)

.PHONY: all run-tests clean

all: $(TARGETS)

-include $(DEPS_DIR)/*.P

test_ghc_decode: $(test_ghc_decode_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_ghc_encode: $(test_ghc_encode_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^


run-tests: $(TARGETS)
	$(foreach TARGET, $(TARGETS), ./$(TARGET);)

clean:
	$(RM) $(TARGETS) $(foreach TARGET,$(TARGETS),\
		$($(TARGET)_OBJECTS)) $(DEPS_DIR)/*.P
