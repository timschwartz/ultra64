#pragma once

#include <string>

class PIFrom
{
  public:
    PIFrom(std::string filename);
    ~PIFrom();
    const std::byte *get_pointer();
  private:
    size_t filesize;
    std::byte *data;
};
