/*
Дослідження інтерліверів та деінтерліверів
gcc -o lab2_inter lab2_inter.c
./lab2_inter
========================================
Необхідно реалізувати:
- Сформувати послідовнісь випадкових даних К=1000
- Сформувати двомірний масив матрицю NхM заповнену сформованими даними
- Визначити закон перемішування як <8 1 3 4 6 7 9 5 2 0>
- Реалізувати перемішувач інтерліверів
- Вивести перемішаний масив
- Пошкодити поряд розташовані 3-4 комірки "біти"
- Впевнитись у тому, що утворені помилки вже поодинокі
========================================
ASCII-коди:
0: 48     A: 65     C: 67
1: 49     B: 66     D: 68
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE_DATA 10

// Закон перемішування
uint8_t pi[] = {8, 1, 3, 4, 6, 7, 9, 5, 2, 0};

void fillData(uint8_t arr[][SIZE_DATA]) {
  srand(time(NULL)); // Ініціалізація генератора
  for (size_t i = 0; i < SIZE_DATA; i++) {
    for (size_t j = 0; j < SIZE_DATA; j++) {
      uint8_t r = (uint8_t)rand() % 2;
      if (r == 0) {
        arr[i][j] = 48; // ascii 0
      } else
        arr[i][j] = 49; // ascii 1
    }
  }
  // Позначаємо помилкові "біти"
  uint8_t row = 0;
  for (size_t col = 0; col <= 3; col++) {
    arr[row][col] = 65 + col;
  }
}

void printData(uint8_t *arr) {
  for (size_t i = 0; i < SIZE_DATA; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n\n");
}

void printData2D(uint8_t arr[][SIZE_DATA]) {
  for (size_t i = 0; i < SIZE_DATA; i++) {
    for (size_t j = 0; j < SIZE_DATA; j++) {
      // printf("%d ", arr[i][j]);
      printf("%c ", (char)arr[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

void printDataPassByPointer(uint8_t *arr) {
  for (size_t i = 0; i < SIZE_DATA; i++) {
    for (size_t j = 0; j < SIZE_DATA; j++) {
      printf("%c ", (char)arr[i * SIZE_DATA + j]);
    }
    printf("\n");
  }
  printf("\n");
}

// Повертає вказівник на масив з пам'яті
uint8_t *interleaver(uint8_t *arr, uint8_t *p) {

  // Виділяємо область динамічної пам'яті під новий масив перемішаних даних
  uint8_t *dataMixed = malloc(SIZE_DATA * SIZE_DATA * sizeof(uint8_t));
  if (dataMixed == NULL) {
    printf("Помилка виділення пам'яті!");
    return NULL;
  }
  // Зчитуємо по col пишемо у row
  for (size_t col = 0; col < SIZE_DATA; col++) {
    int colIndex = p[col]; // <8 1 3 4 6 7 9 5 2 0>
    for (size_t row = 0; row < SIZE_DATA; row++) {
      // Після malloc двомірний масив можна уявити як послідовність байтів
      // одновимірного масив (typecast to 2D)
      dataMixed[col * SIZE_DATA + row] = arr[row * SIZE_DATA + colIndex];
    }
  }

  return dataMixed;
}

uint8_t *deinterleaver(uint8_t *arr, uint8_t *p) {
  uint8_t *dataReMixed = malloc(SIZE_DATA * SIZE_DATA * sizeof(uint8_t));
  if (dataReMixed == NULL) {
    printf("Помилка виділення пам'яті!");
    return NULL;
  }
  // Зчитуємо по row пишемо у col
  for (size_t col = 0; col < SIZE_DATA; col++) {
    int colIndex = p[col]; // <8 1 3 4 6 7 9 5 2 0>
    for (size_t row = 0; row < SIZE_DATA; row++) {
      // Пишу / Читаю
      dataReMixed[row * SIZE_DATA + colIndex] = arr[col * SIZE_DATA + row];
    }
  }

  return dataReMixed;
}

void compData(uint8_t arrA[][SIZE_DATA], uint8_t arrB[][SIZE_DATA]) {
  printf("Перевірка: \n");
  uint8_t ctr = 0;
  for (size_t i = 0; i < SIZE_DATA; i++) {
    for (size_t j = 0; j < SIZE_DATA; j++) {
      if (arrA[i][j] != arrB[i][j]) {
        ctr++;
        // Якщо хоча б одна розбіжність
        if (ctr == 1) {
          printf("Елементи row, col [i,j] не співпадають: \n");
        }
        if (ctr > 0) {
          printf("[%d,%d] ", (uint8_t)i, (uint8_t)j);
          if ((ctr % 10) == 0)
            printf("\n");
        }
      }
    }
  }

  if (ctr > 0) {
    printf("\nКількість неспівпадінь: %d\n\n", ctr);
  } else {
    printf("Всі елементи послідовностей співпадають \nРозбіжність = %d\n", ctr);
  }
}

int main() {
  // Початкова послідовність даних
  uint8_t data[SIZE_DATA][SIZE_DATA];

  // Заповнюємо даними
  fillData(data);

  printf("Передавач:\n");
  printf("P(i): ");
  printData(pi);

  printf("Початкова послідовність: \n");
  printData2D(data);

  // Перемішуємо початкові дані
  printf("Інтерлівірована послідовність: \n");
  uint8_t *dataInter = interleaver(*data, pi);
  printDataPassByPointer(dataInter);

  printf("Відновлена послідовність: \n");
  uint8_t *dataDeInter = deinterleaver(dataInter, pi);
  printDataPassByPointer(dataDeInter);

  // Приведення uint8_t* до  uint8_t(*)[SIZE_DATA]
  compData((uint8_t(*)[SIZE_DATA])data,
           (uint8_t(*)[SIZE_DATA])dataDeInter);

  // Тест
  // Приведення uint8_t* до  uint8_t(*)[SIZE_DATA]
  // compData((uint8_t(*)[SIZE_DATA])dataInter,
  //          (uint8_t(*)[SIZE_DATA])dataDeInter);

  // Звільнення пам'яті
  free(dataInter);
  free(dataDeInter);

  return 0;
}

/*
повідомлення для передачі
0 1 1 1 0 0 1 1

матриця даних
0 1 1 1
0 0 1 1

до передачі зчитуємо
0 0 1 0 1 1 1 1

пошкодження
0 0 [1 0] 1 1 1 1

на приймальній стороні заповнюємо по стовпчиках
0 [1] 1 1
0 [0] 1 1

зчитуємо по рядках
0 [1] 1 1 0 [0] 1 1
*/
