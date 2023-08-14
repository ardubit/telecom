// Вказівники у виразах використовуються для отримання доступу до значень, що
// знаходяться у пам'яті за певною адресою. Основні операції з вказівниками
// включають отримання адреси змінної, розіменування та арифметику вказівників.

// Отримання адреси змінної: Для отримання адреси змінної використовується
// оператор "&". Наприклад, якщо ми маємо змінну "x", то "&x" поверне адресу
// цієї змінної.

// Розіменування: Розіменування вказівника означає отримання значення, до якого
// вказівник вказує. Для розіменування використовується оператор "*". Наприклад,
// якщо у нас є вказівник "ptr", то "*ptr" поверне значення, до якого вказівник
// вказує.

// Приклади використання вказівників у виразах:

int main() {
  int x = 5;
  int *ptr = &x; // Оголошуємо вказівник і присвоюємо йому адресу змінної x

  printf("Адреса x: %p\n", &x);
  printf("Значення ptr: %p\n", ptr);
  printf("Значення x: %d\n", *ptr); // Розіменовуємо вказівник і виводимо значення x

  int y = *ptr + 10; // Вираз з використанням розіменованого вказівника
  printf("Значення y: %d\n", y);

  int *ptr2 = ptr + 1; // Арифметика вказівників
  return 0;
}

// У цьому прикладі ми оголосили змінну "x" і вказівник "ptr", якому присвоєна
// адреса змінної "x". Ми вивели адресу "x", значення "ptr" (яке є адресою "x")
// і розіменували вказівник, щоб вивести значення "x". Також показано
// використання розіменованого вказівника в виразі, де ми додали 10 до значення
// "x" і присвоїли це значення змінній "y". Також згадана арифметика
// вказівників, де ми присвоїли новому вказівнику "ptr2" значення "ptr + 1", що
// означає адресу, яка знаходиться наступною за адресою "x" у пам'яті.