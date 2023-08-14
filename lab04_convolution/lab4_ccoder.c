/*
Дослідження згорткового коду
gcc -o lab4_ccoder lab4_ccoder.c
./lab4_ccoder
========================================
Необхідно реалізувати:
-
Polynomials
13oct 0o13 = 1011 = 0xB -> GP1 = x^3 + x^1 + x^0
17oct 0o17 = 1111 = 0xF -> GP2 = x^3 + x^2 + x^1 + x^0
XOR якщо A!=B, B!=A то Y=1
XOR: y = x1 ^ x2
x1 -->(+)--> y
x2 -->
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE_DATA 30
#define N_OUTS 2
#define INIT_REG 0x0
#define GP1 ((reg >> 3) ^ (reg >> 1) ^ reg) & 0x1
#define GP2 ((reg >> 3) ^ (reg >> 2) ^ (reg >> 1) ^ reg) & 0x1

void fillDataWithRandom(uint8_t *arr, uint8_t size) {
  srand(time(NULL));
  for (size_t i = 0; i < size; i++) {
    arr[i] = (uint8_t)rand() % 2;
    if (i >= (size - 5)) {
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
    // GP1 1011 = 0xB Вихідне значення визначається як XOR по 3 бітам 3, 1, 0
    bufOut1[i] = GP1;
    // GP2 1111 = 0xF Вихідне значення визначається як XOR по 4 бітам 3, 2, 1, 0
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
    printf("%d %d ", dataOut[j], dataOut[j + 1]);
    j += 2;
    if (j % (SIZE_DATA / 2) == 0) {
      printf("\n");
    }
  }
  printf("\n");

  return 0;
}

int main(int argc, char const *argv[]) {
  // uint8_t dataInit[SIZE_DATA] = {1, 1, 1, 1, 0, 0, 0, 0, 0};
  uint8_t dataInit[SIZE_DATA];
  uint8_t dataOut[SIZE_DATA * N_OUTS];

  printf("Початкова послідовність: \n");
  fillDataWithRandom(dataInit, (uint8_t)SIZE_DATA);
  printData(dataInit, (uint8_t)SIZE_DATA);
  printf("---\n");

  coderConv(dataInit, dataOut);

  return 0;
}

// int coderConv(uint8_t *dataIn, uint8_t *dataOut) {
//   printf("Згортковий кодер \n");
//   uint8_t bufOut1[SIZE_DATA];
//   uint8_t bufOut2[SIZE_DATA];
//   uint8_t reg = (uint8_t)INIT_REG;
//   for (size_t i = 0; i < SIZE_DATA; i++) {
//     bufOut1[i] = (reg ^ GP1);
//     bufOut2[i] = (reg ^ GP2);
//     reg = (reg << 1) | dataIn[i];
//   }

//   printf("Вміст вихідного буферу #1: \n");
//   for (size_t i = 0; i < SIZE_DATA; i++)
//     toBinary(bufOut1[i]);
//   printf("\n");
//   printf("Вміст вихідного буферу #2: \n");
//   for (size_t i = 0; i < SIZE_DATA; i++)
//     toBinary(bufOut2[i]);
//   printf("\n");

//   for (size_t i = 0; i < SIZE_DATA; i++) {
//     // GP1 1011 = 0xB
//     // Вихідне значення визначається як XOR по 3 бітам 0, 1, 3
//     bufOut1[i] = ((bufOut2[i] >> 3) ^ (bufOut2[i] >> 1) ^ bufOut2[i]) &
//     0x1;

//     // GP2 1111 = 0xF
//     // Вихідне значення визначається як XOR по 4 бітам 0, 1, 2, 3
//     bufOut2[i] = ((bufOut2[i] >> 3) ^ (bufOut2[i] >> 2) ^ (bufOut2[i] >> 1)
//     ^
//                   bufOut2[i]) &
//                  0x1;
//   }

//   printf("Вихід N1: ");
//   for (size_t i = 0; i < SIZE_DATA; i++)
//     printf("%d ", bufOut1[i]);
//     // toBinary(bufOut1[i]);
//   printf("\n");

//   printf("Вихід N2: ");
//   for (size_t i = 0; i < SIZE_DATA; i++)
//     printf("%d ", bufOut2[i]);

//   printf("\n---\n");
//   printf("Кодована послідовність [n1,n2][]...[] : \n");
//   for (size_t i = 0; i < SIZE_DATA; i++) {
//     uint8_t j = 0;
//     dataOut[j] = bufOut1[i];
//     dataOut[j + 1] = bufOut2[i];
//     printf("%d %d ", dataOut[j], dataOut[j + 1]);
//     j += 2;
//   }
