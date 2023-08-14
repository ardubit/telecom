#include <stdio.h>

#define CODE_RATE 2
#define CONSTRAINT_LENGTH 3

int main() {
    int input[] = {0, 1, 0, 1, 0}; // Вхідний бітовий потік
    int output[CODE_RATE * sizeof(input) / sizeof(int)]; // Вихідний кодовий потік

    int state[CONSTRAINT_LENGTH] = {0}; // Початковий стан зсувних регістрів

    int i, j, k, outputIndex = 0;
    for (i = 0; i < sizeof(input) / sizeof(int); i++) {
        int inputBit = input[i];

        // Кодування кожного біту
        for (j = 0; j < CODE_RATE; j++) {
            int outputBit = 0;

            // Обчислення нового вихідного біту
            for (k = 0; k < CONSTRAINT_LENGTH; k++) {
                outputBit ^= state[k]; // Використовується операція XOR
            }

            // Зсув регістрів
            for (k = CONSTRAINT_LENGTH - 1; k > 0; k--) {
                state[k] = state[k - 1];
            }
            state[0] = inputBit;

            output[outputIndex++] = outputBit;
        }
    }

    // Виведення результату кодування
    printf("Input: ");
    for (i = 0; i < sizeof(input) / sizeof(int); i++) {
        printf("%d ", input[i]);
    }
    printf("\n");

    printf("Output: ");
    for (i = 0; i < sizeof(output) / sizeof(int); i++) {
        printf("%d ", output[i]);
    }
    printf("\n");

    return 0;
}
