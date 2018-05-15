#!/usr/bin/env bash

case "$1" in
"firmware")
    address="0x8002000"
    ;;
"recovery")
    address="0x8041000"
    ;;
"upgrade")
    address="0x8080000"
    ;;    
*)
	echo "You must tell which part of memory you want to load. Possible values are firmware, recovery and upgrade"
	exit 1
    ;;
esac

st-flash write fw.bin $address