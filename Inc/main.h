#ifndef __MAIN_H__
/**
 * This file is part of the Status project, https://status.im/
 *
 * Copyright (c) 2018 Status Research & Development GmbH
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#define __MAIN_H__

#include "sys.h"

// Firmware magic number. Reads as "SHWF" in an hex editor
#define FW_MAGIC 0x46574853

// The constants below define the memory layout documented in the README.md file
#define BOOTLOADER_SIZE 0x2000
#define BOOTLOADER_FIRST_PAGE 0

#define FIRMWARE_SIZE ((FLASH_BANK_SIZE - BOOTLOADER_SIZE) / 2)
#define FIRMWARE_HEADER_SIZE 0x200
#define FIRMWARE_FIRST_PAGE (BOOTLOADER_SIZE / FLASH_PAGE_SIZE)
#define FIRMWARE_PAGE_COUNT (FIRMWARE_SIZE / FLASH_PAGE_SIZE)

#define FIRMWARE_START (FLASH_START_BANK1 + BOOTLOADER_SIZE)
#define FIRMWARE_CODE_START (FIRMWARE_START + FIRMWARE_HEADER_SIZE)

#define UPGRADE_FW_START FLASH_START_BANK2
#define UPGRADE_FW_FIRST_PAGE 0

#define RECOVERY_FW_START (FIRMWARE_START + FIRMWARE_SIZE)
#define RECOVERY_FW_FIRST_PAGE (FIRMWARE_FIRST_PAGE + FIRMWARE_PAGE_COUNT)

#define SIGNATURE_HEADER_OFFSET 8
#define SIGNATURE_LENGTH 64
#define KEY_LENGTH SIGNATURE_LENGTH
#define SIGNATURE_COUNT 4


/**
 * Checks if the bootloader and recovery are write protected, if not protect them. Also enables the Read Protection level 2. Since this is irreversible, the code is disabled during development
 */
void protect_flash();

/**
 * Checks the firmware a the given address. It checks the magic number, validates the size and checks the signature(s).
 */
int check_firmware(uintptr_t addr);

/**
 * Performs a firmware upgrade. It copies the firmware from the upgrade area to the firmware area. Must only be called if a previous call to check_firmware has succeeded.
 */
void upgrade_firmware(void);

/**
 * Performs a factory firmware restore. It copies the firmware from the recovery area to the firmware area. Must only be called if both the firmware and the upgrade are have no valid firmware.
 */
void factory_reset(void);

/**
 * Runs the currently installed firmware.
 */
void run_firmware();

#endif /* __MAIN_H__ */
