#ifndef SYS_H_
#define SYS_H_

#include "stm32l4xx.h"

// MCU specific constants
#define FLASH_BANK_SIZE 0x80000
#define FLASH_PAGE_SIZE 0x800

#define FLASH_START_BANK1 FLASH_BASE
#define FLASH_START_BANK2 (FLASH_BASE + FLASH_BANK_SIZE)

// Utility macros
#define UINT32_PTR(n) ((uint32_t *) n)
#define UINT8_PTR(n) ((uint8_t *) n)

#endif /* SYS_H_ */
