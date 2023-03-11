#include <iostream>
#include "../ultra64.hpp"

class cliUltra64 :  public ultra64
{
};

int main(int argc, char *argv[])
{
    auto u64 = new cliUltra64();
    u64->load_config();

    return 0;
}
