/*
Дослідження декодера Вітербі
gcc -o lab5_reg3viterbi_2 lab5_reg3viterbi_2.c
./lab5_reg3viterbi_2
========================================
Polynomials
13oct 0o13 = 1011 = 0xB -> GP1 = x^3 + x^1 + x^0
17oct 0o17 = 1111 = 0xF -> GP2 = x^3 + x^2 + x^1 + x^0
XOR якщо A!=B, B!=A то Y=1
*/

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE_REG 3 // Довжина регістру
#define N_OUTS 2   // Кількість виходів кодувальника

// --- INIT HERE ----
#define SIZE_DATA_BITS 6 // Довжина послідовності
#define SIZE_DATA (SIZE_DATA_BITS + SIZE_REG)

#define INIT_REG 0x0
#define GP1 (((reg >> 2) ^ (reg >> 1) ^ reg) & 0x1)
#define GP2 (((reg >> 2) ^ reg) & 0x1)

#define MAXSIZE 1000

// Початкові дані (некодовані)
char dataInit[] = {0, 1, 0, 1, 1, 1, 0, 0, 0};

// Отримана послідовнісь (кодована)
char dataCoded[] = {0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0};

// Декодовані дані алгоритмом Вітербі
char dataDecoded[SIZE_DATA + SIZE_REG];

void printData(char *arr, uint8_t length) {
  for (size_t i = 0; i < length; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

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

char getMetricsHem(char *K, int i0, int i, int j) {
  char Hd = 0;

  if (i0 == 0) {
    if (i == 0) {
      if (K[j * 2] != 0) {
        Hd++;
      }
      if (K[j * 2 + 1] != 0) {
        Hd++;
      }
    } else if (i == 1) {
      if (K[j * 2] != 1) {
        Hd++;
      }
      if (K[j * 2 + 1] != 1) {
        Hd++;
      }
    }
  } else if (i0 == 1) {
    if (i == 2) {
      if (K[j * 2] != 0) {
        Hd++;
      }
      if (K[j * 2 + 1] != 1) {
        Hd++;
      }
    } else if (i == 3) {
      if (K[j * 2] != 1) {
        Hd++;
      }
      if (K[j * 2 + 1] != 0) {
        Hd++;
      }
    }
  } else if (i0 == 2) {
    if (i == 0) {
      if (K[j * 2] != 1) {
        Hd++;
      }
      if (K[j * 2 + 1] != 1) {
        Hd++;
      }
    } else if (i == 1) {
      if (K[j * 2] != 0) {
        Hd++;
      }
      if (K[j * 2 + 1] != 0) {
        Hd++;
      }
    }
  } else if (i0 == 3) {
    if (i == 2) {
      if (K[j * 2] != 1) {
        Hd++;
      }
      if (K[j * 2 + 1] != 0) {
        Hd++;
      }
    } else if (i == 3) {
      if (K[j * 2] != 0) {
        Hd++;
      }
      if (K[j * 2 + 1] != 1) {
        Hd++;
      }
    }
  }

  return Hd;
}

void decoder_Viterb(char *K, int m) {
  char ways[4][SIZE_DATA];
  char ways_copy[4][SIZE_DATA];
  char Hd[4][SIZE_DATA] = {0};
  char copy = 0, min;
  int min_i = 0, n = 0;

  for (int j = 0; j < SIZE_DATA; j++) {

    // копія
    for (int k = 0; k < SIZE_DATA; k++) {
      for (int kk = 0; kk < 4; kk++) {
        ways_copy[kk][k] = ways[kk][k];
      }
    }

    // метрики по стовпцям
    for (int i = 0; i < 4; i++) {

      if (j < m) {

        if (j == 0) {
          ways[i][j] = i;
          Hd[i][j] = getMetricsHem(K, 0, i, j);
        }

        if (((i == 0) || (i == 1)) && (j != 0)) {
          for (int k = 0; k < j; k++) {
            ways[i][k] = ways_copy[0][k];
          }
          ways[i][j] = i;
          Hd[i][j] = Hd[0][j - 1] + getMetricsHem(K, 0, i, j);
        }

        if (((i == 2) || (i == 3)) && (j != 0)) {
          for (int k = 0; k < j; k++) {
            ways[i][k] = ways_copy[1][k];
          }
          ways[i][j] = i - 2;
          Hd[i][j] = Hd[1][j - 1] + getMetricsHem(K, 1, i, j);
        }

      } else {

        if ((i == 0) || (i == 1)) {
          copy = 0;
          Hd[i][j] = Hd[0][j - 1] + getMetricsHem(K, 0, i, j);
          copy = Hd[2][j - 1] + getMetricsHem(K, 2, i, j);
          if (copy < Hd[i][j]) {
            Hd[i][j] = copy;
            for (int k = 0; k < j; k++) {
              ways[i][k] = ways_copy[2][k];
            }
          } else {
            for (int k = 0; k < j; k++) {
              ways[i][k] = ways_copy[0][k];
            }
          }
          ways[i][j] = i;
        }

        if ((i == 2) || (i == 3)) {
          copy = 0;
          Hd[i][j] = Hd[1][j - 1] + getMetricsHem(K, 1, i, j);
          copy = Hd[3][j - 1] + getMetricsHem(K, 3, i, j);
          if (copy < Hd[i][j]) {
            Hd[i][j] = copy;
            for (int k = 0; k < j; k++) {
              ways[i][k] = ways_copy[3][k];
            }
          } else {
            for (int k = 0; k < j; k++) {
              ways[i][k] = ways_copy[1][k];
            }
          }
          ways[i][j] = i - 2;
        }
      }
    }
  }

  min = Hd[0][SIZE_DATA - 1];
  for (int i = 0; i < 4; i++) {
    if (Hd[i][SIZE_DATA - 1] < min) {
      min = Hd[i][SIZE_DATA - 1];
      min_i = i;
    }
  }

  for (int i = 0; i < SIZE_DATA; i++) {
    dataDecoded[i] = ways[min_i][i];
  }
}

int main() {
  printf("---\n");

  printf("Початкові дані: \n");
  printData(dataInit, (uint8_t)SIZE_DATA);

  printf("Отримана послідовнісь (кодована): \n");
  printData(dataCoded, (uint8_t)SIZE_DATA * N_OUTS);
  printf("---\n");

  printf("Декодер Вітербі\n");
  decoder_Viterb(dataCoded, 2);

  printf("Декодована послідовнісь (кодована): \n");
  printData(dataDecoded, (uint8_t)SIZE_DATA);

  // printf("Вихідна послідовність: \n");
  // compData(dataInit, dataDecoded);

  return 0;
}