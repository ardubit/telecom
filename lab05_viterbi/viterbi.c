#include <stdio.h>

#define STATE_COUNT 4
#define INPUT_COUNT 2

// Transition matrix
int transitions[STATE_COUNT][INPUT_COUNT] = {
    {0, 2},  // State 0 transitions: 0->0, 1->2
    {1, 3},  // State 1 transitions: 0->1, 1->3
    {0, 2},  // State 2 transitions: 0->0, 1->2
    {1, 3}   // State 3 transitions: 0->1, 1->3
};

// Output matrix
int outputs[STATE_COUNT][INPUT_COUNT] = {
    {0, 1},  // State 0 outputs: 0->0, 1->1
    {1, 0},  // State 1 outputs: 0->1, 1->0
    {0, 1},  // State 2 outputs: 0->0, 1->1
    {1, 0}   // State 3 outputs: 0->1, 1->0
};

// Viterbi decoding function
void viterbiDecode(int* received, int len) {
    int path[len];
    int metric[len];
    int state = 0;

    for (int i = 0; i < len; i++) {
        int input = received[i];
        int prev_state = state;

        int best_metric = __INT_MAX__;
        int best_state = 0;

        for (int s = 0; s < STATE_COUNT; s++) {
            int metric_diff = outputs[s][input] ^ received[i];
            metric_diff += metric[i - 1] + (prev_state ^ s);

            if (metric_diff < best_metric) {
                best_metric = metric_diff;
                best_state = s;
            }
        }

        path[i] = best_state;
        metric[i] = best_metric;
        state = best_state;
    }

    // Backtracking to get decoded bits
    int decoded[len];
    int curr_state = state;

    for (int i = len - 1; i >= 0; i--) {
        decoded[i] = outputs[curr_state][received[i]];
        curr_state = path[i];
    }

    printf("Decoded output: ");
    for (int i = 0; i < len; i++) {
        printf("%d ", decoded[i]);
    }
    printf("\n");
}

int main() {
    int received[] = {0, 1, 1, 0, 1, 0};  // Received bits
    int len = sizeof(received) / sizeof(int);

    viterbiDecode(received, len);

    return 0;
}
