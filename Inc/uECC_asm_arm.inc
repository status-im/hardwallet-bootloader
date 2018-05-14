/* Copyright 2015, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#ifndef _UECC_ASM_ARM_H_
#define _UECC_ASM_ARM_H_

#if (uECC_SUPPORTS_secp256r1 || uECC_SUPPORTS_secp256k1)
    #define uECC_MIN_WORDS 8
#endif
#if uECC_SUPPORTS_secp224r1
    #undef uECC_MIN_WORDS
    #define uECC_MIN_WORDS 7
#endif
#if uECC_SUPPORTS_secp192r1
    #undef uECC_MIN_WORDS
    #define uECC_MIN_WORDS 6
#endif
#if uECC_SUPPORTS_secp160r1
    #undef uECC_MIN_WORDS
    #define uECC_MIN_WORDS 5
#endif

#if (uECC_PLATFORM == uECC_arm_thumb)
    #define REG_RW "+l"
    #define REG_WRITE "=l"
#else
    #define REG_RW "+r"
    #define REG_WRITE "=r"
#endif

#if (uECC_PLATFORM == uECC_arm_thumb || uECC_PLATFORM == uECC_arm_thumb2)
    #define REG_RW_LO "+l"
    #define REG_WRITE_LO "=l"
#else
    #define REG_RW_LO "+r"
    #define REG_WRITE_LO "=r"
#endif

#if (uECC_PLATFORM == uECC_arm_thumb2)
    #define RESUME_SYNTAX
#else
    #define RESUME_SYNTAX ".syntax divided \n\t"
#endif

#if (uECC_OPTIMIZATION_LEVEL >= 2)

uECC_VLI_API uECC_word_t uECC_vli_add(uECC_word_t *result,
                                      const uECC_word_t *left,
                                      const uECC_word_t *right,
                                      wordcount_t num_words) {
#if (uECC_MAX_WORDS != uECC_MIN_WORDS)
  #if (uECC_PLATFORM == uECC_arm_thumb) || (uECC_PLATFORM == uECC_arm_thumb2)
    uint32_t jump = (uECC_MAX_WORDS - num_words) * 4 * 2 + 1;
  #else /* ARM */
    uint32_t jump = (uECC_MAX_WORDS - num_words) * 4 * 4;
  #endif
#endif
    uint32_t carry;
    uint32_t left_word;
    uint32_t right_word;
    
    __asm__ volatile (
        ".syntax unified \n\t"
        "movs %[carry], #0 \n\t"
    #if (uECC_MAX_WORDS != uECC_MIN_WORDS)
        "adr %[left], 1f \n\t"
        ".align 4 \n\t"
        "adds %[jump], %[left] \n\t"
    #endif
        
        "ldmia %[lptr]!, {%[left]} \n\t"
        "ldmia %[rptr]!, {%[right]} \n\t"
        "adds %[left], %[right] \n\t"
        "stmia %[dptr]!, {%[left]} \n\t"
        
    #if (uECC_MAX_WORDS != uECC_MIN_WORDS)
        "bx %[jump] \n\t"
    #endif
        "1: \n\t"
        REPEAT(DEC(uECC_MAX_WORDS),
            "ldmia %[lptr]!, {%[left]} \n\t"
            "ldmia %[rptr]!, {%[right]} \n\t"
            "adcs %[left], %[right] \n\t"
            "stmia %[dptr]!, {%[left]} \n\t")
        
        "adcs %[carry], %[carry] \n\t"
        RESUME_SYNTAX
        : [dptr] REG_RW_LO (result), [lptr] REG_RW_LO (left), [rptr] REG_RW_LO (right),
    #if (uECC_MAX_WORDS != uECC_MIN_WORDS)
          [jump] REG_RW_LO (jump),
    #endif
          [carry] REG_WRITE_LO (carry), [left] REG_WRITE_LO (left_word),
          [right] REG_WRITE_LO (right_word)
        :
        : "cc", "memory"
    );
    return carry;
}
#define asm_add 1

uECC_VLI_API uECC_word_t uECC_vli_sub(uECC_word_t *result,
                                      const uECC_word_t *left,
                                      const uECC_word_t *right,
                                      wordcount_t num_words) {
#if (uECC_MAX_WORDS != uECC_MIN_WORDS)
  #if (uECC_PLATFORM == uECC_arm_thumb) || (uECC_PLATFORM == uECC_arm_thumb2)
    uint32_t jump = (uECC_MAX_WORDS - num_words) * 4 * 2 + 1;
  #else /* ARM */
    uint32_t jump = (uECC_MAX_WORDS - num_words) * 4 * 4;
  #endif
#endif
    uint32_t carry;
    uint32_t left_word;
    uint32_t right_word;
    
    __asm__ volatile (
        ".syntax unified \n\t"
        "movs %[carry], #0 \n\t"
    #if (uECC_MAX_WORDS != uECC_MIN_WORDS)
        "adr %[left], 1f \n\t"
        ".align 4 \n\t"
        "adds %[jump], %[left] \n\t"
    #endif
        
        "ldmia %[lptr]!, {%[left]} \n\t"
        "ldmia %[rptr]!, {%[right]} \n\t"
        "subs %[left], %[right] \n\t"
        "stmia %[dptr]!, {%[left]} \n\t"
        
    #if (uECC_MAX_WORDS != uECC_MIN_WORDS)
        "bx %[jump] \n\t"
    #endif
        "1: \n\t"
        REPEAT(DEC(uECC_MAX_WORDS),
            "ldmia %[lptr]!, {%[left]} \n\t"
            "ldmia %[rptr]!, {%[right]} \n\t"
            "sbcs %[left], %[right] \n\t"
            "stmia %[dptr]!, {%[left]} \n\t")
        
        "adcs %[carry], %[carry] \n\t"
        RESUME_SYNTAX
        : [dptr] REG_RW_LO (result), [lptr] REG_RW_LO (left), [rptr] REG_RW_LO (right),
    #if (uECC_MAX_WORDS != uECC_MIN_WORDS)
          [jump] REG_RW_LO (jump),
    #endif
          [carry] REG_WRITE_LO (carry), [left] REG_WRITE_LO (left_word),
          [right] REG_WRITE_LO (right_word)
        :
        : "cc", "memory"
    );
    return !carry; /* Note that on ARM, carry flag set means "no borrow" when subtracting
                      (for some reason...) */
}
#define asm_sub 1

#endif /* (uECC_OPTIMIZATION_LEVEL >= 2) */

/* ---- "Small" implementations ---- */

#if !asm_mult
uECC_VLI_API void uECC_vli_mult(uECC_word_t *result,
                                const uECC_word_t *left,
                                const uECC_word_t *right,
                                wordcount_t num_words) {
    uint32_t c0 = 0;
    uint32_t c1 = 0;
    uint32_t c2 = 0;
    uint32_t k = 0;
    uint32_t i;
    uint32_t t0, t1;
    
    __asm__ volatile (
        ".syntax unified \n\t"
        
        "1: \n\t" /* outer loop (k < num_words) */
        "movs %[i], #0 \n\t" /* i = 0 */
        "b 3f \n\t"
        
        "2: \n\t" /* outer loop (k >= num_words) */
        "movs %[i], %[k] \n\t"         /* i = k */
        "subs %[i], %[last_word] \n\t" /* i = k - (num_words - 1) (times 4) */
        
        "3: \n\t" /* inner loop */
        "subs %[t0], %[k], %[i] \n\t" /* t0 = k-i */
        
        "ldr %[t1], [%[right], %[t0]] \n\t" /* t1 = right[k - i] */
        "ldr %[t0], [%[left], %[i]] \n\t"   /* t0 = left[i] */
        
        "umull %[t0], %[t1], %[t0], %[t1] \n\t" /* (t0, t1) = left[i] * right[k - i] */
        
        "adds %[c0], %[c0], %[t0] \n\t" /* add low word to c0 */
        "adcs %[c1], %[c1], %[t1] \n\t" /* add high word to c1, including carry */
        "adcs %[c2], %[c2], #0 \n\t"    /* add carry to c2 */

        "adds %[i], #4 \n\t"          /* i += 4 */
        "cmp %[i], %[last_word] \n\t" /* i > (num_words - 1) (times 4)? */
        "bgt 4f \n\t"                 /*   if so, exit the loop */
        "cmp %[i], %[k] \n\t"         /* i <= k? */
        "ble 3b \n\t"                 /*   if so, continue looping */
        
        "4: \n\t" /* end inner loop */
        
        "str %[c0], [%[result], %[k]] \n\t" /* result[k] = c0 */
        "mov %[c0], %[c1] \n\t"       /* c0 = c1 */
        "mov %[c1], %[c2] \n\t"       /* c1 = c2 */
        "movs %[c2], #0 \n\t"         /* c2 = 0 */
        "adds %[k], #4 \n\t"          /* k += 4 */
        "cmp %[k], %[last_word] \n\t" /* k <= (num_words - 1) (times 4) ? */
        "ble 1b \n\t"                 /*   if so, loop back, start with i = 0 */
        "cmp %[k], %[last_word], lsl #1 \n\t" /* k <= (num_words * 2 - 2) (times 4) ? */
        "ble 2b \n\t"                 /*   if so, loop back, start with i = (k + 1) - num_words */
        /* end outer loop */
        
        "str %[c0], [%[result], %[k]] \n\t" /* result[num_words * 2 - 1] = c0 */
        RESUME_SYNTAX
        : [c0] "+r" (c0), [c1] "+r" (c1), [c2] "+r" (c2),
          [k] "+r" (k), [i] "=&r" (i), [t0] "=&r" (t0), [t1] "=&r" (t1)
        : [result] "r" (result), [left] "r" (left), [right] "r" (right),
          [last_word] "r" ((num_words - 1) * 4)
        : "cc", "memory"
    );
}
#define asm_mult 1
#endif

#endif /* _UECC_ASM_ARM_H_ */
