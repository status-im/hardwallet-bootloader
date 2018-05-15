# Hardware Wallet Pro bootloader

This document describes the bootloader, the memory layout and upgrade mechanism of the Hardware Wallet Pro.

## Memory layout

    |===================================| (Bank 1)
    |          BOOTLOADER (WP)          |
    |-----------------------------------|
    |                                   |
    |                                   |
    |             FIRMWARE              |
    |                                   |
    |                                   |
    |-----------------------------------|
    |                                   |
    |                                   |
    |       FACTORY FIRMWARE (WP)       |
    |                                   |
    |                                   |
    |===================================| (Bank 2)
    |                                   |
    |                                   |
    |        UPGRADED FIRMWARE          |
    |                                   |
    |                                   |
    |-----------------------------------|
    |                                   |
    |                                   |
    |                                   |
    |            USER DATA              |
    |                                   |
    |                                   |
    |===================================|
    
The STM32 chips divide flash memory into two banks. Reading one bank while writing on the other is possible, so the user data and the firmware will live in two different banks. The page size is 2k, so all boundaries will be aligned to a multiple of that size. The area allocated for the firmware is (BANK_SIZE - BOOTLOADER_SIZE) / 2, since two instances of the firmware will need to fit in the first bank. The user data area size will be FW_SIZE + BOOTLOADER_SIZE, as to allocate the entire memory. Absolute sizes cannot be provided yet. When we have a final bootloader (estimated to be in the 4-8k range) and a final firmware we will decide how much flash memory on chip we need and with this finally calculate the absolute size. The chip must be chosen by keeping in mind that the firmware might grow by up to 50% in size (although that is unlikely) with future upgrades.

The bootloader is placed at the very beginning of the flash memory, in a write protected area. The bootloader will not be upgradable, so it must be kept as small and simple as possible and be throughly tested and reviewed. The tasks it will perform are:

1. Check if the UPGRADED FIRMWARE area has a valid firmware. If so, start firmware upgrade, else go to 2
2. Check if the FIRMWARE area has a valid firmware. If so, start it, else go to 3
3. Restore the FACTORY FIRMWARE (which is also written in a write protected area)

This design makes it impossible to brick the device by a failed firmware upgrade, because it can always start over
from the initial one. *(should it be possible to eventually upgrade the recovery firmware?)*

The bootloader is not responsible for transferring the new firmware to the UPGRADED FIRMWARE area. The (upgradable) firmware is responsible for that. The reason for this is again that we want to keep the bootloader as small as possible and the device won't have any interface except for BLE (which is not a really simple one). The chip will be set in protection level 2 which makes it impossible to interact with the flash memory externally (using ST-LINK or similar) and also makes it impossible to disable write protection of the bootloader and other write protected areas. Activating protection level 2 on STM32 chips is not reversible.

## Firmware

Each firmware section has a header preceding the actual executable. The header is composed of a magic number, the byte size of the code section and an ECDSA-256 signature, which must be verified by the bootloader on each boot and before performing an upgrade. The size of the header will be fixed. *(Since the current chip requires the Interrupt Vector to be placed at an address which is a multiple of 512, we will have to make the header that long. Any idea about useful metadata to place there? Maybe multiple signatures?)*. The size of the code section is variable, but must fit within the limits of the allocated memory.

The linker definition file of the firmware must set the flash start address at fixed location, accounting for the space occupied by the bootloader and its own header. The exact address will be documented here as soon as it is defined.

## Booting

The bootloader checks the integrity of the firmware by verifying its signatures. It substitutes its Interrupt Vector pointer with the one defined by the firmware by setting the corresponding registry. It clears the entire content of the SRAM1, then restores the stack pointer and jumps to the reset handler of the firmware.

## Upgrade procedure

After determining that there is a firmware to be flashed, the bootloader check the validity of its signature. No unsigned firmware will be loaded. Loading begins by erasing the current firmware. The entire FIRMWARE area is erased, regardless of the actual firmware size. Then, the new firmware is copied over from the UPGRADED FIRMWARE area. The integrity of the copied firmware is verified once again by verifying its signature. At this point upgraded firmware is erased from memory completely. This procedure makes it possible to recover from an interrupted upgrade (for example, low battery), since the bootloader will try again on reset.

## Recovery procedure

It works exactly the same as the upgrade procedure, but the firmware is not deleted at the end.

## Signature key(s)

The bootloader will have an EC public key which will be used to verify the firmware on boot and upgrade. The private part of this key must be kept secret and will be used to sign the released firmware. *(Since the key will not be replaceable, we might increase security by mandating that the firmware is signed with 2 or more keys held by different people/entities. In this way, the leak of a single key is not enough to create a valid firmware.)*


