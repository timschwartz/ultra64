#include "rsp.hpp"

namespace ultra64
{
    rsp::rsp(MMU &mmu)
    {
        this->mmu = &mmu;
        this->mmu->write_word(SP_STATUS_REG, SP_STATUS_HALT);
    }

    rsp::~rsp()
    {
    }
}
