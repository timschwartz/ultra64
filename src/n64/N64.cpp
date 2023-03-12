#include "N64.hpp"
#include "PIFrom.hpp"

void N64::pif_rom_load(std::string filepath)
{
    PIFrom pifrom(filepath);

    uint32_t count = 0;
    uint32_t *p = (uint32_t *)pifrom.get_pointer();

    while(count < 0x7C0)
    {
        this->mmu.write_word_raw(0x1FC00000 + count, *p++);
        count += 4;
    }

    std::cout << "Loaded " << filepath << std::endl;
}
