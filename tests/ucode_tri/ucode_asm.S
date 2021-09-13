#include "tri.S"

.global setup_data
setup_data:
    // Initalize $4 to 0
    li $4, 0 

    // Load test data in vector registers
    lqv $v04, 0, 0, $0
    lqv $v05, 0, 1, $0
    lqv $v06, 0, 2, $0
    lqv $v07, 0, 3, $0
    lqv $v08, 0, 4, $0
    lqv $v09, 0, 5, $0   
    jr $ra
    nop

.global output_data
output_data:
    // output vector registers to memory
    sqv $v16, 0, 0, $0
    sqv $v17, 0, 1, $0
    sqv $v18, 0, 2, $0
    sqv $v19, 0, 3, $0
    sqv $v20, 0, 4, $0
    sqv $v21, 0, 5, $0 
    sqv $v14, 0, 6, $0
    sqv $v15, 0, 7, $0
    jr $ra
    nop
