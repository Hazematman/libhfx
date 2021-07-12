#include <ucode/fixed_vector.h> 

    
// Assume vertex data is the following
// v04,v05 v1
// v06,v07 v2
// v08,v09 v3
// Also assume verticies are sorted from highest to lowest, 
// with v1 at the top of the display
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
    vrcph $v16, 1, $v10, 0
    vrcpl $v17, 1, $v11, 0
    vrcph $v16, 1, $v00, 0

    vmudn $v19, $v17, $v02, 1 // e1 contains constant 2
    vmadm $v18, $v16, $v02, 1
    vmadn $v19, $v00, $v00, 0

    // Calculate (x3-x1) * (1/(y3-y1))
    // After this block v16,v17 contain DxHDy 
    vmudl $v17, $v11, $v19, 0
    vmadm $v16, $v10, $v19, 0,
    vmadn $v17, $v00, $v00, 0

    // Compute 1/(y2-y1)
    // This follows RSP doc pg. 79 for how to compute
    // the receipricol and then multiply by 2
    vrcph $v18, 1, $v12, 0
    vrcpl $v19, 1, $v13, 0
    vrcph $v18, 1, $v00, 0

    vmudn $v21, $v19, $v02, 1 // e1 contains constant 2
    vmadm $v20, $v18, $v02, 1
    vmadn $v21, $v00, $v00, 0

    // Calculate (x2-x1) * (1/(y2-y1))
    // After this block v18,v19 contain DxMDy 
    vmudl $v19, $v13, $v21, 0
    vmadm $v18, $v12, $v21, 0,
    vmadn $v19, $v00, $v00, 0

    // Compute 1/(y3-y2)
    // This follows RSP doc pg. 79 for how to compute
    // the receipricol and then multiply by 2
    vrcph $v20, 1, $v14, 0
    vrcpl $v21, 1, $v15, 0
    vrcph $v20, 1, $v00, 0

    vmudn $v23, $v21, $v02, 1 // e1 contains constant 2
    vmadm $v22, $v20, $v02, 1
    vmadn $v23, $v00, $v00, 0

    // Calculate (x3-x2) * (1/(y3-y2))
    // After this block v20,v21 contain DxLDy 
    vmudl $v21, $v15, $v23, 0
    vmadm $v20, $v14, $v23, 0,
    vmadn $v21, $v00, $v00, 0

    // We have now computed DxHDy, DxMDy, DxLDy
    // We need to calculate the triangle winding order with
    // a the calculation of (x1*y2 - x2*y1) + (x2*y3 - x3*y2) + (x3*y1 - x1*y3)
    // and we can check if the triangle is flipped based on if the result is positive

    jr $ra
    nop

