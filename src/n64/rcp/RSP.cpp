#include "../N64.hpp"
#include "RSP.hpp"

namespace ultra64
{
    RSP::RSP()
    {
    }

    void RSP::set_n64(void *n64)
    {
        this->n64 = (ultra64::N64 *)n64;
        this->n64->mmu->write_word_raw(SP_STATUS_REG, SP_STATUS_HALT);
    }

    RSP::~RSP()
    {
    }
}
