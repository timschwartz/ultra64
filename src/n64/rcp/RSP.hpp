#pragma once

/** RSP status register */
const uint32_t SP_STATUS_REG = 0x04040010;

const uint32_t SP_DMA_BUSY_REG = 0x04040018;

/** RSP Halt */
const uint32_t  SP_STATUS_HALT = 0x00000001;
/** RSP broke */
const uint32_t  SP_STATUS_BROKE	= 0x00000002;

class N64;

class RSP
{
  public:
    RSP();
    ~RSP();
    void set_n64(void *n64);
    void step();
  private:
    N64 *n64 = nullptr;
};