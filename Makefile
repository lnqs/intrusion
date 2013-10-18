CC = clang
LD = ld
NASM = nasm
STRIP = sstrip -z
SCREW_ELF_HEADER = ./screw_elf_header.py
SHADER_MINIFIER = shader_minifier.exe
CFLAGS = -m32 -std=c99 -Wall -Werror -Os -ffast-math -fomit-frame-pointer -march=i686 $(shell pkg-config --cflags sdl) $(shell pkg-config --cflags gl)
LDFLAGS = -melf_i386 -dynamic-linker /lib/ld-linux.so.2 -lc
NASMFLAGS = -f elf

SOURCES = $(wildcard *.c)
ASM_SOURCES = $(wildcard *.asm)
LINKER_SCRIPT = linker.ld
OBJECTS = $(SOURCES:.c=.o) $(ASM_SOURCES:.asm=.o)
SHADERS = $(wildcard *.glsl)
SHADER_HEADER = shader_code.h
EXECUTABLE = planeshift.elf
COMPRESSED = planeshift

all: $(COMPRESSED)

$(COMPRESSED): $(EXECUTABLE)
	echo '#!/bin/sh\ndd if="$$0" bs=1 skip=69|unxz>p;chmod +x p;./p;rm -f p;exit' > $@
	xz -c5 --format=lzma $< >> $@
	chmod +x $@

$(EXECUTABLE): $(LINKER_SCRIPT) $(OBJECTS) $(MAKEFILE_LIST)
	$(LD) $(LDFLAGS) -o $@ -dT $(LINKER_SCRIPT) $(OBJECTS)
	$(STRIP) $@
	$(SCREW_ELF_HEADER) $@

$(SHADER_HEADER): $(SHADERS)
	$(SHADER_MINIFIER) --preserve-externals -o $@ $^

%.o: %.asm
	$(NASM) $(NASMFLAGS) $< -o $@

%.o: %.c %.d $(SHADER_HEADER)
	$(CC) -c $(CFLAGS) $< -o $@

%.d: %.c $(SHADER_HEADER)
	@$(SHELL) -ec '$(CC) -M $(CFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

clean:
	rm -rf *.o *.d $(EXECUTABLE) $(COMPRESSED) $(SHADER_HEADER)

-include $(SOURCES:.c=.d)

