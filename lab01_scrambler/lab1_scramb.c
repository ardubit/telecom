/*
Дослідження скремблерів та дескремблерів
gcc -o lab1_scramb lab1_scramb.c
./lab1_scramb
========================================
Необхідно реалізувати:
-
Generator polynomials
#23 SelfSync GP = x^8 + x^4 + x^2 + 1
#24 AddiSync GP = x^8 + x^3 + x^2 + 1

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
#define INIT_REG 0x2A // 0010 1010 (#24)
#define GP_SSYNC ((reg >> 8) ^ (reg >> 4) ^ (reg >> 2)) & 0x1
#define GP_ASYNC ((reg >> 8) ^ (reg >> 3) ^ (reg >> 2)) & 0x1

#define INIT_TEST 0xFFFF

void compData(uint8_t arrA[], uint8_t arrB[]) {
  printf("Перевірка: \n");
  uint8_t ctr = 0;
  for (size_t i = 0; i < SIZE_DATA; i++) {
    if (arrA[i] != arrB[i]) {
      ctr++;
      // Якщо хоча б одна розбіжність
      if (ctr == 1) {
        printf("Елементи [i] не співпадають: \n");
      }
      if (ctr > 0) {
        printf("[%d] ", (uint8_t)i);
        if ((ctr % 10) == 0)
          printf("\n");
      }
    }
  }

  if (ctr > 0) {
    printf("\nКількість неспівпадінь: %d\n\n", ctr);
  } else {
    printf("Всі елементи послідовностей співпадають \nРозбіжність = %d\n\n",
           ctr);
  }
}

void fillDataWithRandom(uint8_t *arr, uint8_t size) {
  srand(time(NULL));
  for (size_t i = 0; i < size; i++) {
    arr[i] = (uint8_t)rand() % 2;
  }
}

void printData(uint8_t *arr) {
  for (size_t i = 0; i < SIZE_DATA; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void printDataReverse(uint8_t *arr) {
  for (size_t i = SIZE_DATA; i > 0; i--) {
    printf("%d ", arr[i - 1]);
  }
  printf("\n");
}

// Адитивний (additive or synchronous)
int screamblerAddSync(uint8_t *dataIn, uint8_t *dataOut) {
  // Регістр адитивного скремблера ініціалізується початковим значенням
  uint8_t reg = (uint8_t)INIT_REG;
  for (size_t i = 0; i < SIZE_DATA; i++) {
    uint8_t feedBack = GP_ASYNC;
    dataOut[i] = dataIn[i] ^ feedBack;
    reg = (reg << 1) | feedBack;
  }

  return 0;
}

int deScreamblerAddSync(uint8_t *dataIn, uint8_t *dataOut) {
  uint8_t reg = (uint8_t)INIT_REG;
  for (size_t i = 0; i < SIZE_DATA; i++) {
    uint8_t feedBack = GP_ASYNC;
    dataOut[i] = dataIn[i] ^ feedBack;
    reg = (reg << 1) | feedBack;
  }
  return 0;
}

// Самосинхронізуючий (multiplicative or self-synchronizing):
int screamblerSelfSync(uint8_t *dataIn, uint8_t *dataOut) {
  uint8_t reg = 0x0;
  for (size_t i = 0; i < SIZE_DATA; i++) {
    uint8_t feedBack = GP_SSYNC;
    dataOut[i] = dataIn[i] ^ feedBack;
    reg = (reg << 1) | dataOut[i];
  }

  return 0;
}

int deScreamblerSelfSync(uint8_t *dataIn, uint8_t *dataOut) {
  uint8_t reg = 0x0;
  for (size_t i = 0; i < SIZE_DATA; i++) {
    uint8_t feedBack = GP_SSYNC;
    dataOut[i] = dataIn[i] ^ feedBack;
    reg = (reg << 1) | dataIn[i];
  }
  return 0;
}

// Переписує дані у послідовності котру приймає як аргумент
// та повертає код виходу
int screamblerTest(uint8_t *data) {
  uint8_t reg = (uint8_t)INIT_TEST;
  // reg = reg & 0x7F; // Маскування останнього біта
  data[0] = 2;
  return 0;
}

int main() {

  // uint8_t dataInit[SIZE_DATA] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  uint8_t dataInit[SIZE_DATA];
  uint8_t dataOut[SIZE_DATA];
  uint8_t dataRecovered[SIZE_DATA];

  printf("Початкова послідовність: \n");
  fillDataWithRandom(dataInit, SIZE_DATA);
  printData(dataInit);
  printf("---\n");

  // SelfSync
  printf("Скрембльована послідовність SelfSync: \n");
  screamblerSelfSync(dataInit, dataOut);
  printData(dataOut);

  printf("Відновлена послідовність SelfSync: \n");
  deScreamblerSelfSync(dataOut, dataRecovered);
  printData(dataRecovered);

  compData(dataInit, dataRecovered);

  // AddSync
  printf("Скрембльована послідовність AddSync: \n");
  screamblerAddSync(dataInit, dataOut);
  printData(dataOut);

  printf("Відновлена послідовність AddSync: \n");
  deScreamblerAddSync(dataOut, dataRecovered);
  printData(dataRecovered);

  compData(dataInit, dataRecovered);

  // ТЕСТ Пошкодження даних
  // =======================================
  // #define POS 0
  // dataOut[9] = dataOut[9] ^ (1 << POS); // Інвертування 0-го біту
  uint8_t index = 0;

  printf("ТЕСТ Пошкодження даних SelfSync: \n---\n");
  printf("Початкова послідовність: \n");
  printData(dataInit);

  // SelfSync
  printf("Скрембльована послідовність SelfSync: \n");
  screamblerSelfSync(dataInit, dataOut);
  printData(dataOut);

  dataOut[index] ^= 0x1; // Інвертування 0-го біту
  dataOut[index+1] ^= 0x1; // Інвертування 0-го біту
  printf("Пошкодження скрембльованої послідовності [%d],[%d]-елемент: \n", index, index+1);
  // printf("Пошкодження скрембльованої послідовності [%d]-елемент: \n", index);
  printData(dataOut);

  printf("Відновлена послідовність SelfSync: \n");
  deScreamblerSelfSync(dataOut, dataRecovered);
  printData(dataRecovered);
  compData(dataInit, dataRecovered);

  // AddSync
  printf("ТЕСТ Пошкодження даних  AddSync: \n---\n");
  printf("Початкова послідовність: \n");
  printData(dataInit);

  printf("Скрембльована послідовність AddSync: \n");
  screamblerAddSync(dataInit, dataOut);
  printData(dataOut);

  dataOut[index] ^= 0x1; // Інвертування 0-го біту
  dataOut[index+1] ^= 0x1; // Інвертування 0-го біту
  printf("Пошкодження скрембльованої послідовності [%d],[%d]-елемент: \n", index, index+1);
  // printf("Пошкодження скрембльованої послідовності [%d]-елемент: \n", index);
  printData(dataOut);

  printf("Відновлена послідовність SelfSync: \n");
  deScreamblerAddSync(dataOut, dataRecovered);
  printData(dataRecovered);
  compData(dataInit, dataRecovered);

  return 0;
}
