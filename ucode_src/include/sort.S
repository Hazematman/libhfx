#include <ucode/fixed_vector.h>


// Assume vertex data is the following format
// v04, v05 v1
// v06, v07 v2
// v08, v09 v3
// This function will take vertex data in any order
// and sort them so v04, v05 will be the highest vert,
// v06, v07 will be the middle vert
// and v08,v09 will be the lowest vert 
.global sort_tri
sort_tri:
    li $t7, 0xFFFF0000
    // Grab fixed point y1 and store in a1
    mfc2 a1, $v04, 2
    mfc2 a2, $v05, 2 
    sll $a1, $a1, 16
    and $a2, $a2, $t7 
    or $a1, $a1, $a2

    // Grab fixed point y2 and store in a2
    mfc2 a2, $v06, 2
    mfc2 a3, $v07, 2
    sll $a2, $a2, 16
    and $a3, $a3, $t7
    or $a2, $a2, $a3

    // Grab fixed point y3 and store in a3
    mfc2 a3, $v08, 2
    mfc2 t0, $v09, 2
    sll $a3, $a3, 16
    and $t0, $t0, $t7
    or $a3, $a3, $t0

    // Check if y1 > y2
    slt $t1, $a2, $a1
    beq $t1, $zero, second_compare
    nop

    // Set temp (v10,v11) to v2
    vaddc $v10, $v06, $v00, 0
    vaddc $v11, $v07, $v00, 0 

    // Set v2 = v1
    vaddc $v06, $v04, $v00, 0
    vaddc $v07, $v05, $v00, 0

    // Set v1 = temp
    vaddc $v04, $v10, $v00, 0
    vaddc $v05, $v11, $v00, 0

    // Swap y1 and y2
    move $t0, $a1
    move $a1, $a2
    move $a2, $t0

    // Check if y2 > y3
second_compare:
    slt $t1, $a3, $a2
    beq $t1, $zero, third_compare
    nop

    // Set temp (v10,v11) to v3
    vaddc $v10, $v08, $v00, 0
    vaddc $v11, $v09, $v00, 0

    // Set v3 to v2
    vaddc $v08, $v06, $v00, 0
    vaddc $v09, $v07, $v00, 0

    // Set v2 to temp
    vaddc $v06, $v10, $v00, 0
    vaddc $v07, $v11, $v00, 0

    // Swap y2 and y3
    move $t0, $a2
    move $a2, $a3
    move $a3, $t0

    // Check if y1 > y2
third_compare:
    slt $t1, $a2, $a1
    beq $t1, $zero, done_compare
    nop

    // Set temp (v10,v11) to v2
    vaddc $v10, $v06, $v00, 0
    vaddc $v11, $v07, $v00, 0 

    // Set v2 = v1
    vaddc $v06, $v04, $v00, 0
    vaddc $v07, $v05, $v00, 0

    // Set v1 = temp
    vaddc $v04, $v10, $v00, 0
    vaddc $v05, $v11, $v00, 0

done_compare:

    jr $ra
    nop
