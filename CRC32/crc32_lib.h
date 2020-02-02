#pragma once
#include <stdint.h>
uint32_t crc32(uint8_t* buffer, uint32_t len, uint32_t crc_d);
uint32_t crc32_file(const char* file_name);
uint32_t crc32_ram(uintptr_t addr, uint32_t size);
