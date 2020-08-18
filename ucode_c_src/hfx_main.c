#include <hfx_cmds.h>
#include <stdint.h>
#include <stdbool.h>

#define HFX_SET_XBUS_DMEM_DMA 0x0002

#define HFX_READ_REG(reg) hfx_registers[(reg)>>2]
#define HFX_WRITE_REG(reg, value) hfx_registers[(reg)>>2] = (value)
// TODO fix masking here
#define HFX_READ_RB(offset) hfx_rb_buffer[((rb_start+((offset)<<2))&0x3FFF)>>2]

extern uint32_t volatile hfx_registers[HFX_REGISTER_SPACE_SIZE];
volatile uint32_t hfx_rb_buffer[256] __attribute__((aligned(8)));
volatile uint32_t hfx_rdb_buffer[64] __attribute__((aligned(8)));
static uint32_t hfx_rb_end;

void hfx_check_rb_ptr()
{
    uint32_t status;
    uint32_t rb_end = HFX_READ_REG(HFX_REG_RB_END);

    if(hfx_rb_end != rb_end)
    {
        /* Setup DMA transfer registers */
        // TODO only copy data we need to
        asm volatile ("mtc0 %0, $0\n"
                      "mtc0 %1, $1\n"
                      "mtc0 %2, $2"
                      :: "r"(hfx_rb_buffer),
                         "r"(HFX_READ_REG(HFX_REG_RB_ADDR)),
                         "r"(HFX_READ_REG(HFX_REG_RB_SIZE)-1));

        /* Wait for DMA to finish copying data */
        do
        {
            asm volatile ("mfc0 %0, $6" : "=r"(status));
        } while(status != 0);

        /* Update cached rb end value with updated value */
        hfx_rb_end = rb_end;
    }

    return;
}

void hfx_cmd_dma(bool write, uint32_t rb_start)
{
    uint32_t dma_dmem_addr = HFX_READ_RB(1);
    uint32_t dma_mem_addr = HFX_READ_RB(2);
    uint32_t dma_size = HFX_READ_RB(3);
    uint32_t status;

    /* Setup DMA transfer registers */
    asm volatile ("mtc0 %0, $0\n"
                  "mtc0 %1, $1"
                  :: "r"(dma_dmem_addr),
                     "r"(dma_mem_addr));
    if(write)
    {
        asm volatile ("mtc0 %0, $3" :: "r"(dma_size));
    }
    else
    {
        asm volatile ("mtc0 %0, $2" :: "r"(dma_size));
    }

    /* Wait for DMA to finish copying data */
    do
    {
        asm volatile ("mfc0 %0, $6" : "=r"(status));
    } while(status != 0);


    return;
}

void hfx_init_rdp()
{
    /* Set RDP to load using XBUS DMA (DMA from DMEM) */
    /* Set RDP start and end pointer to statically allocated buffer */
    asm volatile("mtc0 %0, $11\n"
                 "mtc0 %1, $8\n"
                 "mtc0 %2, $9"
                 :: "r"(HFX_SET_XBUS_DMEM_DMA),
                    "r"(hfx_rdb_buffer),
                    "r"(hfx_rdb_buffer));
}

int main()
{
    /* Set REG RB_END to zero */
    hfx_rb_end = 0;

    hfx_init_rdp();

    for(;;)
    { 
        hfx_check_rb_ptr();
        uint32_t rb_start = HFX_READ_REG(HFX_REG_RB_START);
        if(rb_start == hfx_rb_end)
            continue;

        uint32_t cmd = hfx_rb_buffer[rb_start>>2];
        switch(cmd & 0xFF)
        {
            case HFX_CMD_NOP:
                rb_start += 4;
                break;
            case HFX_CMD_INT:
                asm volatile ("break");
                rb_start += 4;
                break;
            case HFX_CMD_DMA:
                hfx_cmd_dma(cmd>>8, rb_start);
                rb_start += 16;
                break;
            case HFX_CMD_SET_DISPLAY:
                rb_start += 8;
                break;
        }

        HFX_WRITE_REG(HFX_REG_RB_START, rb_start & 0x3FFF);
    }

    return 0;
}