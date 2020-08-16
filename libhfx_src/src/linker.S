.section .data

.balign 8
.global _hfx_ucode_data_start
_hfx_ucode_data_start:

.incbin DATA_SECTION_BIN

.balign 8
.global _hfx_ucode_start
_hfx_ucode_start:

.incbin TEXT_SECTION_BIN

.balign 8
.global _hfx_ucode_end
_hfx_ucode_end:
