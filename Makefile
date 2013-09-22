CC = clang
CFLAGS = -m32 -std=c99 -Wall -Werror -ggdb -O0 $(shell pkg-config --cflags sdl) $(shell pkg-config --cflags gl) $(shell pkg-config --cflags glew)
LDFLAGS = -m32 -lm $(shell pkg-config --libs sdl) $(shell pkg-config --libs gl) $(shell pkg-config --libs glew)

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
SHADERS = $(wildcard *.glsl)
SHADER_HEADER = shader_code.h
EXECUTABLE = planeshift

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(MAKEFILE_LIST)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

$(SHADER_HEADER): $(SHADERS)
	shader_minifier.exe --preserve-externals -o $@ $^

%.o: %.c %.d $(MAKEFILE_LIST) $(SHADER_HEADER)
	$(CC) -c $(CFLAGS) $< -o $@

%.d: %.c $(SHADER_HEADER)
	@$(SHELL) -ec '$(CC) -M $(CFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

clean:
	rm -rf *.o *.d $(EXECUTABLE) $(SHADER_HEADER)

-include $(SOURCES:.c=.d)

