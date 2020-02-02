﻿# Пример
 CRC32 на Си


Привет всем.

Это простенькую програмку раешил написать увидев тему на васме:https://wasm.in/threads/ischu-programmu-dlja-podscheta-crc32-fajlov.33595/

Вообще в линуксе есть уже встроенная программа в консоле, достаточно например ввести crc32 <FILENAME> и будет подсчет котрольной суммы.

Решил сделать такую-же, дальше можно сделать что-угодно скриптами (сортировка и т.д.).

Но и это ещё не всё, т.к. выкладываю исходники, оформил типо библиотеке, т.е. можно использовать этот алгоритм в своих проектах (Который кстати сходится с винрар и линуксом).

Алгоритм позволяет считать контрольную по частям:

Например нужно посчитать 1Гиг данных, в данном алгоритме выделив буфер например 256Кб., можно потихоньку досчитывать контрольную сумму, сделал комментарии, смотрите исходник:crc_32_lib.cpp.

Может считать контрольную сумму как файла целиком, так и буфера памяти, как использовать:

Функция:

uint32_t crc32_file(const char* file_name)

Функция посчитает CRC32 файла, файл делится порциями по MAX_BUF_SIZE по умолчанию 8КБ, можно увеличить.

Входные параметры:

file_name - Имя файла.

Выходные параметры:

uint32_t CRC32

Функция:

uint32_t crc32_ram(uintptr_t addr, uint32_t size)

Входные параметры:

addr - Адрес буфера, которого нужно посчитать.

size - Размер буфера.

Выходные параметры:

uint32_t CRC32

Пример использования в исходном коде:crc32.cpp

В папке Realese собранный проект CRC32.exe

Пример использования, команда:CRC32.exe <FILE_NAME>