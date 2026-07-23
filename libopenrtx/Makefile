# libopenrtx Makefile
# Simple build for local development

CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude -fPIC
AR = ar
ARFLAGS = rcs

# Source files
SRC = src/init.c src/control.c src/state.c src/channel.c src/audio/codec.c

# Object files
OBJ = $(SRC:.c=.o)

# Library
LIB = libopenrtx.a

# Test executable
TEST = test_basic
TESTSRC = tests/test_basic.c

.PHONY: all clean test

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(LIB) $(TESTSRC)
	$(CC) $(CFLAGS) -L. -o $(TEST) $(TESTSRC) -lopenrtx -lm
	LD_LIBRARY_PATH=. ./$(TEST)

clean:
	rm -f $(OBJ) $(LIB) $(TEST)

install: $(LIB)
	install -d $(DESTDIR)/usr/local/lib
	install -d $(DESTDIR)/usr/local/include
	install -m 644 $(LIB) $(DESTDIR)/usr/local/lib/
	install -m 644 include/*.h $(DESTDIR)/usr/local/include/