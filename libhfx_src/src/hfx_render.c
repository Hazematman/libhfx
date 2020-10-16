#include <hfx.h>
#include <hfx_types.h>
#include <hfx_int.h>
#include <hfx_rb.h>
#include <hfx_rdp.h>

void hfx_render_init(hfx_state *state)
{
    uint64_t cmds[] = 
    {
        /* Enable primitive fill */
        0xEF0000FF80000000ull,
    };

    hfx_cmd_rdp(state, sizeof(cmds)/sizeof(uint64_t), cmds);
}

void hfx_set_primitive_color(hfx_state *state, uint32_t color)
{
    uint64_t cmds[] = 
    {
        0xF900000000000000ull,
    };
    cmds[0] = cmds[0] | color;
    
    hfx_cmd_rdp(state, sizeof(cmds)/sizeof(uint64_t), cmds);

}

void hfx_render_tri(hfx_state *state, HFXfixed *v1, HFXfixed *v2, HFXfixed *v3)
{
    uint32_t a1 = (v1[0] & 0xFFFF0000) | ((v1[1] & 0xFFFF0000) >> 16);
    uint32_t a2 = (v1[2] & 0xFFFF0000) | ((v1[3] & 0xFFFF0000) >> 16);
    uint32_t b1 = (v2[0] & 0xFFFF0000) | ((v2[1] & 0xFFFF0000) >> 16);
    uint32_t b2 = (v2[2] & 0xFFFF0000) | ((v2[3] & 0xFFFF0000) >> 16);
    uint32_t c1 = (v3[0] & 0xFFFF0000) | ((v3[1] & 0xFFFF0000) >> 16);
    uint32_t c2 = (v3[2] & 0xFFFF0000) | ((v3[3] & 0xFFFF0000) >> 16);

    uint32_t d1 = ((v1[0] & 0x0000FFFF) << 16) | (v1[1] & 0x0000FFFF);
    uint32_t d2 = ((v1[2] & 0x0000FFFF) << 16) | (v1[3] & 0x0000FFFF);
    uint32_t e1 = ((v2[0] & 0x0000FFFF) << 16) | (v2[1] & 0x0000FFFF);
    uint32_t e2 = ((v2[2] & 0x0000FFFF) << 16) | (v2[3] & 0x0000FFFF);
    uint32_t f1 = ((v3[0] & 0x0000FFFF) << 16) | (v3[1] & 0x0000FFFF);
    uint32_t f2 = ((v3[2] & 0x0000FFFF) << 16) | (v3[3] & 0x0000FFFF);

    hfx_rb_reserve(state, 13);
    hfx_rb_queue(state, HFX_CMD_TRI);
    hfx_rb_queue(state, a1);
    hfx_rb_queue(state, a2);
    hfx_rb_queue(state, b1);
    hfx_rb_queue(state, b2);
    hfx_rb_queue(state, c1);
    hfx_rb_queue(state, c2);
    hfx_rb_queue(state, d1);
    hfx_rb_queue(state, d2);
    hfx_rb_queue(state, e1);
    hfx_rb_queue(state, e2);
    hfx_rb_queue(state, f1);
    hfx_rb_queue(state, f2);
}

void hfx_render_tri_f(hfx_state *state, float *v1, float *v2, float *v3)
{
    /* Credit to libdragon rdp_draw_filled_triangle for providing the */
    /* conversion algorithm */
    uint64_t edge_coef[4+8];
    uint32_t buffer_index = 0;
    const float to_fixed_11_2 = 4.0f;
    const float to_fixed_16_16 = 65536.0f;
    float temp_x, temp_y;
    float x1 = v1[0], y1 = v1[1];
    float x2 = v2[0], y2 = v2[1];
    float x3 = v3[0], y3 = v3[1];

    /* sort vertices by Y ascending to find the major, mid and low edges */
    if( y1 > y2 ) { temp_x = x2, temp_y = y2; y2 = y1; y1 = temp_y; x2 = x1; x1 = temp_x; }
    if( y2 > y3 ) { temp_x = x3, temp_y = y3; y3 = y2; y2 = temp_y; x3 = x2; x2 = temp_x; }
    if( y1 > y2 ) { temp_x = x2, temp_y = y2; y2 = y1; y1 = temp_y; x2 = x1; x1 = temp_x; }

    /* calculate Y edge coefficients in 11.2 fixed format */
    uint32_t yh = y1 * to_fixed_11_2;
    uint32_t ym = y2 * to_fixed_11_2;
    uint32_t yl = y3 * to_fixed_11_2;
    
    /* calculate X edge coefficients in 16.16 fixed format */
    uint32_t xh = x1 * to_fixed_16_16;
    uint32_t xm = x1 * to_fixed_16_16;
    uint32_t xl = x2 * to_fixed_16_16;
    
    /* calculate inverse slopes in 16.16 fixed format */
    uint32_t dxhdy = ( y3 == y1 ) ? 0 : ( ( x3 - x1 ) / ( y3 - y1 ) ) * to_fixed_16_16;
    uint32_t dxmdy = ( y2 == y1 ) ? 0 : ( ( x2 - x1 ) / ( y2 - y1 ) ) * to_fixed_16_16;
    uint32_t dxldy = ( y3 == y2 ) ? 0 : ( ( x3 - x2 ) / ( y3 - y2 ) ) * to_fixed_16_16;
    
    /* determine the winding of the triangle */
    int32_t winding = ( x1 * y2 - x2 * y1 ) + ( x2 * y3 - x3 * y2 ) + ( x3 * y1 - x1 * y3 );
    uint32_t flip = (winding > 0 ? 1 : 0 ); 

    HFX_RDP_PKT_TRI_NON_SHADE(edge_coef,
                              HFX_RDP_CMD_TRI_SHADE,
                              flip,
                              0,
                              0,
                              yl,
                              ym,
                              yh,
                              xl,
                              dxldy,
                              xh,
                              dxhdy,
                              xm,
                              dxmdy);
    HFX_RDP_PKT_TRI_SHADE(edge_coef, (255ull<<16), 0, 0, (255ull<<16),
                                     0,0,0,0,
                                     0,0,0,0,
                                     0,0,0,0);

    hfx_cmd_rdp(state, sizeof(edge_coef)/sizeof(uint64_t), edge_coef);
}