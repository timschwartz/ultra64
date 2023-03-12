#include "../N64.hpp"
#include "RSP.hpp"

RSP::RSP()
{
}

void RSP::set_n64(void *n64)
{
    this->n64 = (N64 *)n64;
}

RSP::~RSP()
{
}
