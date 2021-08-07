#include "N64.hpp"
#include "PIFrom.hpp"

using namespace ultra64;

void N64::load_pif_rom(std::string filepath)
{
    ultra64::PIFrom pifrom(filepath);

    uint32_t count = 0;
    uint32_t *p = (uint32_t *)pifrom.get_pointer();

    ultra64::map_memory(this->mmu, "pif_rom", 0x1FC00000, 0x7C0, 0x7C0, new std::byte[0x7C0], nullptr, nullptr);

    while(count < 0x7C0)
    {
        this->mmu->write_word_raw(0x1FC00000 + count, *p++);
        count += 4;
    }

    std::cout << "Loaded " << filepath << std::endl;
}
