
/*
Дослідження декодера Вітербі. Кодер
gcc -o 5v3 5v3.c
./5v3
========================================
XOR якщо A!=B, B!=A то Y=1
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE_DATA 9

#define STATE_00 0x0
#define STATE_01 0x1
#define STATE_10 0x2
#define STATE_11 0x3

char dataInit[SIZE_DATA] = {0, 1, 0, 1, 1, 1, 0, 0, 0};

char dataCoded[2 * SIZE_DATA] = {0, 0, 1, 1, 0, 1, 0, 0, 1,
                                 0, 0, 1, 1, 0, 1, 1, 0, 0};

// перші
char dataCodedErr1[2 * SIZE_DATA] = {1, 0, 1, 1, 0, 1, 0, 0, 1,
                                     0, 0, 1, 1, 0, 1, 1, 0, 1};

// пара
char dataCodedErr2[2 * SIZE_DATA] = {1, 1, 1, 1, 0, 1, 0, 0, 1,
                                     0, 0, 1, 1, 0, 1, 1, 1, 1};

// тріоль
char dataCodedErr3[2 * SIZE_DATA] = {1, 1, 0, 1, 0, 1, 0, 0, 1,
                                     0, 0, 1, 1, 0, 1, 0, 1, 1};

// всі в 1
char dataCodedErr4[2 * SIZE_DATA] = {1, 1, 1, 1, 1, 1, 1, 1, 1,
                                     1, 1, 1, 1, 1, 1, 1, 1, 1};

char dataDecoded[SIZE_DATA];
#define N_OUTS 2

void compData(char arrA[], char arrB[]) {
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

// Таблиця
// Поточ.стан | Наст.стан Z0=0 | Наст.стан Z0=1 | OUT при Z0=0 | OUT при Z0=1
// stateDiagram dg[4] = {{STATE_00, STATE_00, STATE_10, 0x0, 0x3},
//                       {STATE_01, STATE_00, STATE_10, 0x3, 0x0},
//                       {STATE_10, STATE_01, STATE_11, 0x2, 0x1},
//                       {STATE_11, STATE_01, STATE_11, 0x1, 0x2}};

// char getMetricsHem(char *inData, int i0, int i, int j) {
//   char hemDist = 0;
//   const char values[4][4][2] = {{{0, 0}, {0, 0}, {0, 0}, {0, 0}},
//                                 {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
//                                 {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
//                                 {{0, 0}, {0, 0}, {0, 0}, {0, 0}}};
//   if (inData[j * 2] != values[i0][i][0]) {
//     hemDist++;
//   }
//   if (inData[j * 2 + 1] != values[i0][i][1]) {
//     hemDist++;
//   }
//   return hemDist;
// }

char getMetricsHem(char *inData, int state, int i, int j) {
  char hemDist = 0;

  switch (state) {
  case 0:
    switch (i) {
    case 0:
      if (inData[j * 2] != 0) {
        hemDist++;
      }
      if (inData[j * 2 + 1] != 0) {
        hemDist++;
      }
      break;
    case 1:
      if (inData[j * 2] != 1) {
        hemDist++;
      }
      if (inData[j * 2 + 1] != 1) {
        hemDist++;
      }
      break;
    }
    break;
  case 1:
    switch (i) {
    case 2:
      if (inData[j * 2] != 0) {
        hemDist++;
      }
      if (inData[j * 2 + 1] != 1) {
        hemDist++;
      }
      break;
    case 3:
      if (inData[j * 2] != 1) {
        hemDist++;
      }
      if (inData[j * 2 + 1] != 0) {
        hemDist++;
      }
      break;
    }
    break;
  case 2:
    switch (i) {
    case 0:
      if (inData[j * 2] != 1) {
        hemDist++;
      }
      if (inData[j * 2 + 1] != 1) {
        hemDist++;
      }
      break;
    case 1:
      if (inData[j * 2] != 0) {
        hemDist++;
      }
      if (inData[j * 2 + 1] != 0) {
        hemDist++;
      }
      break;
    }
    break;
  case 3:
    switch (i) {
    case 2:
      if (inData[j * 2] != 1) {
        hemDist++;
      }
      if (inData[j * 2 + 1] != 0) {
        hemDist++;
      }
      break;
    case 3:
      if (inData[j * 2] != 0) {
        hemDist++;
      }
      if (inData[j * 2 + 1] != 1) {
        hemDist++;
      }
      break;
    }
    break;
  }

  return hemDist;
}

void decoderViterbi(char *inData, char *dataOut) {
  const int bitsPerDataTime = 2;
  char tablPaths[4][SIZE_DATA];
  char tablHemDist[4][SIZE_DATA] = {0};
  int min_ = 0;

  for (int j = 0; j < SIZE_DATA; j++) {
    char tablPathsCopy[4][SIZE_DATA];

    // Copy
    for (int k = 0; k < SIZE_DATA; k++) {
      for (int kk = 0; kk < 4; kk++) {
        tablPathsCopy[kk][k] = tablPaths[kk][k];
      }
    }

    for (int i = 0; i < 4; i++) {
      if (j < bitsPerDataTime) {
        if (j == 0) {
          tablPaths[i][j] = i;
          tablHemDist[i][j] = getMetricsHem(inData, 0, i, j);
        } else if (((i == 0) || (i == 1)) && (j != 0)) {
          for (int k = 0; k < j; k++) {
            tablPaths[i][k] = tablPathsCopy[0][k];
          }
          tablPaths[i][j] = i;
          tablHemDist[i][j] =
              tablHemDist[0][j - 1] + getMetricsHem(inData, 0, i, j);
        } else if (((i == 2) || (i == 3)) && (j != 0)) {
          for (int k = 0; k < j; k++) {
            tablPaths[i][k] = tablPathsCopy[1][k];
          }
          tablPaths[i][j] = i - 2;
          tablHemDist[i][j] =
              tablHemDist[1][j - 1] + getMetricsHem(inData, 1, i, j);
        }
      } else {
        char cp = 0;
        if ((i == 0) || (i == 1)) {
          tablHemDist[i][j] =
              tablHemDist[0][j - 1] + getMetricsHem(inData, 0, i, j);
          cp = tablHemDist[2][j - 1] + getMetricsHem(inData, 2, i, j);
          if (cp < tablHemDist[i][j]) {
            tablHemDist[i][j] = cp;
            for (int k = 0; k < j; k++) {
              tablPaths[i][k] = tablPathsCopy[2][k];
            }
          } else {
            for (int k = 0; k < j; k++) {
              tablPaths[i][k] = tablPathsCopy[0][k];
            }
          }
          tablPaths[i][j] = i;
        } else if ((i == 2) || (i == 3)) {
          tablHemDist[i][j] =
              tablHemDist[1][j - 1] + getMetricsHem(inData, 1, i, j);
          cp = tablHemDist[3][j - 1] + getMetricsHem(inData, 3, i, j);
          if (cp < tablHemDist[i][j]) {
            tablHemDist[i][j] = cp;
            for (int k = 0; k < j; k++) {
              tablPaths[i][k] = tablPathsCopy[3][k];
            }
          } else {
            for (int k = 0; k < j; k++) {
              tablPaths[i][k] = tablPathsCopy[1][k];
            }
          }
          tablPaths[i][j] = i - 2;
        }
      }
    }
  }

  char min = tablHemDist[0][SIZE_DATA - 1];
  for (int i = 0; i < 4; i++) {
    if (tablHemDist[i][SIZE_DATA - 1] < min) {
      min = tablHemDist[i][SIZE_DATA - 1];
      min_ = i;
    }
  }

  for (int i = 0; i < SIZE_DATA; i++) {
    dataOut[i] = tablPaths[min_][i];
  }
}

void printData(char *arr, uint8_t length) {
  for (size_t i = 0; i < length; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

int main(void) {

  printf("---\n");
  printf("Початкові дані: \n");
  printData(dataInit, SIZE_DATA);

  printf("Отримана послідовнісь (кодована): \n");
  printData(dataCoded, SIZE_DATA * N_OUTS);
  printf("\n");
  printf("Декодер Вітербі\n");
  decoderViterbi(dataCoded, dataDecoded);
  printf("Декодована послідовнісь: \n");
  printData(dataDecoded, SIZE_DATA);
  compData(dataInit, dataDecoded);

  printf(" --- Тест ---\n");
  printf("1. Пошкоджені дані -Інверсія перший та останній біт: \n");
  printData(dataCodedErr1, SIZE_DATA * N_OUTS);
  printf("Декодована послідовнісь з виправленням помилок: \n");
  decoderViterbi(dataCodedErr1, dataDecoded);
  printData(dataDecoded, SIZE_DATA);
  compData(dataInit, dataDecoded);

  printf("2. Пошкоджені дані -Інверсія [Пари] перший та останній біт: \n");
  printData(dataCodedErr2, SIZE_DATA * N_OUTS);
  decoderViterbi(dataCodedErr2, dataDecoded);
  printf("Декодована послідовнісь з виправленням помилок: \n");
  printData(dataDecoded, SIZE_DATA);
  compData(dataInit, dataDecoded);

  printf("3. Пошкоджені дані -Інверсія [Тріоль] перший та останній біт: \n");
  printData(dataCodedErr3, SIZE_DATA * N_OUTS);
  decoderViterbi(dataCodedErr3, dataDecoded);
  printf("Декодована послідовнісь з виправленням помилок: \n");
  printData(dataDecoded, SIZE_DATA);
  compData(dataInit, dataDecoded);

  return 0;
}

// void conv_coder(char *K) {
//   char reg = 0b000;
//   char j = 0;
//   char reg_array[6];
//   int n = 1;

//   for (int j = 0; j < MAXSIZE; j++) {
//     reg <<= 1;
//     reg &= 0b111;
//     reg += (K[j]);

//     for (int i = 0; i <= 5; i++) {
//       reg_array[i] = (reg >> i) & 0b1;
//     }

//     // coded[j * 2] = reg_array[0] ^ reg_array[1] ^ reg_array[2];
//     // coded[j * 2 + 1] = reg_array[0] ^ reg_array[2];

//     coded[j * 2 + 1] = reg_array[0] ^ reg_array[1] ^ reg_array[2];
//     coded[j * 2] = reg_array[0] ^ reg_array[2];
//   }
// }

// char getMetricsHem(char *inData, int i0, int i, int j) {
//   char hemDist = 0;

//   if ((i0 == 0) && (i == 0)) {
//     if (inData[j * 2] != 0) {
//       hemDist++;
//     }
//     if (inData[j * 2 + 1] != 0) {
//       hemDist++;
//     }
//   }

//   if ((i0 == 0) && (i == 1)) {
//     if (inData[j * 2] != 1) {
//       hemDist++;
//     }
//     if (inData[j * 2 + 1] != 1) {
//       hemDist++;
//     }
//   }

//   if ((i0 == 1) && (i == 2)) {
//     if (inData[j * 2] != 0) {
//       hemDist++;
//     }
//     if (inData[j * 2 + 1] != 1) {
//       hemDist++;
//     }
//   }

//   if ((i0 == 1) && (i == 3)) {
//     if (inData[j * 2] != 1) {
//       hemDist++;
//     }
//     if (inData[j * 2 + 1] != 0) {
//       hemDist++;
//     }
//   }

//   if ((i0 == 2) && (i == 0)) {
//     if (inData[j * 2] != 1) {
//       hemDist++;
//     }
//     if (inData[j * 2 + 1] != 1) {
//       hemDist++;
//     }
//   }

//   if ((i0 == 2) && (i == 1)) {
//     if (inData[j * 2] != 0) {
//       hemDist++;
//     }
//     if (inData[j * 2 + 1] != 0) {
//       hemDist++;
//     }
//   }

//   if ((i0 == 3) && (i == 2)) {
//     if (inData[j * 2] != 1) {
//       hemDist++;
//     }
//     if (inData[j * 2 + 1] != 0) {
//       hemDist++;
//     }
//   }

//   if ((i0 == 3) && (i == 3)) {
//     if (inData[j * 2] != 0) {
//       hemDist++;
//     }
//     if (inData[j * 2 + 1] != 1) {
//       hemDist++;
//     }
//   }

//   return hemDist;
// }

// void decoderViterbi(char *inData) {
//   int bitsPerDataTime = 2;
//   char tablPaths[4][SIZE_DATA];
//   char tablPathsCopy[4][SIZE_DATA];
//   char tablHemDist[4][SIZE_DATA] = {0};
//   char cp = 0, min;
//   int min_ = 0, n = 0;

//   for (int j = 0; j < SIZE_DATA; j++) {

//     // копія
//     for (int k = 0; k < SIZE_DATA; k++) {
//       for (int kk = 0; kk < 4; kk++) {
//         tablPathsCopy[kk][k] = tablPaths[kk][k];
//       }
//     }

//     for (int i = 0; i < 4; i++) {

//       if (j < bitsPerDataTime) {

//         if (j == 0) {
//           tablPaths[i][j] = i;
//           tablHemDist[i][j] = getMetricsHem(inData, 0, i, j);
//         }

//         if (((i == 0) || (i == 1)) && (j != 0)) {
//           for (int k = 0; k < j; k++) {
//             tablPaths[i][k] = tablPathsCopy[0][k];
//           }
//           tablPaths[i][j] = i;
//           tablHemDist[i][j] = tablHemDist[0][j - 1] + getMetricsHem(inData,
//           0, i, j);
//         }

//         if (((i == 2) || (i == 3)) && (j != 0)) {
//           for (int k = 0; k < j; k++) {
//             tablPaths[i][k] = tablPathsCopy[1][k];
//           }
//           tablPaths[i][j] = i - 2;
//           tablHemDist[i][j] = tablHemDist[1][j - 1] + getMetricsHem(inData,
//           1, i, j);
//         }

//       } else {

//         if ((i == 0) || (i == 1)) {
//           cp = 0;
//           tablHemDist[i][j] = tablHemDist[0][j - 1] + getMetricsHem(inData,
//           0, i, j); cp = tablHemDist[2][j - 1] + getMetricsHem(inData, 2, i,
//           j); if (cp < tablHemDist[i][j]) {
//             tablHemDist[i][j] = cp;
//             for (int k = 0; k < j; k++) {
//               tablPaths[i][k] = tablPathsCopy[2][k];
//             }
//           } else {
//             for (int k = 0; k < j; k++) {
//               tablPaths[i][k] = tablPathsCopy[0][k];
//             }
//           }
//           tablPaths[i][j] = i;
//         }

//         if ((i == 2) || (i == 3)) {
//           cp = 0;
//           tablHemDist[i][j] = tablHemDist[1][j - 1] + getMetricsHem(inData,
//           1, i, j); cp = tablHemDist[3][j - 1] + getMetricsHem(inData, 3, i,
//           j); if (cp < tablHemDist[i][j]) {
//             tablHemDist[i][j] = cp;
//             for (int k = 0; k < j; k++) {
//               tablPaths[i][k] = tablPathsCopy[3][k];
//             }
//           } else {
//             for (int k = 0; k < j; k++) {
//               tablPaths[i][k] = tablPathsCopy[1][k];
//             }
//           }
//           tablPaths[i][j] = i - 2;
//         }
//       }
//     }
//   }

//   min = tablHemDist[0][SIZE_DATA - 1];
//   for (int i = 0; i < 4; i++) {
//     if (tablHemDist[i][SIZE_DATA - 1] < min) {
//       min = tablHemDist[i][SIZE_DATA - 1];
//       min_ = i;
//     }
//   }

//   for (int i = 0; i < SIZE_DATA; i++) {
//     dataDecoded[i] = tablPaths[min_][i];
//   }
// }