/* The link map is always the second entry of the GOT */
PROVIDE (_link_map = LOADADDR(.got.plt) + 4);

/* Well... this is reliable as long as libc is the only lib we're linking to directly */
PROVIDE (_libc_filename = LOADADDR(.dynstr) + 1);

/* Yay! screw_elf_header.py writes us strings to the elf-header, define symbols for them here! */
PROVIDE (_gl_library = __executable_start + 46);
