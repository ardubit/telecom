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
*/

#include <stdint.h>
#include <stdio.h>

#define POLY_DEG 16 // getDegree(polynom)

uint16_t mask_SingleBit_Error = 0x1;  // 0001
uint16_t mask_PairBit_Error = 0x3;    // 0011
uint16_t mask_NonPairBit_Error = 0x7; // 0111
uint16_t mask_First2Bytes = 0xFFFF;
uint32_t polynom = 0x18BB7;

// Визначення ступеня CRC-поліному
// int getDegree(uint32_t polynom) {
//   int degree = 0;
//   while (polynom != 0) {
//     degree++;
//     polynom >>= 1;
//   }
//   return degree - 1;
// }

// Функція, що додає нулі праворуч до вхідних даних – виконує зсув
// void appendZeros(uint32_t *data, int degree) {
//   int numZeros = degree;
//   *data = *data << numZeros; // *data <<= numZeros;
// }

// Вивід у термінал hex-bin значень
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
uint16_t getCRC16(uint16_t data, uint32_t *poly) {
  // Обрізка поліному до 16 біт (CRC-16/T10-DIF)
  // *poly = *poly & mask_first16Bits;

  // Початкове значення CRC-регістру
  uint16_t crcReg = data;
  // Маска для 15-го розряду
  uint32_t maskMSB = 1 << (POLY_DEGREE - 1);

  printf("Лістинг обчислення CRC\nПочаткове значення CRC-регістру: 0x%X\n",
         crcReg);

  for (int i = 0; i < POLY_DEGREE; i++) {
    if ((crcReg & maskMSB) != 0) {
      // Якщо встановлено 15-й MSB-біт, то зсув вмісту регістру на 1 вліво за
      // межі регістру та XOR з поліномом
      crcReg = crcReg << 1;
      crcReg = crcReg ^ (*poly);
      printf("MSB! i: %d\t crc: 0x%X polynom: 0x%X\n", i, crcReg, *poly);
    } else {
      crcReg <<= 1; // Шукаємо далі
    }
  }
  printf("\n");

  return crcReg;
}

int main() {
  // Початкова послідовність даних
  uint32_t data = 0xFFFF; // F=1111

  printf("*** Передавач ***\n");
  printf("Початкова послідовність [Дані]: 0x%X\n", data);
  printBinary(data);

  printf("Поліном: 0x%X Ступінь (розмір CRC-регістру): %d\n", polynom, POLY_DEGREE);
  printBinary(polynom);

  printf("Початкова послідовність з доповненням: 0x%X\n", data = data << POLY_DEGREE);
  printBinary(data); // 1111 1111 1111 1111 0000 0000 0000 0000

  // Обчислення CRC
  data = data >> POLY_DEGREE; // Дані у 3,4-му байті [data, data, crc, crc]
  uint16_t crcResult = getCRC16(data, &polynom);

  printf("Значення CRC [Передавач]: 0x%X\n", crcResult);
  printBinary(crcResult);

  // Додавання CRC до вхідної послідовності
  // uint64_t data_CRC = (data << 16) | crcResult;
  uint32_t data_withCRC = (data << POLY_DEGREE) | crcResult;

  // Вихідна послідовність
  printf("Вихідна послідовність: 0x%X\n", data_withCRC);
  printBinary(data_withCRC);

  // Тест CRC
  // =======================================
  // data_withCRC = 0xFFFFAAAA; // Тестові дані
  printf("*** Приймач ***\nПеревірка алгоритму CRС\n\n");
  printf("Отримана послідовність [Приймач]: 0x%X\n", data_withCRC);
  printBinary(data_withCRC);

  data = data_withCRC >> POLY_DEGREE; // Дані у 3,4-му байті [data, data, crc, crc]
  printf("Дані [Приймач]: 0x%X\n", data);
  printBinary(data);

  uint16_t crc =
      data_withCRC & mask_First2Bytes; // Отримане значення CRC у 1,2-му байті
  printf("Значення CRC [Приймач]: 0x%X\n", crc);
  printBinary(crc);

  // Пошкодження біту даних
  data = data ^ mask_SingleBit_Error;
  printf("Пошкодження блоку даних [Приймач]: 0x%X\n", data);
  printBinary(data);

  // Обрахунок CRC з прийнятої послідовності
  uint16_t crcTest = getCRC16(data, &polynom);
  printf("Обраховане значення CRC [Приймач]: 0x%X\n", crcTest);
  printBinary(crcTest);

  printf("ТЕСТ CRC:\n");
  if (crcTest == crc) {
    printf("-> Перевірка CRC пройшла успішно, DONE!\n\n");
  } else {
    printf("-> Перевірка CRC неуспішна, FAIL!\n");
    printf("-> Дані пошкоджено!\n\n");
  }

  return 0;
}
