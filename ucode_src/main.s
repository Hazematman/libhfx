#include <ucode.S>
#include <ucode/rsp.h>
#include <ucode/hfx_asm.h>

.text
__start:
    move REG_RB_END, $zero // Load RB_END registers with zero
    
    /* Commands are processed like this:
        1. Check if RB_END needs to be updated
        2. Get RB size
        3. If size > 0 process 1 command
        4. repeat until the end of time 
    */
forever_loop:
    jal check_rb_end_ptr
        nop
    lw REG_RB_START, HFX_REG_RB_START($zero) /* Note $t5 will have RB_START for the entire duration of this */
    /* If there are no commands just restart */
    beq REG_RB_START, REG_RB_END, forever_loop
        nop
    
    /* This block processes one command and then increments RB_START */
    // TODO this will have to change when hfx_rb_buffer is just a window to a larger RB in RAM
    lw $t4, %lo(hfx_rb_buffer)(REG_RB_START)
    sll $t4, $t4, 2
    lw $t5, %lo(offset_table)($t4)
    
    jalr $t5
        nop
    
    andi REG_RB_START, REG_RB_START, 0x3FFF // TODO replace this with properly calculated bit mask
    sw REG_RB_START, HFX_REG_RB_START($zero)
    
end:    
    j forever_loop
        nop


check_rb_end_ptr:
    lw $t4, HFX_REG_RB_END($zero)
    beq $t4, REG_RB_END, check_rb_end_ptr_done
        nop
    /* If we didn't branch there is new data in ringbuffer, load ring buffer with data new data */
    // TODO come up logic to only load new data instead of whole RB
    // TODO remove wait for DMA to complete and add logic to check before we process
    li $t5, %lo(hfx_rb_buffer)
    lw $t6, HFX_REG_RB_ADDR($zero)
    lw $t7, HFX_REG_RB_SIZE($zero)
    addi $t7, $t7, -1
    mtc0 $t5, $c0
    mtc0 $t6, $c1
    mtc0 $t7, $c2
    // Now wait for DMA to be compelete
check_rb_end_ptr_dma_wait:
    mfc0 $t5, $c6 // Check DMA busy status
    bne $t5, $0, check_rb_end_ptr_dma_wait
        nop
    
    // Set RB_END to be new value
    move REG_RB_END, $t4
check_rb_end_ptr_done:    
    jr $ra
        nop
        

.data
.global hfx_registers
hfx_registers:
    .fill 32
 
.balign 8
.global hfx_rb_buffer
hfx_rb_buffer:
    .fill 1024
 
/* Ensure that this matches hfx.h */
// TODO find our if we can auto generate this so that it always matches hfx.h
.global offset_table
offset_table:
    .word hfx_cmd_nop
    .word hfx_cmd_int
    .word hfx_cmd_dma_read
