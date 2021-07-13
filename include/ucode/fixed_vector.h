#ifndef FIXED_VECTOR_H
#define FIXED_VECTOR_H

#include <ucode.S>

// TODO fill them with element redefine recommendations
// from pg. 58 of the RSP programming guide
// TODO submit pull request to libdragon to include these
.set f,  0b0000
.set q0, 0b0010
.set q1, 0b0011
.set h0, 0b0100
.set h1, 0b0101
.set h2, 0b0110
.set h3, 0b0111
.set w0, 0b1000
.set w1, 0b1001
.set w2, 0b1010
.set w3, 0b1011
.set w4, 0b1100
.set w5, 0b1101
.set w6, 0b1110
.set w7, 0b1111

.macro add_fixed res_hi, res_lo, a_hi, a_lo, b_hi, b_lo, e=0
    vaddc \res_lo, \a_lo, \b_lo, \e
    vadd  \res_hi, \a_hi, \b_hi, \e
.endm

.macro sub_fixed res_hi, res_lo, a_hi, a_lo, b_hi, b_lo
    vsubc   \res_lo, \a_lo, \b_lo, 0
    vsub    \res_hi, \a_hi, \b_hi, 0
.endm

.macro mul_fixed res_hi, res_lo, a_hi, a_lo, b_hi, b_lo, e=0
    vmudl \res_hi, \a_lo, \b_lo, \e
    vmadm \res_hi, \a_hi, \b_lo, \e
    vmadn \res_lo, \a_lo, \b_hi, \e
    vmadh \res_hi, \a_hi, \b_hi, \e
.endm


#endif
