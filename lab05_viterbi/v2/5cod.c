/*
Дослідження декодера Вітербі. Кодер
gcc -o 5cod 5cod.c
./5cod
========================================
Coder 3 registers
G1 7oct G2 5oct
Polynomials
7oct 0o7 = 111 = 0x7 -> GP1 = x^2 + x^1 + x^0
5oct 0o5 = 101 = 0x5 -> GP2 = x^2 + x^0
XOR якщо A!=B, B!=A то Y=1
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE_REG 3 // Довжина регістру
#define N_OUTS 2 // Кількість виходів кодувальника

// --- INIT HERE ----
#define SIZE_DATA_BITS 6 // Довжина послідовності
#define SIZE_DATA (SIZE_DATA_BITS + SIZE_REG)

#define INIT_REG 0x0
#define GP2 ((reg >> 2) ^ (reg >> 1) ^ reg) & 0x1
#define GP1 ((reg >> 2) ^ reg) & 0x1

void fillDataWithRandom(uint8_t *arr, uint8_t dataSize) {
  srand(time(NULL));
  for (size_t i = 0; i < dataSize; i++) {
    arr[i] = (uint8_t)rand() % 2;
    if (i >= (dataSize - SIZE_REG)) {
      arr[i] = 0x0;
    }
  }
}

void printData(uint8_t *arr, uint8_t length) {
  for (size_t i = 0; i < length; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void toBinary(uint8_t value) {
  for (int i = (sizeof(value) * 8 - 1) / 2; i >= 0; i--) {
    printf("%d", (value >> i) & 1);
    if (i % 4 == 0) {
      printf(", ");
    }
  }
}

int coderConv(uint8_t *dataIn, uint8_t *dataOut) {
  printf("Згортковий кодер \n");
  uint8_t bufOut1[SIZE_DATA];
  uint8_t bufOut2[SIZE_DATA];
  uint8_t reg = (uint8_t)INIT_REG;
  for (size_t i = 0; i < SIZE_DATA; i++) {
    reg = (reg << 1) | dataIn[i];
    // Вихідне значення визначається як XOR по бітам
    bufOut1[i] = GP1;
    bufOut2[i] = GP2;
  }

  printf("Вихід N1:\n");
  for (size_t i = 0; i < SIZE_DATA; i++) {
    printf("%d ", bufOut1[i]);
    // toBinary(bufOut1[i]);
  }
  printf("\n");

  printf("Вихід N2:\n");
  for (size_t i = 0; i < SIZE_DATA; i++) {
    printf("%d ", bufOut2[i]);
  }

  printf("\n---\n");
  printf("Кодована послідовність [n1,n2][]... : \n");
  uint8_t j = 0;
  for (size_t i = 0; i < SIZE_DATA; i++) {
    dataOut[j] = bufOut1[i];
    dataOut[j + 1] = bufOut2[i];
    printf("[%d %d] ", dataOut[j], dataOut[j + 1]);
    j += 2;
    // if (j % (SIZE_DATA) == 0) {
    //   printf("\n");
    // }
  }
  printf("\n");

  return 0;
}

int main(int argc, char const *argv[]) {
  uint8_t dataInit[SIZE_DATA] = {0, 1, 0, 1, 1, 1};
  // uint8_t dataInit[SIZE_DATA];
  uint8_t dataOut[SIZE_DATA * N_OUTS];

  printf("Початкова послідовність: \n");
  // fillDataWithRandom(dataInit, (uint8_t)SIZE_DATA);
  printData(dataInit, (uint8_t)SIZE_DATA);
  printf("---\n");

  coderConv(dataInit, dataOut);

  return 0;
}