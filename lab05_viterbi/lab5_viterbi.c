/*
Дослідження декодера Вітербі
gcc -o lab5_viterbi lab5_viterbi.c
./lab5_viterbi
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

#define SIZE_DATA 5
#define SIZE_METRCIS 8 // for 4 bit coder
#define INIT_REG 0x0

int getMetricHammingDist(uint8_t twoBitsA, uint8_t twoBitsB) {
  uint8_t metric = 0;
  uint8_t xor = (twoBitsA ^ twoBitsB) & 0x3;
  if (((xor >> 1) & 0x1) == 1)
    metric++;
  if ((xor & 0x1) == 1)
    metric++;
  printf("Відстань Хеммінга: %d\n", metric);

  return metric;
}

int decoderViterbi(uint8_t *dataIn, uint8_t *dataOut) {
  uint8_t reg = (uint8_t)INIT_REG;

  return 0;
}

int main(int argc, char const *argv[]) {
  // З кожного стану існує два шляхи, всього 8 станів від 000 до 111
  uint8_t metrics[SIZE_METRCIS * 2]; 

  uint8_t dataInit[SIZE_DATA] = {1, 0, 0, 0, 0};
  // uint8_t dataInit[SIZE_DATA];
  // uint8_t dataOut[SIZE_DATA * N_OUTS];

  // printf("Початкова послідовність: \n");
  // // fillDataWithRandom(dataInit, (uint8_t)SIZE_DATA);
  // printData(dataInit, (uint8_t)SIZE_DATA);
  // printf("---\n");

  // coderConv(dataInit, dataOut);

  return 0;
}
