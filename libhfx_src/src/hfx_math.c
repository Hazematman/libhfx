#include "hfx.h"

HFXfixed hfx_addx(HFXfixed a, HFXfixed b)
{
    return a + b;
}

HFXfixed hfx_subx(HFXfixed a, HFXfixed b)
{
    return a - b;
}

HFXfixed hfx_mulx(HFXfixed a, HFXfixed b)
{
    return (HFXfixed)(((int64_t)a * (int64_t)b) >> 16);
}

HFXfixed hfx_divx(HFXfixed a, HFXfixed b)
{
    return (HFXfixed)(((int64_t)a << 16) / ((int64_t)b));
}