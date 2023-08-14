#include <stdio.h>
#include <stdlib.h>

uint8_t arr[] = {0, 0, 1, 1, 2, 2, 3, 3};
uint8_t arrLength = 8;

int main(int argc, char const *argv[]) {

  printf("Поелементно: \n");
  for (size_t i = 0; i < arrLength; i++) {
    printf("%d, ", arr[i]);
  }

  printf("\n");

  printf("По два елементи з кроком - 2: \n");
  for (size_t k = 0; k < arrLength / 2; k++) {
    printf("%d%d ", arr[k * 2], arr[k * 2 + 1]);
  }

  return 0;
}
