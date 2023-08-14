#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_SIZE 10

void getRandom(int data[], int size) {
  srand(time(NULL));

  for (size_t i = 0; i < size; i++) {
    data[i] = rand() % 2;
  }
}

void compArr(int arrA[], int arrB[], int sizeA, int sizeB) {
  int ctr = 0;
  if (sizeA != sizeB) {
    printf("Розмір: arrA != arrB");
    exit(EXIT_FAILURE);
  } else {
    for (size_t i = 0; i < sizeA; i++) {
      if (arrA != arrB) {
        ctr++;
        // Якщо хоча б одна розбіжність
        if (ctr == 1) {
          printf("Елементи з індексами [i] не співпадають: \n");
        }
        if (ctr > 0) {
          printf("[%d]", (int)i);
        }
      }
    }

    printf("\n");

    if (ctr > 0) {
      printf("Кількість неспівпадінь: %d\n", (int)ctr);
    } else {
      printf("Всі елементи послідовності співпадають. Розбіжність = %d\n",
             (int)ctr);
    }
  }
}

int main(int argc, char const *argv[]) {
  int data1[DATA_SIZE];
  int data2[DATA_SIZE];

  getRandom(data1, DATA_SIZE);
  getRandom(data2, DATA_SIZE);

  for (size_t i = 0; i < sizeof(data1) / sizeof(int); i++) {
    printf("%ul", data1[i]);
  }
  printf("\n");

  compArr(data1, data2, DATA_SIZE, DATA_SIZE);

  return 0;
}
