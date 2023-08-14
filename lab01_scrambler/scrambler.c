#include <stdio.h>

#define SCRAMBLER_KEY 0x5A /* Ключ для скремблера */

void scramble(char *message, int length) {
  for (int i = 0; i < length; i++) {
    message[i] ^= SCRAMBLER_KEY;
    /* Використовуємо побітове XOR для скремблювання кожного
    символу повідомлення */
  }
}

int main() {
  char message[] = "Hello, World!";
  int length = sizeof(message) - 1;
  /* Визначаємо довжину повідомлення, не враховуючи 
  символ закінчення рядка '\0'*/

  printf("Початкове повідомлення: %s\n", message);

  scramble(message, length);

  printf("Скрембльоване повідомлення: %s\n", message);

  scramble(message, length);
  /* Для дешифрування достатньо знову застосувати 
  той самий скремблер */

  printf("Дешифроване повідомлення: %s\n", message);

  return 0;
}
