#include <stdio.h>

// Scrambler configuration
#define TAP1 0    // Feedback tap 1 (bit position)
#define TAP2 1    // Feedback tap 2 (bit position)

// Scrambler function
unsigned char scramble(unsigned char data, unsigned char *state) {
    unsigned char output = 0;

    // XOR the input data with the feedback bits
    output = data ^ (state[TAP1] ^ state[TAP2]);

    // Shift the state register
    state[TAP2] = state[TAP1];
    state[TAP1] = data;

    return output;
}

int main() {
    unsigned char data[] = {0x12, 0x34, 0x56, 0x78};  // Input data
    unsigned char state[2] = {0};                    // Scrambler state
    unsigned char output[sizeof(data)];              // Scrambled output data
    int i;

    // Scramble the input data
    for (i = 0; i < sizeof(data); i++) {
        output[i] = scramble(data[i], state);
    }

    // Display the scrambled output data
    printf("Scrambled data: ");
    for (i = 0; i < sizeof(output); i++) {
        printf("%02X ", output[i]);
    }
    printf("\n");

    return 0;
}
