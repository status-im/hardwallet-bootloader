#!/usr/bin/env python

import ecdsa
import hashlib
import binascii
import re

def main():
    sk = ecdsa.SigningKey.generate(curve=ecdsa.curves.SECP256k1, hashfunc=hashlib.sha256)
    sk_pem = sk.to_pem()
    vk = sk.get_verifying_key()

    vk_hex = binascii.hexlify(vk.to_string())
    vk_h = re.sub(r"(..)", "0x\\1, ", vk_hex) + "\\"
    open("secret_key.pem","w").write(sk.to_pem())
    open("public_key.h","w").write(vk_h)
if __name__ == '__main__':
    main()