#include <stdint.h>
#include <stdio.h>

const uint32_t polynom = 0x18BB7;

int getDegree(uint32_t polynomial) {
    int degree = 0;
    while (polynomial != 0) {
        degree++;
        polynomial >>= 1;
    }
    return degree - 1;
}

void appendZeros(uint64_t *data, int degree) {
    int numZeros = degree;
    *data <<= numZeros;
}

void toBinary(unsigned int value) {
  for (int i = sizeof(value) * 8 - 1; i >= 0; i--) {
    printf("%d", (value >> i) & 1);
    if (i % 4 == 0) {
      printf(" ");
    }
  }
  printf("\n --- \n");
}

uint16_t getCRC(uint8_t data, const uint32_t *polynom) {
  uint16_t crc = 0xFFFF;
  for (int i = 0; i < sizeof(data) * 8; i++) {
    if ((crc ^ data) & 0x01) {
      crc = (crc >> 1) ^ *polynom;
    } else {
      crc >>= 1;
    }
    data >>= 1;
  }
  return crc;
}

int main() {
  uint64_t data = 0xFF;

  printf("Початкова послідовність: 0x%llX\n", data);
  toBinary(data);

  appendZeros(&data, getDegree(polynom));

  printf("Початкова послідовність з доповненням: 0x%llX\n", data);
  toBinary(data);

  printf("Поліном: 0x%X \t Ступінь поліному %d\n", polynom, getDegree(polynom));
  toBinary(polynom);

  uint16_t crcResult = getCRC(data, &polynom);

  printf("Значення CRC (Передавач): 0x%X\n", crcResult);
  toBinary(crcResult);

  uint64_t data_CRC = (data << 16) | crcResult;

  printf("Вихідна послідовність: 0x%llX\n", data_CRC);
  toBinary(data_CRC);

  printf("\nТрансмішн ..- --. ..- --. .-- ..- --. ..- --. .--\n\n");

  uint16_t crc = getCRC(data_CRC, &polynom);

  printf("Значення CRC (Прймач): 0x%X\n", crc);
  toBinary(crc);

  if (crc == 0) {
    printf("Перевірка CRC пройшла успішно! CRC = 0\n");
  } else {
    printf("Перевірка CRC неуспішна. CRC != 0\n");
  }

  return 0;
}
