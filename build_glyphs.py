#!/usr/bin/env python

# This script takes an image file (anything PIL can read) and reads chunks of
# 4 * 8 pixels from it, treating these as rasterized letters.
# It expects the image to cover the ASCII-characters from 32 till 126.
# Then, each letter it finds in the source, that is used in a string included
# in _(), is encoded as bitmap in an int, and all of them are written
# to stdout, in form of an array in a C-headerfile.

import sys
from sets import Set
from PIL import Image

SCALE_X = 4
SCALE_Y = 8
GLYPH_CTYPE = 'uint32_t'  # sizeof() has to match SCALE_X * SCALE_Y
START_ASCII_CODE = 32
END_ASCII_CODE = 126


def get_glyphs(filename):
    image = Image.open(filename).convert('L')
    for i in range(END_ASCII_CODE + 1 - START_ASCII_CODE):
        crop = image.crop(
            (i * SCALE_X, 0, (i + 1) * SCALE_X, SCALE_Y)).tobytes()
        yield chr(i + START_ASCII_CODE), crop


def encode_image(image):
    glyph = 0
    for i in xrange(SCALE_X * SCALE_Y):
        if image[i] != chr(0xff):
            glyph |= 0x1 << i
    return glyph


def get_used_letters(source_files):
    used_letters = Set()

    for filename in source_files:
        with open(filename) as file_:
            content = file_.read()

        in_identifier = False
        in_string = False
        skip_next = False
        for i in xrange(len(content)):
            if skip_next:
                skip_next = False
            elif not in_identifier and content[i] == '_' \
                    and len(content) >= i + 1 and content[i + 1] == '(':
                in_identifier = True
                skip_next = True
            elif in_identifier:
                if not in_string and content[i] == ')':
                    in_identifier = False
                elif in_string and content[i] == '\\':
                    skip_next = True
                elif content[i] == '"':
                    in_string = not in_string
                elif in_string:
                    used_letters.add(content[i])

    return used_letters


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print 'usage: {} IMAGE_FILE SOURCE_FILE [SOURCE_FILE ...]' \
            .format(sys.argv[0])
        sys.exit(1)

    image_file = sys.argv[1]
    source_files = sys.argv[2:]

    used_letters = get_used_letters(source_files)
    glyphs = {c: encode_image(i)
              if c in used_letters else 0x0
              for (c, i) in get_glyphs(image_file)}

    # Multiline comments, templates, etc., I know. I'm not in the mood :o)
    print '#ifndef GLYPHS_H'
    print '#define GLYPHS_H'
    print
    print 'typedef {} glyph_t;'.format(GLYPH_CTYPE)
    print
    print 'static const size_t glyphs_ascii_begin = {};' \
        .format(START_ASCII_CODE)
    print 'static const size_t glyphs_ascii_end = {};'.format(END_ASCII_CODE)
    print 'static const size_t glyph_width = {};'.format(SCALE_X)
    print 'static const size_t glyph_height = {};'.format(SCALE_Y)
    print
    print 'static const glyph_t glyphs[] = {'
    print '\n'.join(
        ['    0x{:08x}, // {}'.format(g,
                                      l if l != '\\' else 'backslash')
         for (l, g) in sorted(glyphs.iteritems())])
    print '};'
    print
    print '#endif'
    print
