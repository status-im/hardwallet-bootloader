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

#include "main.h"
#include "flash.h"
#include "sha256.h"
#include "uECC.h"

// The public keys used to verify the firmware signatures. These are the raw X,Y coordinates. Each key is exactly 64 bytes long and there is no separator.

#ifdef BOOTLOADER_RELEASE
uint8_t fw_public_keys[] = { \
//TODO: generate production keys
};
#else
uint8_t fw_public_keys[] = { \
    0xfe, 0xcb, 0x28, 0xb9, 0x50, 0xdd, 0x8b, 0x2f, 0xc7, 0x34, 0xd3, 0x60, 0x5b, 0x1a, 0xc6, 0xed, 0x02, 0x50, 0xf2, 0x4a, 0xc4, 0x75, 0xd1, 0x28, 0x7f, 0x7c, 0xb5, 0xce, 0x61, 0xd6, 0x95, 0xb9, 0xb5, 0x27, 0x0b, 0x52, 0x77, 0x42, 0x4b, 0xf3, 0xb4, 0x3c, 0xef, 0xcb, 0x56, 0xd1, 0x98, 0x22, 0x11, 0xc2, 0xe5, 0xd3, 0xf0, 0x22, 0x87, 0xb9, 0xe8, 0x20, 0xdc, 0xee, 0x9f, 0xc2, 0xad, 0x22, \
    0xe8, 0xb4, 0x6f, 0xfd, 0xe2, 0x77, 0xe4, 0xb7, 0x8e, 0x64, 0xed, 0x8d, 0x1d, 0xd4, 0xe0, 0x41, 0x72, 0x40, 0xba, 0xc3, 0xc5, 0x25, 0xbe, 0x53, 0xb7, 0x5e, 0xf9, 0xf5, 0x19, 0xda, 0x03, 0xdf, 0xb9, 0x41, 0xeb, 0x63, 0x39, 0xdf, 0xee, 0x47, 0x9f, 0x86, 0xbf, 0x87, 0x8a, 0xcd, 0xf0, 0x3b, 0x1e, 0x7d, 0x85, 0xd2, 0x3d, 0x96, 0xf2, 0x31, 0x1a, 0x49, 0x0a, 0xd6, 0xad, 0xdd, 0x44, 0xf0, \
    0x5c, 0x25, 0xe5, 0x96, 0xa1, 0xc0, 0x17, 0xf2, 0x18, 0x80, 0x3f, 0x99, 0x40, 0xda, 0x02, 0x8a, 0x9a, 0x8c, 0xef, 0x34, 0x60, 0xc9, 0x53, 0x6b, 0x34, 0x07, 0x42, 0x87, 0xce, 0xe7, 0xa1, 0x47, 0x6a, 0x9f, 0xf2, 0x9e, 0xfd, 0xf3, 0xa0, 0x1a, 0xd2, 0x6c, 0xd0, 0x28, 0xc9, 0x4c, 0x21, 0xfb, 0x32, 0xcc, 0x08, 0x56, 0x16, 0xa7, 0x86, 0xcb, 0x36, 0x26, 0x1e, 0x60, 0x58, 0x90, 0x67, 0xae, \
    0x37, 0xa5, 0xfb, 0xbc, 0xe9, 0xa9, 0x62, 0x45, 0x0f, 0x71, 0x7a, 0x91, 0x09, 0xb3, 0xe5, 0xfe, 0x2c, 0x37, 0x94, 0x5e, 0xc0, 0x91, 0xe7, 0x91, 0xd8, 0xc0, 0xe3, 0x84, 0x7e, 0x48, 0xe0, 0xe3, 0x5b, 0xb2, 0xf3, 0xe0, 0xc6, 0x86, 0x37, 0xc9, 0xd3, 0x56, 0x7d, 0x5e, 0xe1, 0xfc, 0x71, 0x1f, 0xf0, 0xfb, 0xa0, 0xe5, 0xf4, 0xc8, 0x8f, 0x40, 0x5d, 0x95, 0x0d, 0xd6, 0x51, 0xd3, 0xb3, 0x13, \
};
#endif
int main(void) {
  protect_flash();

  if (!check_firmware(UPGRADE_FW_START)) {
    upgrade_firmware();
  } else if (check_firmware(FIRMWARE_START)) {
    if(!check_firmware(RECOVERY_FW_START)) {
      factory_reset();
    } else {
      for(;;); // This should never happen, but if the recovery firmware is also corrupted, we enter an endless loop since there is nothing else we can do
    }
  }

  run_firmware();
}

int check_firmware(uintptr_t addr) {
  if(UINT32_PTR(addr)[0] != FW_MAGIC) {
    return 1;
  }

  uint32_t fw_size = UINT32_PTR(addr)[1];

  if (fw_size > FIRMWARE_SIZE) {
    return 1;
  }

  uint8_t hash[CF_SHA256_HASHSZ];
  cf_sha256_context ctx;
  uECC_Curve ec_curve = uECC_secp256k1();

  cf_sha256_init(&ctx);
  cf_sha256_update(&ctx, UINT8_PTR(addr + FIRMWARE_HEADER_SIZE), fw_size);
  cf_sha256_digest(&ctx, hash);

  for(int i = 0; i < SIGNATURE_COUNT; i++) {
    if (uECC_verify((fw_public_keys + (KEY_LENGTH * i)), hash, CF_SHA256_HASHSZ, UINT8_PTR(addr + SIGNATURE_HEADER_OFFSET + (SIGNATURE_LENGTH * i)), ec_curve) != 1) {
      return 1;
    }
  }

  return 0;
}

void protect_flash() {
#ifdef BOOTLOADER_RELEASE
  if (FLASH_IS_RDP2()) {
    return;
  }

  flash_optunlock();

  FLASH_SET_RDP2();
  FLASH_WP(FLASH->WRP1AR, BOOTLOADER_FIRST_PAGE, (FIRMWARE_FIRST_PAGE - 1));
  FLASH_WP(FLASH->WRP1BR, RECOVERY_FW_FIRST_PAGE, (RECOVERY_FW_FIRST_PAGE + FIRMWARE_PAGE_COUNT - 1));

  // This resets the MCU
  flash_optprogram();
#endif
}


void _load_firmware(uintptr_t newfw, uint8_t newfw_bank, uint8_t newfw_page, uint8_t remove_newfw) {
  flash_unlock();

  SET_BIT(RCC->CR, RCC_CR_HSION);

  do {
    flash_erase(FLASH_BANK1, FIRMWARE_FIRST_PAGE, FIRMWARE_PAGE_COUNT);
    flash_copy(UINT32_PTR(newfw),  UINT32_PTR(FIRMWARE_START), UINT32_PTR(newfw)[1]);
  } while(check_firmware(FIRMWARE_START) != 0);

  if (remove_newfw) {
    flash_erase(newfw_bank, newfw_page, FIRMWARE_PAGE_COUNT);
  }

  CLEAR_BIT(RCC->CR, RCC_CR_HSION);

  flash_lock();
}

void upgrade_firmware() {
  _load_firmware(UPGRADE_FW_START, FLASH_BANK2, UPGRADE_FW_FIRST_PAGE, 1);
}

void factory_reset(void) {
  _load_firmware(RECOVERY_FW_START, FLASH_BANK1, RECOVERY_FW_FIRST_PAGE, 0);
}

void run_firmware(void) {
  memzero(SRAM_START, SRAM_END);
  uint32_t* fw_entry = UINT32_PTR(FIRMWARE_CODE_START);
  SCB->VTOR = (uint32_t) fw_entry;
  __set_MSP(fw_entry[0]);
  ((void (*)(void))fw_entry[1])();
}

