#include <iostream>
#include "../ultra64.hpp"

class cliUltra64 :  public ultra64
{
};

void display_usage(std::string name)
{
    std::cout << "Usage:\n" << std::endl;
    std::cout << name << " game.rom" << std::endl;
}

int main(int argc, char *argv[])
{
    auto u64 = new cliUltra64();
    std::cout << "Loaded config from " << u64->config_load() << std::endl;

    if(argc < 2)
    {
        display_usage(argv[0]);
        return 1;
    }

    for(auto i = 1; i < argc; i++)
    {
        std::string opt = argv[i];
        if(opt[0] == '-' && opt[1] == '-')
        {
            opt = opt.substr(2, opt.size() - 1);

            if(!u64->is_valid_option(opt))
            {
                std::cout << "Unknown option: --" << opt << std::endl;
                return 1;
            }

            if(!u64->has_args(opt)) u64->option_set(opt, "true");
            else
            {
                i++;
                u64->option_set(opt, argv[i]);
            }
        }
        else
        {
            u64->option_set("rom", opt);
        }
    }

    std::cout << u64->config_get() << std::endl;

    u64->n64_reset();

    return 0;
}
