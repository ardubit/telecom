#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE_DATA 10

void copyData(uint8_t source[][SIZE_DATA], uint8_t dest[][SIZE_DATA]) {
  for (size_t i = 0; i < SIZE_DATA; i++) {
    for (size_t j = 0; j < SIZE_DATA; j++) {
      dest[i][j] = source[i][j];
    }
  }
  printf("Копіювання даних завершено! \n");
}

int main(int argc, char const *argv[]) {

  // Початкова послідовність даних
  uint8_t data[SIZE_DATA][SIZE_DATA];
  
  // Заготовка під копію
  uint8_t dataCopy[SIZE_DATA][SIZE_DATA];

  return 0;
}
