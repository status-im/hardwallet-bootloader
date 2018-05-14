/*
 * cifra - embedded cryptography library
 * Written in 2014 by Joseph Birr-Pixton <jpixton@gmail.com>
 *
 * To the extent possible under law, the author(s) have dedicated all
 * copyright and related and neighboring rights to this software to the
 * public domain worldwide. This software is distributed without any
 * warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication
 * along with this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

#ifndef SHA2_H
#define SHA2_H

#include <stddef.h>
#include <stdint.h>

/* .. c:macro:: CF_SHA256_HASHSZ
 * The output size of SHA256: 32 bytes. */
#define CF_SHA256_HASHSZ 32

/* .. c:macro:: CF_SHA256_BLOCKSZ
 * The block size of SHA256: 64 bytes. */
#define CF_SHA256_BLOCKSZ 64

/* .. c:type:: cf_sha256_context
 * Incremental SHA256 hashing context.
 *
 * .. c:member:: cf_sha256_context.H
 * Intermediate values.
 *
 * .. c:member:: cf_sha256_context.partial
 * Unprocessed input.
 *
 * .. c:member:: cf_sha256_context.npartial
 * Number of bytes of unprocessed input.
 *
 * .. c:member:: cf_sha256_context.blocks
 * Number of full blocks processed.
 */
typedef struct
{
  uint32_t H[8];                      /* State. */
  uint8_t partial[CF_SHA256_BLOCKSZ]; /* Partial block of input. */
  uint32_t blocks;                    /* Number of full blocks processed into H. */
  size_t npartial;                    /* Number of bytes in prefix of partial. */
} cf_sha256_context;

/* .. c:function:: $DECL
 * Sets up `ctx` ready to hash a new message.
 */
extern void cf_sha256_init(cf_sha256_context *ctx);

/* .. c:function:: $DECL
 * Hashes `nbytes` at `data`.  Copies the data if there isn't enough to make
 * a full block.
 */
extern void cf_sha256_update(cf_sha256_context *ctx, const void *data, size_t nbytes);

/* .. c:function:: $DECL
 * Finishes the hash operation, writing `CF_SHA256_HASHSZ` bytes to `hash`.
 *
 * This destroys `ctx`.
 */
extern void cf_sha256_digest(cf_sha256_context *ctx, uint8_t hash[CF_SHA256_HASHSZ]);

#endif
