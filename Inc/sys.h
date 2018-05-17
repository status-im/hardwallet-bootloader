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
