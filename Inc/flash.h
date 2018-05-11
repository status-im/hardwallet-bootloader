#ifndef FLASH_H_
#define FLASH_H_

#include "sys.h"

// The values below come from the ST DM00083560 document. They might need to be changed if the MCU is swapped for a different part
#define FLASH_KEYR1 0x45670123
#define FLASH_KEYR2 0xCDEF89AB

#define FLASH_BANK1 0
#define FLASH_BANK2 1

/**
 * Unlocks the flash, allowing writing.
 */
int flash_unlock();

/**
 * Erases `page_count` pages, starting from `page` in the given `bank`. Since this function is only called using constant values, the input parameters are not validated.
 */
int flash_erase(uint8_t bank, uint8_t page, uint8_t page_count);

/**
 * Copies `size` bytes from `src` to `dst`. Since this function is only called using constant values and previously checked values, the input parameters are not validated. The addresses must be aligned according
 * to the rules specified in the ST DM00083560 document.
 */
int flash_copy(uint32_t* src, __IO uint32_t* dst, uint32_t size);

/**
 * Locks the flash again, preventing writing.
 */
int flash_lock();

#endif /* FLASH_H_ */
