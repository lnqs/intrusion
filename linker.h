#ifndef LINKER_H
#define LINKER_H

#include "clib.h"
#include <elf.h>
#include <link.h>
#include <string.h>

extern const struct link_map* _link_map; // provided by link_map.ld linker-script

static const char* libc_filename = "libc.so.6";
static const uint32_t libc_dlopen_mode_hash = 0xf2cb98a2;

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
    // assume the entry exists to save some bytes
    const Elf32_Dyn* dyn = (const Elf32_Dyn*)map->l_ld;
    while (dyn->d_tag != type)
    {
        dyn += 1;
    }

    return (void*)dyn->d_un.d_ptr;
}

// This function only works for libraries using gnu-style hash-tables.
// While the sysv-style table makes lookups fast, it is impossible to find
// the requested symbol without its name.
static stdcall void* resolve_symbol(const char* library, uint32_t hash)
{
    // To keep the code short, we trust in the library to be loaded and finding
    // the symbol.
    // If this isn't the case, it'll behave undefined and segfault eventually.

    const struct link_map* map = link_map_entry_for_library(library);

    const uint32_t* hashtab = get_table(map, DT_GNU_HASH);
    const Elf32_Sym* symtab = get_table(map, DT_SYMTAB);

    uint32_t nbuckets = hashtab[0];
    uint32_t symndx = hashtab[1];
    uint32_t maskwords = hashtab[2];
    // index 3 is used for a 'fast-reject-filter' we just ignore
    const uint32_t* buckets = &hashtab[4 + maskwords];
    const uint32_t* values = &hashtab[4 + maskwords + nbuckets];

    uint32_t chain = buckets[hash % nbuckets];
    const Elf32_Sym* sym = &symtab[chain];
    const uint32_t* chain_ptr = &values[chain - symndx];

    // Yep, if we have a hash-collision we've got a problem.
    // Last bit is used as 'stop-bit' to find the end of the chain, so ignore it
    // when comparing
    while ((hash & ~1) != (*chain_ptr & ~1))
    {
        sym += 1;
        chain_ptr += 1;
    }
    return (void*)(map->l_addr + sym->st_value);
}

static stdcall void load_library(const char* filename)
{
    // Yay! Using internals of libc! Let's just cross fingers this won't change too fast :/
    // 0xf2cb98a2 is the hash of __libc_dlopen_mode.
    void* (*__libc_dlopen_mode_fn)(const char*, int) = resolve_symbol(libc_filename, libc_dlopen_mode_hash);
    __libc_dlopen_mode_fn(filename, RTLD_NOW | RTLD_GLOBAL);
}

#endif

