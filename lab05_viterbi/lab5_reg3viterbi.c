/*
Дослідження декодера Вітербі
gcc -o lab5_reg3viterbi lab5_reg3viterbi.c
./lab5_reg3viterbi
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
#define GP1 ((reg >> 2) ^ (reg >> 1) ^ reg) & 0x1
#define GP2 ((reg >> 2) ^ reg) & 0x1

void printData(uint8_t *arr, uint8_t length) {
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

uint8_t getHm(uint8_t *data, uint8_t initSt, uint8_t i, uint8_t j) {
  uint8_t metr = 0;
  uint8_t numOfSt = (uint8_t)pow(2, N_OUTS);

  for (size_t i = 0; i < numOfSt; i++)
  {
    if (initSt == i) && ()
    {
      /* code */
    }
    
  }
  

}

// Довідник структурою
struct d {
  uint8_t curState;
  uint8_t nxtStateZ00;
  uint8_t nxtStateZ01;
  uint8_t outZ00;
  uint8_t outZ01;
};

#define STATE_00 0x0
#define STATE_01 0x1
#define STATE_10 0x2
#define STATE_11 0x3

// struct d d1 = {0, 0, 2, 0x0, 0x3};
// struct d d2 = {1, 0, 2, 0x3, 0x0};
// struct d d3 = {2, 1, 3, 0x2, 0x1};
// struct d d4 = {3, 1, 3, 0x1, 0x2};

// Або через новий тип даних на основі структури
typedef const struct d stateDiagram;

// Таблиця
// Поточ.стан | Наст.стан Z0=0 | Наст.стан Z0=1 | OUT при Z0=0 | OUT при Z0=1
stateDiagram dg[4] = {{STATE_00, STATE_00, STATE_10, 0x0, 0x3},
                      {STATE_01, STATE_00, STATE_10, 0x3, 0x0},
                      {STATE_10, STATE_01, STATE_11, 0x2, 0x1},
                      {STATE_11, STATE_01, STATE_11, 0x1, 0x2}};

struct accMetrics {
  uint8_t *ptrCurState[4];
  uint8_t metric[SIZE_DATA];
};

// Таблиця метрик заповнена нулями
typedef struct accMetrics accMet;
accMet am[4] = {{&dg[0].curState, {0}},
                {&dg[1].curState, {0}},
                {&dg[2].curState, {0}},
                {&dg[3].curState, {0}}};

// struct accMetrics accMet;

// Довідник [ряд][кол]
// Поточ.стан | Наст.стан Z0=0 | Наст.стан Z0=1 | OUT при Z0=0 | OUT при Z0=1
// 0x0= 00, 0x1= 01, 0x2= 10, 0x3= 11
const uint8_t tabStates[4][5] = {
    // Z0=0 Z0=1
    {STATE_00, STATE_00, STATE_10, 0x0, 0x3},
    {STATE_01, STATE_00, STATE_10, 0x3, 0x0},
    {STATE_10, STATE_01, STATE_11, 0x2, 0x1},
    {STATE_11, STATE_01, STATE_11, 0x1, 0x2}};

#define X -1

// Поточний/наступний стан: i,j: [00,00] 01 10 00
const uint8_t tabAllowedStates[4][4] = {
    {0, X, 1, X}, {0, X, 1, X}, {X, 0, X, 1}, {X, 0, X, 1}};

int main(int argc, char const *argv[]) {
  printf("---\n");

  // Початкові дані (некодовані)
  uint8_t dataInit[] = {0, 1, 0, 1, 1, 1, 0, 0, 0};

  // Отримана послідовнісь (кодована)
  uint8_t dataCoded[] = {0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0};

  // Декодовані дані алгоритмом Вітербі
  uint8_t dataDecoded[SIZE_DATA + SIZE_REG];

  printf("Початкові дані: \n");
  printData(dataInit, (uint8_t)SIZE_DATA);

  printf("Отримана послідовнісь (кодована): \n");
  printData(dataCoded, (uint8_t)SIZE_DATA * N_OUTS);
  printf("---\n");

  printf("Декодер Вітербі\n");
  decoderViterbi(dataCoded, dataDecoded);

  printf("Вихідна послідовність: \n");
  compData(dataInit, dataDecoded);

  return 0;
}

// bufData[i] = dataIn[i];
// bufData[i] = bufData[i] << 1;
// bufData[i] = (bufData[i] | (dataIn[i + 1] & 0x1)) & 0x3;

// Довідник [ряд][кол]
// Поточ.стан | Наст.стан Z0=0 | Наст.стан Z0=1 | OUT при Z0=0 | OUT при Z0=1
// 0x0= 00, 0x1= 01, 0x2= 10, 0x3= 11
// const uint8_t d[4][5] = {
//     // Z0=0 Z0=1
//     {0, 0, 2, 0x0, 0x3},
//     {1, 0, 2, 0x3, 0x0},
//     {2, 1, 3, 0x2, 0x1},
//     {3, 1, 3, 0x1, 0x2}};

// printf("Рівні метрики! [data]: %d [Z00] %d [Z01] %d\n", dataInBuf[i],
//              tmpZ00, tmpZ01);

int getHammingDist(uint8_t *twoBitsData, uint8_t *twoBitsZ0) {
  uint8_t metric = 0;
  uint8_t xor = (*twoBitsData ^ *twoBitsZ0) & 0x3;
  if (((xor >> 1) & 0x1) == 1) {
    metric++;
  }
  if ((xor&0x1) == 1) {
    metric++;
  }
  printf("Відстань Хеммінга: %d, [data]: %d [Z0] %d\n", metric, *twoBitsData,
         *twoBitsZ0);
  return metric;
}

int decoderViterbi(uint8_t *dataIn, uint8_t *dataOut) {
  uint8_t reg = (uint8_t)INIT_REG;
  uint8_t curState = STATE_00, nextState = 0;
  

  // Пакування вхідних даних по 2 біти у байт
  uint8_t dataInBuf[SIZE_DATA];
  uint8_t j = 0;
  printf("Пакування вхідних даних: \n");
  for (size_t i = 0; i < SIZE_DATA; i++) {
    dataInBuf[i] = (dataIn[j] << 1) | dataIn[j + 1];
    dataInBuf[i] = dataInBuf[i] & 0x3;
    printf("%d ", (uint8_t)dataInBuf[i]);
    j += 2;
  }
  printf("\n");

  // Таблиця накопичених метрик
  uint8_t tabMetrics[totalStates]
                    [SIZE_DATA]; // кільк. вхідних пар * кільк. станів
  uint8_t metric = 0;

  // Перебирання вхідної послідовності по 2 біти
  for (size_t timeIndex = 0; timeIndex < SIZE_DATA; timeIndex++) {
    uint8_t dataIndex = timeIndex;

    // Пишу у ряд таблиці

    for (size_t rowStIndex = 0; rowStIndex < amountOfStates; rowStIndex++) {

      // Пишу по колонках таблиці

      // Первіряю чи дозволено писати (є шлях з поточного стану у наступний),
      // якщо ні записую 0
      uint8_t tmpAllowedStates[4];
      for (size_t i = 0; i < amountOfStates; i++) {
        if (curState ==)
      }

      if () {
        //
      } else {
        // Запиши метрику як 0
      }

      // Взяти вхідну пару і порівняти з OUT при Z0=0 | OUT при Z0=1
      uint8_t tmpOutZ0 = tabStates[state].outZ00;
      tabMetrics[state][i] = getHammingDist(&dataInBuf[i],
    }

    uint8_t metZ00, metZ01; // Значення метрики при Z0=0, Z0=1 у поточному стані

    // Розрахунок метрик Z00, Z01. Через tmp для узгодження вказівника з конст
    uint8_t tmpZ00 = dg[state].outZ00;
    uint8_t tmpZ01 = dg[state].outZ01;
    metZ00 = getHammingDist(&dataInBuf[i], &tmpZ00);
    metZ01 = getHammingDist(&dataInBuf[i], &tmpZ01);

    // Визначення наступного стану. Якщо метрики рівні або котра найменша
    if (metZ00 == metZ01) {
      // TODO
    } else if (metZ00 < metZ01) {
      next = dg[state].nxtStateZ00;
    } else {
      next = dg[state].nxtStateZ01;
    }

    // Оновити поточний стан
    state = next;
  }

  printf("\n");

  return 0;
}