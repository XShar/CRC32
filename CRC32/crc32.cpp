#include "crc32_lib.h"
#include <iostream>

int  main(int argc, char* argv[])
{
    uint32_t crc = 0;
    if (argc == 2) {
        crc = crc32_file(argv[1]);
        printf("Filename %s CRC32 = 0x%x \n", argv[1], crc);
    }
    else {
        printf("Error args, example crc32 <filename> \n");
        return (-1);
    }

//Проверка CRC подсчета в памяти
#if 0
    static uint8_t tst_str[] = "cmd.exe\0";    
    printf("Test to crc_to_ram, test example:%s \n", tst_str);
    crc = crc32_ram((uintptr_t)tst_str, strlen((char*)tst_str));
    printf("CRC32 = 0x%x\n", crc);
#endif
}