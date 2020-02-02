#include <stdint.h>
#include <stdio.h>
#include "crc32_lib.h"

#define MAX_BUF_SIZE  8*1024U
static uint8_t buf_to_crc[MAX_BUF_SIZE];

/*
�������� ������ CRC32, ������ �������� ��������� ����������� CRC32 �� ��������.
�������� ���� �� �������� ���� 1���, ����� ��� ���������� �� 512�� � ������� ���������.
��� ��� ������� �������� ������ �������, ������ ����� � ���� ���������.
*/

/*
������� ������:
buffer - ������, ������� ����� ���������.
len - ����� ������.
crc_d - ���������� CRC, ���� ������ �������� (��������� �������� ������-���� 0xFFFFFFFFUL).

����������, ���� CRC32.
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
������� ��������� CRC32 �����, ���� ������� �������� �� MAX_BUF_SIZE �� ��������� 8��, ����� ���������.
������� ���������:
file_name - ��� �����.

�������� ���������:
uint32_t CRC32
*/
uint32_t crc32_file(const char* file_name) {

	//��������� �������� CRC
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
��������� CRC32 � ������.
������� ���������:
addr - ����� ������, �������� ����� ���������.
size - ������ ������.
*/
uint32_t crc32_ram(uintptr_t addr, uint32_t size) {

	uint32_t crc = 0xFFFFFFFFUL;
	uint32_t size_read = size;
	uint32_t addr_src = addr;

	//������ CRC32 � ������ �������
	while (size_read > MAX_BUF_SIZE) {
		crc = crc32((uint8_t*)addr_src, MAX_BUF_SIZE, crc);
		addr_src += MAX_BUF_SIZE;
		size_read = size_read - MAX_BUF_SIZE;
	}

	//������ CRC32 � ������ (�������)
	if ((size_read != 0) && (size_read <= MAX_BUF_SIZE)) {
		crc = crc32(buf_to_crc, size_read, crc);
	}

	crc ^= 0xFFFFFFFFUL;

	return crc;
}