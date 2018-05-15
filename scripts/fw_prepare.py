#!/usr/bin/env python

import struct

SIGNATURE_AREA_SIZE = 512 - 8

def main():
    fw = open("fw.bin", "rb").read()
    
    if fw[:4] == b'SHWF':
        print("Firmware already prepared")
        exit(1)
    
    header = b'SHWF'
    header += struct.pack('<I', len(fw))
    header += b'\x00' * SIGNATURE_AREA_SIZE
    open("fw.bin", "wb").write(header + fw)
if __name__ == '__main__':
    main()