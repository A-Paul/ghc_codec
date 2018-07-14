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
TYPE_CODER ?= stub

TARGETS = test_ghc_decompress test_ghc_compress

test_ghc_decompress_SOURCES := test_ghc_decompress.c src/ghc_decompress.c
test_ghc_decompress_OBJECTS = $(test_ghc_decompress_SOURCES:.c=.o)

test_ghc_compress_SOURCES := test_ghc_compress.c src/ghc_decompress.c \
	src/ghc_compress-$(TYPE_CODER).c
test_ghc_compress_OBJECTS = $(test_ghc_compress_SOURCES:.c=.o)

.PHONY: all run-tests clean

all: $(TARGETS)

-include $(DEPS_DIR)/*.P

test_ghc_decompress: $(test_ghc_decompress_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_ghc_compress: $(test_ghc_compress_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^


run-tests: $(TARGETS)
	$(foreach TARGET, $(TARGETS), ./$(TARGET);)

clean:
	$(RM) $(TARGETS) $(foreach TARGET,$(TARGETS),\
		$($(TARGET)_OBJECTS)) $(DEPS_DIR)/*.P
