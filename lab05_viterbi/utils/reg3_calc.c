/*
Розрахунок таблиці
gcc -o reg3_calc reg3_calc.c
./reg3calc
========================================
Coder 3 registers
G1 7oct G2 5oct
Polynomials
7oct 0o7 = 111 = 0x7 -> GP1 = x^2 + x^1 + x^0
5oct 0o5 = 101 = 0x5 -> GP2 = x^2 + x^0
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE_REG 3 // Довжина регістру
#define AMOUNT_OF_STATES 4
#define GP1 ((reg >> 2) ^ (reg >> 1) ^ reg) & 0x1
#define GP2 ((reg >> 2) ^ reg) & 0x1

void toBinary(uint8_t value) {
  for (int i = SIZE_REG-1; i >= 0; i--) {
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

  printf("Поточний стан регістру [Zo, X, X], m-регістру %d\n", SIZE_REG);
  printf("Вихідне значення кодувальника n1(XOR біт 2,1,0) n2("
         "XOR біт 2,0)\n\n");

  // Якщо Z0 отримує - 0
  printf("Z0 отримує '0'\n");
  for (size_t i = 0; i < AMOUNT_OF_STATES; i++) {
    reg = dataIn[i] & 0x7;
    n1[i] = GP1;
    n2[i] = GP2;
    toBinary(reg);
    printf("Z0=0, [n1,n2] %d %d \n", n1[i], n2[i]);
  }
  printf("\n");

  // Якщо Z0 отримує - 1
  printf("Z0 отримує '1'\n");
  for (size_t i = 0; i < AMOUNT_OF_STATES; i++) {
    reg = (dataIn[i] & 0x7) | 0x4;
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
