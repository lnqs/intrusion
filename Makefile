CC = clang
CFLAGS = -std=c99 -Wall -Werror -ggdb -O0 $(shell pkg-config --cflags sdl) $(shell pkg-config --cflags gl)
LDFLAGS = $(shell pkg-config --libs sdl) $(shell pkg-config --libs gl)

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = planeshift

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(MAKEFILE_LIST)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

%.o: %.c %.d $(MAKEFILE_LIST)
	$(CC) -c $(CFLAGS) $< -o $@

%.d: %.c
	@$(SHELL) -ec '$(CC) -M $(CFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

clean:
	rm -rf *.o *.d $(EXECUTABLE)

-include $(SOURCES:.c=.d)

