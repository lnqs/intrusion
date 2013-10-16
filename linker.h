#ifndef LINKER_H
#define LINKER_H

#include "clib.h"
#include <elf.h>
#include <link.h>
#include <string.h>

extern const struct link_map* _link_map; // provided by link_map.ld linker-script

static const char* libc_filename = "libc.so.6";
static const uint32_t libc_dlopen_mode_hash = 0xf2cb98a2;

// This is the hash-function used for gnu-style hash-tables in elf.
// While it doesn't really matter what function we use -- we don't use the
// hash-tables at all -- it should be fine
static stdcall uint32_t gnu_hash(const char* s)
{
    uint32_t h = 5381;

    for (; *s != '\0'; s++)
    {
        h = h * 33 + *s;
    }

    return h & 0xffffffff;
}

static stdcall const struct link_map* link_map_entry_for_library(const char* library)
{
    // assume the entry exists to save some bytes
    const struct link_map* map = _link_map;
    // At least one call directly to a shared library is needed, otherwise the
    // linker omits the sections for dynamic linking completly.
    // This strstr() call is this one!
    while (strstr(map->l_name, library) == NULL)
    {
        map = map->l_next;
    }

    return map;
}

static stdcall const void* get_table(const struct link_map* map, int type)
{
    const Elf32_Dyn* dyn = (const Elf32_Dyn*)map->l_ld;
    while (dyn->d_tag != type)
    {
        dyn += 1;
    }

    return (void*)dyn->d_un.d_ptr;
}

// Gnah. After implementing symbol-lookups via gnu hash-table, I realized that
// there're still libraries out using the old sysv-style hash-tables.
// NVidias GL implementation is the one that made problems on my system.
// Therefore, to avoid the size-overhead of implementing both, we keep it way
// simpler here and just walk the symbol table to find symbols.
static stdcall void* resolve_symbol(const char* library, uint32_t hash)
{
    // To keep the code short, we trust in the library to be loaded and finding
    // the symbol.
    // If this isn't the case, it'll behave undefined and segfault eventually.

    const struct link_map* map = link_map_entry_for_library(library);
    const Elf32_Sym* symtab = get_table(map, DT_SYMTAB);
    const char* strtab = get_table(map, DT_STRTAB);

    while (hash != gnu_hash(strtab + symtab->st_name))
    {
        symtab += 1;
    }

    return (void*)(map->l_addr + symtab->st_value);
}

static stdcall void load_library(const char* filename)
{
    // Yay! Using internals of libc! Let's just cross fingers this won't change too fast :/
    void* (*__libc_dlopen_mode_fn)(const char*, int) = resolve_symbol(libc_filename, libc_dlopen_mode_hash);
    __libc_dlopen_mode_fn(filename, RTLD_NOW | RTLD_GLOBAL);
}

#endif

