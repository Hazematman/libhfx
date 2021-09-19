#include <ucode/fixed_vector.h>

// Assume data is in the following format
// v04, v05 vector
// v06, v07 matrix row 1
// v08, v09 matrix row 2
// v10, v11 matrix row 3
// v12, v13 matrix row 4
// Note: Matrix is already transposed so each vector
// register represents one row in the matrix
// Return results transformed vector in v04, v05
.global mul_mat_vec
mul_mat_vec:
    // Multiply vector with first row of matrix
    mul_fixed $v14, $v15, $v06, $v07, $v04, $v05
    // Accumulate the results of the multiply
    add_fixed $v16, $v17, $v14, $v15, $v14, $v15, w1
    add_fixed $v16, $v17, $v16, $v17, $v14, $v15, w2
    add_fixed $v16, $v17, $v16, $v17, $v14, $v15, w3

    // Multiply vector with first second of matrix
    mul_fixed $v14, $v15, $v08, $v09, $v04, $v05
    // Accumulate the results of the multiply
    add_fixed $v18, $v19, $v14, $v15, $v14, $v15, w1
    add_fixed $v18, $v19, $v18, $v19, $v14, $v15, w2
    add_fixed $v18, $v19, $v18, $v19, $v14, $v15, w3

    // Multiply vector with third row of matrix
    mul_fixed $v14, $v15, $v10, $v11, $v04, $v05
    // Accumulate the results of the multiply
    add_fixed $v20, $v21, $v14, $v15, $v14, $v15, w1
    add_fixed $v20, $v21, $v20, $v21, $v14, $v15, w2
    add_fixed $v20, $v21, $v20, $v21, $v14, $v15, w3

    // Multiply vector with first row of matrix
    mul_fixed $v14, $v15, $v12, $v13, $v04, $v05
    // Accumulate the results of the multiply
    add_fixed $v22, $v23, $v14, $v15, $v14, $v15, w1
    add_fixed $v22, $v23, $v22, $v23, $v14, $v15, w2
    add_fixed $v22, $v23, $v22, $v23, $v14, $v15, w3

    // Store results back in v04, v05
    vmov $v04, 0, $v16, w0
    vmov $v05, 0, $v17, w0
    vmov $v04, 1, $v18, w0
    vmov $v05, 1, $v19, w0
    vmov $v04, 2, $v20, w0
    vmov $v05, 2, $v21, w0
    vmov $v04, 3, $v22, w0
    vmov $v05, 3, $v23, w0

    jr $ra
    nop
