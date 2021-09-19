#include "mat_vec.S"

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
    lqv $v10, 0, 6, $0   
    lqv $v11, 0, 7, $0   
    lqv $v12, 0, 8, $0   
    lqv $v13, 0, 9, $0   
    jr $ra
    nop

.global output_data
output_data:
    // output vector registers to memory
    ssv $v04, 0, 0, $0
    ssv $v05, 0, 1, $0
    ssv $v04, 2, 2, $0
    ssv $v05, 2, 3, $0
    ssv $v04, 4, 4, $0
    ssv $v05, 4, 5, $0
    ssv $v04, 6, 6, $0
    ssv $v05, 6, 7, $0

    jr $ra
    nop
