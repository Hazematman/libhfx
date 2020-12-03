#include <hfx.h>
#include <hfx_types.h>
#include <hfx_int.h>
#include <hfx_rb.h>
#include <hfx_rdp.h>
#include <math.h>
#include <stdio.h>

#define MIN_FLOAT 0.0001f

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

void barycentric(float px, float py, float x1, float y1, float x2, float y2, float x3, float y3, float *u, float *v, float *w)
{
    float v0x = x2 - x1, v0y = y2 - y1; 
    float v1x = x3 - x1, v1y = y3 - y1; 
    float v2x = px - x1, v2y = py - y1;
    float d00 = v0x*v0x + v0y * v0y;
    float d01 = v0x*v1x + v0y * v1y;
    float d11 = v1x*v1x + v1y * v1y;
    float d20 = v2x*v0x + v2y * v0y;
    float d21 = v2x*v1x + v2y * v1y;
    float denom = d00 * d11 - d01 * d01;
    *v = (fabs(denom) < MIN_FLOAT) ? 0.0f : ((d11 * d20 - d01 * d21) / denom);
    *w = (fabs(denom) < MIN_FLOAT) ? 0.0f : ((d00 * d21 - d01 * d20) / denom);
    *u = 1.0f - *v - *w;
}

uint32_t f_to_u(float a)
{
    return *(uint32_t*)&a;
}

void hfx_render_tri_f(hfx_state *state, float *v1, float *v2, float *v3, float *vc1, float *vc2, float *vc3)
{
    /* Credit to libdragon rdp_draw_filled_triangle for providing the */
    /* conversion algorithm */
    uint64_t edge_coef[4+8+2];
    uint32_t buffer_index = 0;
    const float to_fixed_11_2 = 4.0f;
    const float to_fixed_16_16 = 65536.0f;
    float temp_x, temp_y, temp_z;
    float x1 = v1[0], y1 = v1[1], z1 = v1[2];
    float x2 = v2[0], y2 = v2[1], z2 = v2[2];
    float x3 = v3[0], y3 = v3[1], z3 = v3[2];

    float *c1 = vc1, *c2 = vc2, *c3 = vc3, *temp_c;

    /* sort vertices by Y ascending to find the major, mid and low edges */
    if( y1 > y2 ) 
    { 
        temp_x = x2, temp_y = y2; temp_z = z2 ;temp_c = c2;
        y2 = y1; y1 = temp_y; 
        x2 = x1; x1 = temp_x;
        z2 = z1; z1 = temp_z;
        c2 = c1; c1 = temp_c;
    }
    if( y2 > y3 ) 
    { 
        temp_x = x3, temp_y = y3; temp_z = z3; temp_c = c3;
        y3 = y2; y2 = temp_y; 
        x3 = x2; x2 = temp_x;
        z3 = z2; z2 = temp_z; 
        c3 = c2; c2 = temp_c;
    }
    if( y1 > y2 ) 
    { 
        temp_x = x2, temp_y = y2; temp_z = z2; temp_c = c2;
        y2 = y1; y1 = temp_y; 
        x2 = x1; x1 = temp_x;
        z2 = z1; z1 = temp_z;
        c2 = c1; c1 = temp_c;
    }

    x1 = floorf(x1) + fmodf(x1, 1.0f/4.0f);
    x2 = floorf(x2) + fmodf(x2, 1.0f/4.0f);
    x3 = floorf(x3) + fmodf(x3, 1.0f/4.0f);
    y1 = floorf(y1) + fmodf(y1, 1.0f/4.0f);
    y2 = floorf(y2) + fmodf(y2, 1.0f/4.0f);
    y3 = floorf(y3) + fmodf(y3, 1.0f/4.0f);

    /* calculate Y edge coefficients in 11.2 fixed format */
    uint32_t yh = y1 * to_fixed_11_2;
    uint32_t ym = y2 * to_fixed_11_2;
    uint32_t yl = y3 * to_fixed_11_2;
    
    float dxhdy_f = ( fabs(y3-y1) < MIN_FLOAT ) ? 0 : ( ( x3 - x1 ) / ( y3 - y1 ) );
    float dxmdy_f = ( fabs(y2-y1) < MIN_FLOAT ) ? 0 : ( ( x2 - x1 ) / ( y2 - y1 ) );
    float dxldy_f = ( fabs(y3-y2) < MIN_FLOAT ) ? 0 : ( ( x3 - x2 ) / ( y3 - y2 ) );

    /* calculate inverse slopes in 16.16 fixed format */
    uint32_t dxhdy = dxhdy_f * to_fixed_16_16;
    uint32_t dxmdy = dxmdy_f * to_fixed_16_16;
    uint32_t dxldy = dxldy_f * to_fixed_16_16;

    /* calculate X edge coefficients in 16.16 fixed format */
    uint32_t xh = (x1) * to_fixed_16_16;
    uint32_t xm = (x1) * to_fixed_16_16;
    uint32_t xl = (x2) * to_fixed_16_16;

    /* determine the winding of the triangle */
    int32_t winding = ( x1 * y2 - x2 * y1 ) + ( x2 * y3 - x3 * y2 ) + ( x3 * y1 - x1 * y3 );
    uint32_t flip = (winding > 0 ? 1 : 0 );

    uint32_t r = c1[0]*to_fixed_16_16;
    uint32_t g = c1[1]*to_fixed_16_16;
    uint32_t b = c1[2]*to_fixed_16_16;
    uint32_t a = c1[3]*to_fixed_16_16;

    uint32_t drde;
    uint32_t dgde;
    uint32_t dbde;
    uint32_t dade;
    uint32_t drdx;
    uint32_t dgdx;
    uint32_t dbdx;
    uint32_t dadx;
    uint32_t drdy;
    uint32_t dgdy;
    uint32_t dbdy;
    uint32_t dady;

    uint32_t dzde;
    uint32_t dzdx;
    uint32_t dzdy;

    float inv_z1 = (fabs(z1) < MIN_FLOAT) ? 65532.0f : (1.0f / z1);
    float inv_z2 = (fabs(z2) < MIN_FLOAT) ? 65532.0f : (1.0f / z2);
    float inv_z3 = (fabs(z3) < MIN_FLOAT) ? 65532.0f : (1.0f / z3);

    uint32_t iz1 = ((uint32_t)(inv_z1 * to_fixed_16_16) << 12);
    
    {
        float u2,v2,w2,u3,v3,w3,u4,v4,w4;
        barycentric(x1+1,y1,x1,y1,x2,y2,x3,y3, &u2, &v2, &w2);
        barycentric(x1,y1+1,x1,y1,x2,y2,x3,y3, &u3, &v3, &w3);
        barycentric(x1+dxhdy_f,y1+1,x1,y1,x2,y2,x3,y3, &u4, &v4, &w4);
        
        drdx = ((c1[0]*u2 + c2[0]*v2 + c3[0]*w2) - c1[0]) * to_fixed_16_16;
        dgdx = ((c1[1]*u2 + c2[1]*v2 + c3[1]*w2) - c1[1]) * to_fixed_16_16;
        dbdx = ((c1[2]*u2 + c2[2]*v2 + c3[2]*w2) - c1[2]) * to_fixed_16_16;
        dadx = ((c1[3]*u2 + c2[3]*v2 + c3[3]*w2) - c1[3]) * to_fixed_16_16;

        drdy = ((c1[0]*u3 + c2[0]*v3 + c3[0]*w3) - c1[0]) * to_fixed_16_16;
        dgdy = ((c1[1]*u3 + c2[1]*v3 + c3[1]*w3) - c1[1]) * to_fixed_16_16;
        dbdy = ((c1[2]*u3 + c2[2]*v3 + c3[2]*w3) - c1[2]) * to_fixed_16_16;
        dady = ((c1[3]*u3 + c2[3]*v3 + c3[3]*w3) - c1[3]) * to_fixed_16_16;

        drde = ((c1[0]*u4 + c2[0]*v4 + c3[0]*w4) - c1[0]) * to_fixed_16_16;
        dgde = ((c1[1]*u4 + c2[1]*v4 + c3[1]*w4) - c1[1]) * to_fixed_16_16;
        dbde = ((c1[2]*u4 + c2[2]*v4 + c3[2]*w4) - c1[2]) * to_fixed_16_16;
        dade = ((c1[3]*u4 + c2[3]*v4 + c3[3]*w4) - c1[3]) * to_fixed_16_16;


        /* Calculate depth values */
        dzdx = ((uint32_t)(((inv_z1*u2 + inv_z2*v2 + inv_z3*w2) - inv_z1) * to_fixed_16_16) << 12);
        dzdy = ((uint32_t)(((inv_z1*u3 + inv_z2*v3 + inv_z3*w3) - inv_z1) * to_fixed_16_16) << 12);
        dzde = ((uint32_t)(((inv_z1*u4 + inv_z2*v4 + inv_z3*w4) - inv_z1) * to_fixed_16_16) << 12);
    }

    HFX_RDP_PKT_TRI_NON_SHADE(edge_coef,
                              HFX_RDP_CMD_TRI_SHADE_DEPTH,
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
    HFX_RDP_PKT_TRI_SHADE(edge_coef, r, g, b, a,
                                     drdx, dgdx, dbdx, dadx,
                                     drdy, dgdy, dbdy, dady,
                                     drde, dgde, dbde, dade);
    HFX_RDP_PKT_TRI_DEPTH(edge_coef, iz1, dzdx, dzdy, dzde);

    hfx_cmd_rdp(state, sizeof(edge_coef)/sizeof(uint64_t), edge_coef);
}