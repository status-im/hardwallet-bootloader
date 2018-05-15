#ifndef SYS_H_
#define SYS_H_

#include "stm32l4xx.h"

#include <string.h>

// MCU specific constants
#define FLASH_BANK_SIZE 0x80000
#define FLASH_PAGE_SIZE 0x800

#define FLASH_START_BANK1 FLASH_BASE
#define FLASH_START_BANK2 (FLASH_BASE + FLASH_BANK_SIZE)

#define SRAM_START SRAM1_BASE
#define SRAM_END (SRAM_START + SRAM1_SIZE_MAX)

// Utility macros
#define UINT32_PTR(n) ((uint32_t *) n)
#define UINT8_PTR(n) ((uint8_t *) n)

#ifndef MIN
  #define MIN(x, y) \
    ({ typeof (x) __x = (x); \
       typeof (y) __y = (y); \
       __x < __y ? __x : __y; })
#endif
#ifndef MAX
  #define MAX(x, y) \
    ({ typeof (x) __x = (x); \
       typeof (y) __y = (y); \
       __x > __y ? __x : __y; })
#endif

void memzero(uintptr_t start, uintptr_t end);

#endif /* SYS_H_ */
