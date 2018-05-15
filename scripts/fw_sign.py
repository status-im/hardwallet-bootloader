#!/usr/bin/env python

import ecdsa
import hashlib
import sys

SIG_LEN = 64
SIG_OFF = 8
HEADER_SIZE = 512

def main(index):
    fw = open("fw.bin", "rb").read()
    
    if index < 0 or index > 6:
        print("Index must be between 0 and 6")
        exit(1)
    
    if fw[:4] != b'SHWF':
        print("Firmware must be prepared with fw_prepare.py first!")
        exit(1)
        
    sk_pem = open("secret_key.pem", "r").read()
    sk = ecdsa.SigningKey.from_pem(sk_pem)
    plain_fw = fw[HEADER_SIZE:]
    fw_sig = sk.sign_deterministic(plain_fw, hashfunc=hashlib.sha256)
    fw = fw[:SIG_OFF + SIG_LEN * (index) ] + fw_sig + fw[SIG_OFF + SIG_LEN * (index + 1):]
    open("fw.bin", "wb").write(fw)
if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("You must insert the (zero-based) index of the signature")
        exit(1)
    main(int(sys.argv[1]))