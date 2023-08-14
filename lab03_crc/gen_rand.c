#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void genRandomSeq(int length) {
  srand(time(NULL)); // Ініціалізація генератора випадкових чисел зі значенням часу

  for (int i = 0; i < length; i++) {
    int bit = rand() % 2; // Генерувати випадкове число 0 або 1
    printf("%d", bit);
  }

  printf("\n");
}

int main() {
  int sequenceLength = 1000;

  genRandomSeq(sequenceLength);

  return 0;
}
