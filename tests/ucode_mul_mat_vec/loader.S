.section .data

.balign 8
.global _ucode_data_start
_ucode_data_start:

.incbin "ucode_mul_mat_vec_ucode_data.bin"

.balign 8
.global _ucode_start
_ucode_start:

.incbin "ucode_mul_mat_vec_ucode_text.bin"

.balign 8
.global _ucode_end
_ucode_end:
