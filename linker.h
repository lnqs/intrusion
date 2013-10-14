#ifndef LINKER_H
#define LINKER_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <elf.h>
#include <link.h>
#include <string.h>
#include <stdlib.h>

extern const void __executable_start; // provided by the ld's default linker-script

// function the linker uses for the hashes in the GNU_HASH section
static stdcall uint32_t gnu_hash(const char* s)
{
    uint32_t h = 5381;
    for (unsigned char c = *s; c != '\0'; c = *++s)
    {
        h = h * 33 + c;
    }

    return h & 0xffffffff;
}

static stdcall const struct link_map* get_link_map()
{
    // The symbol itself marks the the start of the executable, it's not a pointer
    const Elf32_Ehdr* ehdr = &__executable_start;
    const Elf32_Phdr* phdr = &__executable_start + ehdr->e_phoff;

    for (; phdr->p_type != PT_DYNAMIC; phdr++);

    const Elf32_Dyn* dyn = (const Elf32_Dyn*)phdr->p_vaddr;
    for (; dyn->d_tag != DT_PLTGOT; dyn++);

    // Yay! We got the global offset table!
    void** got = (void*)dyn->d_un.d_ptr;

    // get the second GOT-entry, it's a pointer to the link map
    return got[1];
}

static stdcall const struct link_map* link_map_entry_for_library(const char* library)
{
    for (const struct link_map* map = get_link_map(); map != NULL; map = map->l_next)
    {
        if (map->l_name && strcmp(basename(map->l_name), basename(library)) == 0)
        {
            return map;
        }
    }

    return NULL;
}

static stdcall const void* get_table(const struct link_map* map, int type)
{
    for (const Elf32_Dyn* dyn = (const Elf32_Dyn*)map->l_ld; dyn->d_tag != DT_NULL; dyn++)
    {
        if (dyn->d_tag == type)
        {
            return (void*)dyn->d_un.d_ptr;
        }
    }

    return NULL;
}

static stdcall void* resolve_symbol(const char* library, const char* symbol)
{
    // To keep the code short, we trust in the library to be loaded and finding
    // the symbol.
    // If this isn't the case, it'll behave undefined and segfault eventually.

    const struct link_map* map = link_map_entry_for_library(library);

    const uint32_t* hashtab = get_table(map, DT_GNU_HASH);
    const Elf32_Sym* symtab = get_table(map, DT_SYMTAB);
    const char* strtab = get_table(map, DT_STRTAB);

    uint32_t nbuckets = hashtab[0];
    uint32_t symndx = hashtab[1];
    uint32_t maskwords = hashtab[2];
    // index 3 is used for a 'fast-reject-filter' we just ignore
    const uint32_t* buckets = &hashtab[4 + maskwords];
    const uint32_t* values = &hashtab[4 + maskwords + nbuckets];

    uint32_t hash = gnu_hash(symbol);

    uint32_t chain = buckets[hash % nbuckets];
    const Elf32_Sym* sym = &symtab[chain];
    const uint32_t* chain_ptr = &values[chain - symndx];
    while (1)
    {
        if ((hash & ~1) == (*chain_ptr & ~1) && strcmp(symbol, strtab + sym->st_name) == 0)
        {
            return (void*)(map->l_addr + sym->st_value);
        }

        sym += 1;
        chain_ptr += 1;
    }
}

static stdcall void load_library(const char* filename)
{
    // Yay! Using internals of libc! Let's just cross fingers this won't change too fast :/
    void* (*__libc_dlopen_mode)(const char*, int) = resolve_symbol("libc.so.6", "__libc_dlopen_mode");
    __libc_dlopen_mode(filename, RTLD_NOW | RTLD_GLOBAL);
}

#endif

