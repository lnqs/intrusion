CC = clang
LD = ld
NASM = nasm
STRIP = sstrip -z
SCREW_ELF_HEADER = ./screw_elf_header.py
SHADER_MINIFIER = shader_minifier.exe
CFLAGS = -m32 -std=c99 -Wall -Werror -ggdb -Oz -ffast-math -fomit-frame-pointer -march=i686 $(shell pkg-config --cflags sdl) $(shell pkg-config --cflags gl)
LDFLAGS = -melf_i386 -dynamic-linker /lib/ld-linux.so.2 -lc
NASMFLAGS = -f elf

SOURCES = $(wildcard *.c)
ASM_SOURCES = $(wildcard *.asm)
LINKER_SCRIPTS = $(wildcard *.ld)
OBJECTS = $(SOURCES:.c=.o) $(ASM_SOURCES:.asm=.o)
SHADERS = $(wildcard *.glsl)
SHADER_HEADER = shader_code.h
EXECUTABLE = planeshift.elf
COMPRESSED = planeshift

all: $(SOURCES) $(COMPRESSED)

$(COMPRESSED): $(EXECUTABLE)
	echo '#!/bin/sh\nP=/tmp/p;dd if="$$0" bs=1 skip=71|unxz>$$P;chmod +x $$P;$$P;exit' > $@
	xz -c9 --format=lzma $< >> $@
	chmod +x $@

$(EXECUTABLE): $(LINKER_SCRIPTS) $(OBJECTS) $(MAKEFILE_LIST)
	$(LD) $(LDFLAGS) -o $@ $(LINKER_SCRIPTS) $(OBJECTS)
	$(STRIP) $@
	$(SCREW_ELF_HEADER) $@

$(SHADER_HEADER): $(SHADERS)
	$(SHADER_MINIFIER) --preserve-externals -o $@ $^

%.o: %.asm $(MAKEFILE_LIST)
	$(NASM) $(NASMFLAGS) $< -o $@

%.o: %.c %.d $(MAKEFILE_LIST) $(SHADER_HEADER)
	$(CC) -c $(CFLAGS) $< -o $@

%.d: %.c $(SHADER_HEADER)
	@$(SHELL) -ec '$(CC) -M $(CFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

clean:
	rm -rf *.o *.d $(EXECUTABLE) $(COMPRESSED) $(SHADER_HEADER)

-include $(SOURCES:.c=.d)

