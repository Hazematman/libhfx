#include <ucode/fixed_vector.h> 

    
// Assume vertex data is the following format
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

    jr $ra
    nop

