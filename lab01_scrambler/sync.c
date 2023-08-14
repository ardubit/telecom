#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_SIZE 10
#define SCRAMBLER_SIZE 4

void scramble(int *data, int size, int *scrambler, int scrambler_size) {
    int feedback;
    for (int i = 0; i < size; i++) {
        feedback = (data[i] + scrambler[i % scrambler_size]) % 2;
        data[i] = feedback;
        for (int j = scrambler_size - 1; j > 0; j--) {
            scrambler[j] = scrambler[j - 1];
        }
        scrambler[0] = feedback;
    }
}

int main() {
    int data[DATA_SIZE] = {1, 0, 1, 1, 0, 0, 1, 1, 0, 1};
    int scrambler[SCRAMBLER_SIZE] = {1, 0, 0, 1};
    
    // Scramble the data using the scrambler sequence
    scramble(data, DATA_SIZE, scrambler, SCRAMBLER_SIZE);
    
    // Print the scrambled data and the scrambler sequence
    printf("Scrambled data: ");
    for (int i = 0; i < DATA_SIZE; i++) {
        printf("%d", data[i]);
    }
    printf("\nScrambler sequence: ");
    for (int i = 0; i < SCRAMBLER_SIZE; i++) {
        printf("%d", scrambler[i]);
    }
    printf("\n");
    
    return 0;
}