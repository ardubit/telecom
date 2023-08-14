/*
Розрахунок циклічного надлишкового коду
CRC (Cyclic Redundancy Code)
gcc -o lab3_crc lab3_crc.c
./lab3_crc
========================================
Необхідно реалізувати:
- Початкова послідовність
- Обрахунок CRC
- Початкова послідовність + CRC
- Обрахунок CRC з отриманої послідовності

CRC-16/T10-DIF
XOR з поліномом x16 + x15 + x11 + x9 + x8 + x7 + x5 + x4 + x2 + x1 + х0
0001 1000 1011 1011 0111 = 0x18BB7

XOR якщо A!=B, B!=A то Y=1
*/

#include <stdint.h>
#include <stdio.h>

#define DEGREE 16 // Ступінь поліному
#define POLYNOM 0x18BB7

const uint16_t mask_SingleBit_Error = 0x1;  // 0001
const uint16_t mask_PairBit_Error = 0x3;    // 0011
const uint16_t mask_NonPairBit_Error = 0x7; // 0111
const uint16_t mask_First2Bytes = 0xFFFF;

// Вивід у термінал hex-to-bin значень
void printBinary(uint32_t value) {
  for (int i = sizeof(value) * 8 - 1; i >= 0; i--) {
    printf("%u", (value >> i) & 1);
    if (i % 4 == 0) {
      printf(" ");
    }
  }
  printf("\n\n");
}

// Функція обчислення CRC
uint16_t getCRC16(uint16_t data) {
  // Ініціалізація CRC-регістру вхідними даними
  uint16_t crcREG = data;
  uint32_t maskMSB = 1 << (DEGREE - 1);

  printf("Лістинг обчислення CRC\n");
  printf("Початкове значення CRC-регістру: 0x%X\n", crcREG);

  for (int i = 0; i < DEGREE; i++) {
    if ((crcREG & maskMSB) != 0) {
      // Якщо 15-й MSB-біт, то зсув вмісту регістру
      // на один біт вліво за межі регістру та XOR з поліномом
      crcREG = crcREG << 1;
      crcREG = crcREG ^ (POLYNOM);
      printf("MSB! i: %d\t crc: 0x%X polynom: 0x%X\n", i, crcREG, POLYNOM);
    } else {
      crcREG <<= 1; // Шукаємо далі
    }
  }
  printf("\n");

  return crcREG;
}

int main() {
  // Початкова послідовність даних
  uint32_t data = 0xFFFF; // F=1111

  printf("Передавач\n");
  printf("Початкова послідовність даних: 0x%X\n", data);
  printBinary(data);

  printf("Поліном: 0x%X, cтупінь (розмір CRC-регістру): %d\n", POLYNOM, DEGREE);
  printBinary(POLYNOM);

  printf("Послідовність з доповненням: 0x%X\n", data = data << DEGREE);
  printBinary(data);

  // Обчислення CRC
  data = data >> DEGREE; // Дані у 3,4-му байті [data, data, crc, crc]
  uint16_t crc = getCRC16(data);

  printf("Значення CRC [Передавач]: 0x%X\n", crc);
  printBinary(crc);

  // Додавання CRC до вхідної послідовності
  uint32_t dataWithCRC = (data << DEGREE) | crc;

  // Вихідна послідовність
  printf("Вихідна послідовність: 0x%X\n", dataWithCRC);
  printBinary(dataWithCRC);

  // Тестування CRC
  // =======================================
  // dataWithCRC = 0xFFFFAAAA; // Тестові дані

  printf("Приймач\n");
  printf("Перевірка алгоритму CRС \n\n");

  printf("Отримана послідовність [Приймач]: 0x%X\n", dataWithCRC);
  printBinary(dataWithCRC);

  data = dataWithCRC >> DEGREE;
  printf("Дані [Приймач]: 0x%X\n", data);
  printBinary(data);

  crc = dataWithCRC & mask_First2Bytes; // Отримане значення CRC у 1,2-му байті
  printf("Значення CRC [Приймач]: 0x%X\n", crc);
  printBinary(crc);

  // Пошкодження біту даних
  // =======================================
  data = data ^ mask_SingleBit_Error;
  printf("Пошкодження блоку даних [Приймач]: 0x%X\n", data);
  printBinary(data);

  uint16_t crcTest = getCRC16(data);
  printf("Розраховане значення CRC [Приймач]: 0x%X\n", crcTest);
  printBinary(crcTest);

  printf("Тест CRC:\n");
  if (crcTest == crc) {
    printf("Перевірка CRC пройшла успішно, DONE!\n\n");
  } else {
    printf("Перевірка CRC неуспішна, FAIL!\n");
    printf("Дані пошкоджено!\n\n");
  }

  return 0;
}
