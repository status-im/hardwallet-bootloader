#include "main.h"
#include "flash.h"
#include "sha256.h"

int main(void) {
  if (!check_firmware(UPGRADE_FW_START)) {
    upgrade_firmware();
  } else if (check_firmware(FIRMWARE_START)) {
    factory_reset();
  }

  run_firmware();
}

int check_firmware(uintptr_t addr) {
  if(UINT32_PTR(addr)[0] != FW_MAGIC) {
    return 1;
  }

  cf_sha256_context ctx;
  cf_sha256_init(&ctx);
  cf_sha256_update(&ctx, UINT8_PTR(addr), UINT32_PTR(addr)[1]);
  uint8_t hash[CF_SHA256_HASHSZ];
  cf_sha256_digest(&ctx, hash);
  //TODO: verify signature!!!

  return 0;
}


void _load_firmware(uintptr_t newfw, uint8_t newfw_bank, uint8_t newfw_page) {
  flash_unlock();

  do {
    flash_erase(FLASH_BANK1, FIRMWARE_FIRST_PAGE, FIRMWARE_PAGE_COUNT);
    flash_copy(UINT32_PTR(newfw),  UINT32_PTR(FIRMWARE_START), UINT32_PTR(newfw)[1]);
  } while(check_firmware(FIRMWARE_START) != 0);

  flash_erase(newfw_bank, newfw_page, FIRMWARE_PAGE_COUNT);
  flash_lock();
}

void upgrade_firmware() {
  _load_firmware(UPGRADE_FW_START, FLASH_BANK2, UPGRADE_FW_FIRST_PAGE);
}

void run_firmware(void) {
  uint32_t* fw_entry = UINT32_PTR(FIRMWARE_CODE_START);
  SCB->VTOR = (uint32_t) fw_entry;
  __set_MSP(fw_entry[0]);
  ((void (*)(void))fw_entry[1])();
}

void factory_reset(void) {
  _load_firmware(RECOVERY_FW_START, FLASH_BANK1, RECOVERY_FW_FIRST_PAGE);
}
