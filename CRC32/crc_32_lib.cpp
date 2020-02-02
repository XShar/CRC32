#include <stdint.h>
#include <stdio.h>
#include "crc32_lib.h"

#define MAX_BUF_SIZE  8*1024U
static uint8_t buf_to_crc[MAX_BUF_SIZE];

/*
Основной расчет CRC32, данный лагоритм посволяет расчитывать CRC32 по кусочкам.
Например если вы считаете файл 1Гиг, можно его раздробить по 512Кб и считать кусочками.
Как это сделать смотрите пример функции, расчет файла в этом исходнике.
*/

/*
Входные данные:
buffer - Данные, которые нужно посчитать.
len - Длина данных.
crc_d - Предыдущий CRC, если расчет порциями (Начальное значение должно-быть 0xFFFFFFFFUL).

Возвращает, саму CRC32.
*/
uint32_t crc32(uint8_t* buffer, uint32_t len, uint32_t crc_d) {

	uint32_t crc_table[256];
	uint32_t crc;
	int32_t i, j;

	for (i = 0; i < 256; i++) {
		crc = i;
		for (j = 0; j < 8; j++)
			crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;

		crc_table[i] = crc;
	}

	while (len--)
		crc_d = crc_table[(crc_d ^ *buffer++) & 0xFF] ^ (crc_d >> 8);

	return crc_d;
}

/*
Функция посчитает CRC32 файла, файл делится порциями по MAX_BUF_SIZE по умолчанию 8КБ, можно увеличить.
Входные параметры:
file_name - Имя файла.

Выходные параметры:
uint32_t CRC32
*/
uint32_t crc32_file(const char* file_name) {

	//Начальное значение CRC
	uint32_t crc = 0xFFFFFFFFUL;
	errno_t error = 0;

	FILE* src = fopen(file_name, "rb");
	if (src == NULL) {
		printf("%s- file not found\n", src);
		return 0;
	}

	size_t total_bytes = 0;

	while (!feof(src)) {
		size_t read_byte = fread(buf_to_crc, 1, MAX_BUF_SIZE, src);
		if ((read_byte != MAX_BUF_SIZE) && (!feof(src))) {
			printf("\nError read file\n");
			return 0;
		}
		crc = crc32(buf_to_crc, read_byte, crc);
		total_bytes += MAX_BUF_SIZE;
	}

	crc ^= 0xFFFFFFFFUL;
	fclose(src);

	return crc;
}

/*
Посчитает CRC32 в памяти.
Входные параметры:
addr - Адрес буфера, которого нужно посчитать.
size - Размер буфера.
*/
uint32_t crc32_ram(uintptr_t addr, uint32_t size) {

	uint32_t crc = 0xFFFFFFFFUL;
	uint32_t size_read = size;
	uint32_t addr_src = addr;

	//Подчет CRC32 в памяти частями
	while (size_read > MAX_BUF_SIZE) {
		crc = crc32((uint8_t*)addr_src, MAX_BUF_SIZE, crc);
		addr_src += MAX_BUF_SIZE;
		size_read = size_read - MAX_BUF_SIZE;
	}

	//Подчет CRC32 в памяти (Остаток)
	if ((size_read != 0) && (size_read <= MAX_BUF_SIZE)) {
		crc = crc32(buf_to_crc, size_read, crc);
	}

	crc ^= 0xFFFFFFFFUL;

	return crc;
}