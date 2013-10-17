#!/usr/bin/env python

import sys

CLEAR_BYTES = [
    {'offset':  4, 'length': 1},  # e_ident[EI_CLASS]
    {'offset':  5, 'length': 1},  # e_ident[EI_DATA]
    {'offset':  6, 'length': 1},  # e_ident[EI_VERSION]
    {'offset':  7, 'length': 1},  # e_ident[EI_OSABI]
    {'offset':  8, 'length': 1},  # e_ident[EI_ABIVERSION]
    {'offset':  9, 'length': 7},  # unused
    {'offset': 20, 'length': 4},  # e_version
    {'offset': 32, 'length': 4},  # e_shoff
    {'offset': 36, 'length': 4},  # e_flags
    {'offset': 40, 'length': 2},  # e_ehsize
    {'offset': 46, 'length': 2},  # e_shentsize
    {'offset': 48, 'length': 2},  # e_shnum
    {'offset': 50, 'length': 2},  # e_shstrndx
]

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print 'usage: {} FILENAME'.format(sys.argv[0])
        sys.exit(1)

    with open(sys.argv[1], 'r+b') as f:
        for byte in CLEAR_BYTES:
            f.seek(byte['offset'])
            f.write(byte['length'] * chr(0))
