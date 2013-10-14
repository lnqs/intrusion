#!/usr/bin/env python

import sys


def gnu_hash(s):
    h = 5381
    for c in s:
        h = h * 33 + ord(c)
    return h & 0xffffffff

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print 'usage: {} SYMBOL_NAME'.format(sys.argv[0])
        sys.exit(1)

    print '0x{:08x}'.format(gnu_hash(sys.argv[1]))
