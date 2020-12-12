#include <hfx.h>
#include <hfx_types.h>
#include <hfx_int.h>
#include <hfx_rb.h>
#include <hfx_rdp.h>

void hfx_tex_image_2d(hfx_state *state, uint32_t target, int32_t level, int32_t internalformat, uint32_t width, uint32_t height, int32_t border, uint32_t format, uint32_t type, const void *data)
{
    uint64_t cmds[3];

    uint32_t size_in_bytes = width*2; // TODO This is hardcoded uint16
    uint32_t round_amt = (size_in_bytes%8) ? 1 : 0;
    uint32_t size_in_words = (size_in_bytes/8) + round_amt;

    // TODO probably need to verify that texture data is 64 byte aligned
    cmds[0] = HFX_RDP_PKT_SET_TEXTURE_IMAGE(HFX_RDP_CMD_SET_TEXTURE_IMAGE_RGBA,
                                            HFX_RDP_CMD_SET_TEXTURE_IMAGE_16B,
                                            width-1, (uintptr_t)data);
    cmds[1] = HFX_RDP_PKT_SET_TILE(HFX_RDP_CMD_SET_TILE_RBGA,
                                   HFX_RDP_CMD_SET_TILE_16B,
                                   size_in_words,
                                   0, // TODO always loading to start of TMEM
                                   0, // TODO tile desc is set to 0
                                   0, // TODO palette is hardcoded to 0
                                   0, // TODO ct set to zero
                                   0, // TODO mt set to zero
                                   3, // TODO mask_t hardcoded to 3 bits
                                   0, // TODO shift_t set to zero
                                   0, // TODO cs set to zero
                                   0, // TODO ms set to zero
                                   3, // TODO mask_s hardcoded to 3 bits
                                   0 // TODO set shift to zero
                                   );
    cmds[2] = HFX_RDP_PKT_LOAD_TILE(0, // TODO set sl to zero
                                    0, // TODO set tl to zeor
                                    0, // TODO set tile to zero
                                    (width-1)<<2,
                                    (height-1)<<2);

    hfx_cmd_rdp(state, sizeof(cmds)/sizeof(uint64_t), cmds);
}