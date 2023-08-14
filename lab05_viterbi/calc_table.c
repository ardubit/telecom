/*
Розрахунок таблиці
- Дослідження згорткового коду
- Дослідження декодера Вітербі
gcc -o calc_table calc_table.c
./calc_table
========================================
Необхідно реалізувати:
-
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define AMOUNT_OF_STATES 8
#define GP1 ((reg >> 3) ^ (reg >> 1) ^ reg) & 0x1
#define GP2 ((reg >> 3) ^ (reg >> 2) ^ (reg >> 1) ^ reg) & 0x1

void toBinary(uint8_t value) {
  for (int i = (sizeof(value) * 8 - 1) / 2; i >= 0; i--) {
    printf("%d", (value >> i) & 1);
    if (i % 4 == 0) {
      printf(", ");
    }
  }
}

void calc(uint8_t *dataIn) {
  printf("\nКалькулятор таблиці");
  printf("\n---\n");
  uint8_t n1[AMOUNT_OF_STATES];
  uint8_t n2[AMOUNT_OF_STATES];
  uint8_t reg;

  printf("Поточний стан регістру [Zo, X, X, X]\n");

  // Якщо Z0 отримує - 0
  printf("Z0 отримує '0'\nВихідне значення кодувальника n1(XOR біт 3,1,0) n2("
         "XOR біт 3-0)\n");
  for (size_t i = 0; i < AMOUNT_OF_STATES; i++) {
    reg = dataIn[i] & 0xF;
    n1[i] = GP1;
    n2[i] = GP2;
    toBinary(reg);
    printf("Z0=0, [n1,n2] %d %d \n", n1[i], n2[i]);
  }
  printf("\n---\n");

  // Якщо Z0 отримує - 1
  printf("Z0 отримує '1'\nВихідне значення кодувальника n1(XOR біт 3,1,0) n2("
         "XOR біт 3-0)\n");
  for (size_t i = 0; i < AMOUNT_OF_STATES; i++) {
    reg = (dataIn[i] & 0xF) | 0x8;
    n1[i] = GP1;
    n2[i] = GP2;
    toBinary(reg);
    printf("Z0=1, [n1,n2] %d %d \n", n1[i], n2[i]);
  }
}

int main(int argc, char const *argv[]) {
  uint8_t REG[] = {0, 1, 2, 3, 4, 5, 6, 7};
  calc(REG);

  return 0;
}
