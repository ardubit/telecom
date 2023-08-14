#include <stdio.h>
#include <string.h>

/*
Self-sync Scrambler
100 1000 100 1

Sync Additive Scrambler
100 1000 100 1
x1 + X4 + X8 + x11 polynomial

Shift Reg
000 0100 100
*/

char *str = "telecom";
int data_in[] = {1, 1, 1, 1, 0, 0, 0, 0};
int data_out[sizeof(data_in)];
int errors = 0;

// Поліном визначає відведення
// #define TAP1
// #define TAP2
// #define TAP3

int main(int argc, char const *argv[]) {
  printf("%s\n", str);
  printf("Початкові дані: %d\n", data_in);

  printf("Результат скремблювання, Self-sync (multiplicative): %d\n", );
  printf("Результат дескремблювання, Self-sync (multiplicative): %d\n", );
  printf("Помилки: %d, %d%% \n", errors, errors / (int)sizeof(data_in) * 100);

  printf("Результат скремблювання, Sync (additive): %d\n", );
  printf("Результат дескремблювання, Sync (additive): %d\n", );
  printf("Помилки: %d, %d%% \n", errors, errors / (int)sizeof(data_in) * 100);
  
  return 0;
}
