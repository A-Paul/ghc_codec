CC = $(shell which gcc)

CFLAGS_INCLUDES = -I./include
CFLAGS_DEBUG = -g
CFLAGS_CC = -strict -std=c11 -Wall
CFLAGS += $(CFLAGS_CC) $(CFLAGS_INCLUDES)

TARGETS = check_ghc_codec
OBJECT_check_ghc_codec = $(TARGETS:%=%.o)

LDFLAGS +=

all: $(TARGETS)

check_ghc_codec: $(OBJECTS_check_ghc_codec) 

clean:
	$(RM) $(TARGETS) $(OBJECTS)
