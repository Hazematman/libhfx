#include "sort.S"

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
    ssv $v04, 0, 0, $0
    ssv $v05, 0, 1, $0
    ssv $v04, 2, 2, $0
    ssv $v05, 2, 3, $0

    ssv $v06, 0, 4, $0
    ssv $v07, 0, 5, $0
    ssv $v06, 2, 6, $0
    ssv $v07, 2, 7, $0

    ssv $v08, 0, 8, $0
    ssv $v09, 0, 9, $0
    ssv $v08, 2, 10, $0
    ssv $v09, 2, 11, $0

    jr $ra
    nop
