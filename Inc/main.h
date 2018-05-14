#ifndef __MAIN_H__
#define __MAIN_H__

#include "sys.h"

// Firmware magic number. Reads as "SHWF" in an hex editor
#define FW_MAGIC 0x46574853

// The constants below define the memory layout documented in the README.md file
#define BOOTLOADER_SIZE 0x2000
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
