	.text
	.syntax unified
	.global memzero
	.func memzero
	.thumb_func

memzero:
	/* on entry
	 *   r0 = start
	 *   r1 = end
	 * on exit
	 *   r0 = end
	 */
    mov r2, #0
 L_loop_begin:
    str r2, [r0], 4
    cmp r0, r1
    bne L_loop_begin
    bx lr
