#include <ucode/fixed_vector.h> 

    
// Assume vertex data is the following format
// v04,v05 v1
// v06,v07 v2
// v08,v09 v3
// Also assume verticies are sorted from highest to lowest, 
// with v1 at the top of the display
// Also assume that $4 contains a pointer RDP ring buffer
.global draw_tri
draw_tri:
    // Calculate x3-x1, y3-y1
    sub_fixed $v10, $v11, $v08, $v09, $v04, $v05
    // Calculate x2-x1, y2-y1
    sub_fixed $v12, $v13, $v06, $v07, $v04, $v05
    // Calculate x3-x2, y3-y2
    sub_fixed $v14, $v15, $v08, $v09, $v06, $v07

    // Compute 1/(y3-y1)
    // This follows RSP doc pg. 79 for how to compute
    // the receipricol and then multiply by 2
    vrcph $v16, 1, $v10, 1
    vrcpl $v17, 1, $v11, 1
    vrcph $v16, 1, $v00, 0

    vmudn $v19, $v17, $v02, w1 // w1 contains constant 2
    vmadm $v18, $v16, $v02, w1
    vmadn $v19, $v00, $v00, 0

    // Calculate (x3-x1) * (1/(y3-y1))
    // After this block v16,v17 contain DxHDy 
    vmudl $v17, $v11, $v19, h1
    vmadm $v16, $v10, $v19, h1
    vmadn $v17, $v00, $v00, 0

    // Compute 1/(y2-y1)
    // This follows RSP doc pg. 79 for how to compute
    // the receipricol and then multiply by 2
    vrcph $v18, 1, $v12, 1
    vrcpl $v19, 1, $v13, 1
    vrcph $v18, 1, $v00, 0

    vmudn $v21, $v19, $v02, w1 // w1 contains constant 2
    vmadm $v20, $v18, $v02, w1
    vmadn $v21, $v00, $v00, w1

    // Calculate (x2-x1) * (1/(y2-y1))
    // After this block v18,v19 contain DxMDy 
    vmudl $v19, $v13, $v21, h1
    vmadm $v18, $v12, $v21, h1,
    vmadn $v19, $v00, $v00, 0

    // Compute 1/(y3-y2)
    // This follows RSP doc pg. 79 for how to compute
    // the receipricol and then multiply by 2
    vrcph $v20, 1, $v14, 1
    vrcpl $v21, 1, $v15, 1
    vrcph $v20, 1, $v00, 0

    vmudn $v23, $v21, $v02, w1 // w1 contains constant 2
    vmadm $v22, $v20, $v02, w1
    vmadn $v23, $v00, $v00, 0

    // Calculate (x3-x2) * (1/(y3-y2))
    // After this block v20,v21 contain DxLDy 
    vmudl $v21, $v15, $v23, h1
    vmadm $v20, $v14, $v23, h1
    vmadn $v21, $v00, $v00, 0

    // We have now computed DxHDy(v16,v17)  DxMDy(v18,v19), DxLDy(v20,v21)
    // We need to calculate the triangle winding order with
    // a the calculation of (x1*y2 - x2*y1) + (x2*y3 - x3*y2) + (x3*y1 - x1*y3)
    // and we can check if the triangle is flipped based on if the result is positive

    // In order to perform a cross product we need vectors that look like a=(x1, x2, x3)
    // and b=(y1,y2,y3), c=(x2,x3,x1), d=(y2,y3,y1)
    // Then we can do r=(a*d) - (c*b) and accumlate the results of this vector (r[0]+r[1]+r[2])
    // to obtain the winding value
    vmov $v10, 0, $v04, w0
    vmov $v10, 1, $v06, w0
    vmov $v10, 2, $v08, w0
    vmov $v11, 0, $v05, w0
    vmov $v11, 1, $v07, w0
    vmov $v11, 2, $v09, w0

    vmov $v12, 0, $v04, w1
    vmov $v12, 1, $v06, w1
    vmov $v12, 2, $v08, w1
    vmov $v13, 0, $v05, w1
    vmov $v13, 1, $v07, w1
    vmov $v13, 2, $v09, w1

    vmov $v14, 0, $v10, w1
    vmov $v14, 1, $v10, w2
    vmov $v14, 2, $v10, w0
    vmov $v15, 0, $v11, w1
    vmov $v15, 1, $v11, w2
    vmov $v15, 2, $v11, w0

    vmov $v22, 0, $v12, w1
    vmov $v22, 1, $v12, w2
    vmov $v22, 2, $v12, w0
    vmov $v23, 0, $v13, w1
    vmov $v23, 1, $v13, w2
    vmov $v23, 2, $v13, w0

    mul_fixed $v24, $v25, $v10, $v11, $v22, $v23
    mul_fixed $v26, $v27, $v12, $v13, $v14, $v15
    sub_fixed $v10, $v11, $v24, $v25, $v26, $v27

    // $v10,$v11 no contains the cross product of the (x1,x2,x3) (y1,y2,y3), we need to accumlate
    // the result[0] + result[1] + result[2]
    add_fixed $v12, $v13, $v10, $v11, $v10, $v11, w1
    add_fixed $v14, $v15, $v12, $v13, $v10, $v11, w2

    // Now store all of these values into the RDP command buffer
    add $5, $0, $0
    lui $5, 0x0800
    mfc2 a2, $v14, 0  
    slt $6, $0, $6
    sll $6, $6, 23
    or $5, $5, $6

    // Load YL interger in a2
    mfc2 a2, $v08, 1
    // Load YL frac in a3
    mfc2 a3, $v09, 1

    // Get the upper 2 bits of YL frac and shift them to the start of the register
    andi $a3, $a3, 0xC000
    srl $a3, $a3, 14
    
    // Get the lower 11 bits and sign of YL int and sift them over two bits to make room for YL frac
    andi $a2, $a2, 0x1FFF
    sll $a2, $a2, 2

    // OR YL int and YL frac to combine them
    or $a2, $a2, $a3
    
    // OR YL with the command buffer register
    or $5, $5, $a2

    // Store first 32bit word of command in RDP command buffer
    sw $5, 0($4)

    // Load YM interger in a2
    mfc2 a2, $v06, 1
    // Load YM frac in a3
    mfc2 a3, $v07, 1

    // Get the upper 2 bits of YM frac and shift them to the start of the register
    andi $a3, $a3, 0xC000
    srl $a3, $a3, 14
    
    // Get the lower 11 bits and sign of YM int and sift them over two bits to make room for YM frac
    andi $a2, $a2, 0x1FFF
    sll $a2, $a2, 2

    // OR YM int and YM frac to combine them
    or $a2, $a2, $a3

    // Move YM into upper part of next word
    sll $5, $a2, 16

    // Load YH interger in a2
    mfc2 a2, $v04, 1
    // Load YH frac in a3
    mfc2 a3, $v05, 1

    // Get the upper 2 bits of YH frac and shift them to the start of the register
    andi $a3, $a3, 0xC000
    srl $a3, $a3, 14
    
    // Get the lower 11 bits and sign of YH int and sift them over two bits to make room for YH frac
    andi $a2, $a2, 0x1FFF
    sll $a2, $a2, 2

    // OR YH int and YH frac to combine them
    or $a2, $a2, $a3

    // OR YM and YH to combine them
    or $5, $5, $a2

    // Store second 32bit word of command in RDP buffer
    sw $5, 4($4)

    ssv $v08, 0, 4, $4
    ssv $v09, 0, 5, $4
    ssv $v20, 0, 6, $4
    ssv $v21, 0, 7, $4

    ssv $v04, 0, 8, $4
    ssv $v05, 0, 9, $4
    ssv $v16, 0, 10, $4
    ssv $v17, 0, 11, $4

    ssv $v06, 0, 12, $4
    ssv $v07, 0, 13, $4
    ssv $v18, 0, 14, $4
    ssv $v19, 0, 15, $4

    jr $ra
    nop

