/* The link map is always the second entry of the GOT */
PROVIDE (_link_map = LOADADDR(.got.plt) + 4);

/* Well... this is reliable as long as libc is the only lib we're linking to directly */
PROVIDE (_libc_filename = LOADADDR(.dynstr) + 1);
