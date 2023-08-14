#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAXSIZE 1000

char coded[2 * SIZE_DATA];
char decoded[SIZE_DATA];

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
    }
    else if (i == 1) {
      if (K[j * 2] != 1) {
        Hd++;
      }
      if (K[j * 2 + 1] != 1) {
        Hd++;
      }
    }
  }
  else if (i0 == 1) {
    if (i == 2) {
      if (K[j * 2] != 0) {
        Hd++;
      }
      if (K[j * 2 + 1] != 1) {
        Hd++;
      }
    }
    else if (i == 3) {
      if (K[j * 2] != 1) {
        Hd++;
      }
      if (K[j * 2 + 1] != 0) {
        Hd++;
      }
    }
  }
  else if (i0 == 2) {
    if (i == 0) {
      if (K[j * 2] != 1) {
        Hd++;
      }
      if (K[j * 2 + 1] != 1) {
        Hd++;
      }
    }
    else if (i == 1) {
      if (K[j * 2] != 0) {
        Hd++;
      }
      if (K[j * 2 + 1] != 0) {
        Hd++;
      }
    }
  }
  else if (i0 == 3) {
    if (i == 2) {
      if (K[j * 2] != 1) {
        Hd++;
      }
      if (K[j * 2 + 1] != 0) {
        Hd++;
      }
    }
    else if (i == 3) {
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



void conv_coder(char *K) {
  char reg = 0b000;
  char reg_array[6];
  int n = 1;

  for (int j = 0; j < SIZE_DATA; j++) {
    reg <<= 1;
    reg &= 0b111;
    reg += (K[j]);

    for (int i = 0; i <= 5; i++) {
      reg_array[i] = (reg >> i) & 0b1;
    }

    coded[j * 2] = reg_array[0] ^ reg_array[2];
    coded[j * 2 + 1] = reg_array[0] ^ reg_array[1] ^ reg_array[2];
  }
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
    decoded[i] = ways[min_i][i];
  }
}

int main(void) {
  char K[SIZE_DATA];
  int cnt = 0, cnt_BER = 0;
  int j = 2, n = 1;

  srand(time(NULL));
  for (int i = 0; i < SIZE_DATA; i++) {
    K[i] = (char)(rand() % 2);
  }

  conv_coder(K);

  printf("\n\n");
  printf("Coded:\n");
  for (int i = 0; i < 2 * SIZE_DATA; i++) {
    if (i == n * 100) {
      printf("\n");
      n++;
    }
    printf("%d", coded[i]);
  }

  decoder_Viterb(coded, 2);

  printf("\n\n");
  printf("Decoded:\n");
  for (int i = 0; i < SIZE_DATA; i++) {
    if (i == n * 100) {
      printf("\n");
      n++;
    }
    printf("%d", decoded[i]);
  }

  for (int i = 0; i < SIZE_DATA; i++) {
    if (K[i] != decoded[i]) {
      cnt++;
      printf("\n%d error is found in %d element\n", cnt, i);
    }
  }

  if (cnt == 0) {
    printf("\nNo error found! Decoding is over successfully!\n");
  }

  coded[10] = !coded[10];
  coded[25] = !coded[25];
  coded[100] = !coded[100];
  coded[200] = !coded[200];
  printf("\n\n");

  decoder_Viterb(coded, 2);

  printf("\n\n");
  printf("\n\n");
  printf("Decoded:\n");
  for (int i = 0; i < SIZE_DATA; i++) {
    if (i == n * 100) {
      printf("\n");
      n++;
    }
    printf("%d", decoded[i]);
  }

  for (int i = 0; i < SIZE_DATA; i++) {
    if (K[i] != decoded[i]) {
      cnt++;
      printf("\n%d error is found in %d element\n", cnt, i);
    }
  }

  if (cnt == 0) {
    printf("\nNo error found! Decoding is over successfully!\n");
  }

  conv_coder(K);

  while (cnt == 0) {
    srand(time(NULL));
    j = (int)(rand() % 1000);
    coded[j] = !coded[j];
    decoder_Viterb(coded, 2);
    for (int i = 0; i < SIZE_DATA; i++) {
      if (K[i] != decoded[i]) {
        cnt++;
        printf("\n%d error is found in %d element\n", cnt, i);
      }
    }
    cnt_BER++;
  }

  printf("\ncnt = %d", cnt_BER);
  printf("\nBER = %f", (((float)(cnt_BER)) / SIZE_DATA));

  return 0;
}
