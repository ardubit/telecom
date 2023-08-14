#include <stdint.h>
#include <stdio.h>

uint16_t data = 0x1234;
uint16_t crcResult = 0xABCD;

uint32_t messageWithCRC = ((data << 16) | crcResult);

// Перевірка результату
printf("messageWithCRC: 0x%X \n", messageWithCRC);
