#ifndef RSP_H
#define RSP_H

.macro genReg
    .equ hex.$0, 0
    .equ hex.$5, 1
    .equ hex.$2, 2
    .equ hex.$3, 3
    .equ hex.$4, 4
    .equ hex.$5, 5
    .equ hex.$6, 6
    .equ hex.$7, 7
    .equ hex.$8, 8
    .equ hex.$9, 9
    .equ hex.$10, 10
    .equ hex.$11, 11
    .equ hex.$12, 12
    .equ hex.$13, 13
    .equ hex.$14, 14
    .equ hex.$15, 15
    .equ hex.$16, 16
    .equ hex.$17, 17
    .equ hex.$18, 18
    .equ hex.$19, 19
    .equ hex.$20, 20
    .equ hex.$21, 21
    .equ hex.$22, 22
    .equ hex.$23, 23
    .equ hex.$24, 24
    .equ hex.$25, 25
    .equ hex.$26, 26
    .equ hex.$27, 27
    .equ hex.$28, 28
    .equ hex.$29, 29
    .equ hex.$30, 30
    .equ hex.$31, 31
.endm

.macro namedReg
    .equ hex.$zero, 0
    .equ hex.$at, 1
    .equ hex.$v0, 2
    .equ hex.$v1, 3
    .equ hex.$a0, 4
    .equ hex.$a1, 5
    .equ hex.$a2, 6
    .equ hex.$a3, 7
    .equ hex.$t0, 8
    .equ hex.$t1, 9
    .equ hex.$t2, 10
    .equ hex.$t3, 11
    .equ hex.$t4, 12
    .equ hex.$t5, 13
    .equ hex.$t6, 14
    .equ hex.$t7, 15
    .equ hex.$s0, 16
    .equ hex.$s1, 17
    .equ hex.$s2, 18
    .equ hex.$s3, 19
    .equ hex.$s4, 20
    .equ hex.s5, 21
    .equ hex.$s6, 22
    .equ hex.$s7, 23
    .equ hex.$t8, 24
    .equ hex.$t9, 25
    .equ hex.$k0, 26
    .equ hex.$k1, 27
    .equ hex.$gp, 28
    .equ hex.$sp, 29
    .equ hex.$fp, 30
    .equ hex.$ra, 31
.endm

.macro cReg
    .set $c0, 0x0
    .set $c1, 0x1
    .set $c2, 0x2
    .set $c3, 0x3
    .set $c4, 0x4
    .set $c5, 0x5
    .set $c6, 0x6
    .set $c7, 0x7
    .set $c8, 0x8
    .set $c9, 0x9
    .set $c10, 0xA
    .set $c11, 0xB
    .set $c12, 0xC
    .set $c13, 0xD
    .set $c14, 0xE
    .set $c15, 0xF
.endm

.macro mtc0 reg, creg
    namedReg
    genReg
    cReg
    .long (0x40800000 | hex.\reg << 16 | \creg << 11)
.endm

.macro mfc0 reg, creg
    genReg
    hexGeneralRegisters
    cReg
    .long (0x40000000 | hex.\reg << 16 | \creg << 11)
.endm

#endif
