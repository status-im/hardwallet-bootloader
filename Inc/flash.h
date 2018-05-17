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

#ifndef FLASH_H_
#define FLASH_H_

#include "sys.h"

// The values below come from the ST DM00083560 document. They might need to be changed if the MCU is swapped for a different part
#define FLASH_KEYR1 0x45670123
#define FLASH_KEYR2 0xCDEF89AB

#define FLASH_OPTKEYR1 0x08192A3B
#define FLASH_OPTKEYR2 0x4C5D6E7F

#define FLASH_BANK1 0
#define FLASH_BANK2 1

#define RDP2 0xcc

#define FLASH_IS_RDP2() ((FLASH->OPTR & 0xff) == RDP2)
#define FLASH_SET_RDP2() FLASH->OPTR |= RDP2
#define FLASH_WP(reg, start, end) reg = (0xFF00FF00 | end << 16 | start)


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

/**
 * Unlock the option bytes
 */
int flash_optunlock();

/**
 * Write and reload the option bytes
 */
void flash_optprogram();


#endif /* FLASH_H_ */
