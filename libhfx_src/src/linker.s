.section .data

.balign 8
.global __ucode_data_start
__ucode_data_start:

.incbin DATA_SECTION_BIN

.balign 8
.global __ucode_start
__ucode_start:

.incbin TEXT_SECTION_BIN

.balign 8
.global __ucode_end
__ucode_end:
