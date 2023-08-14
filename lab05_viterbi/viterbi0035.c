#include <stdio.h>

// Define the trellis
struct Trellis {
    char input[3];
    char state[3];
};

// Define state transition probabilities
double transition_probs[4][4] = {
    {0.5, 0.5, 0.0, 0.0},
    {0.0, 0.0, 0.5, 0.5},
    {0.5, 0.5, 0.0, 0.0},
    {0.0, 0.0, 0.5, 0.5}
};

// Define branch metrics
int branch_metrics[8][2] = {
    {0, 1},
    {1, 0},
    {1, 0},
    {0, 1},
    {0, 1},
    {1, 0},
    {0, 1},
    {1, 0}
};

// Function to decode the input sequence
void viterbi_decoder(char** input_sequence, char** expected_output_sequence, int sequence_length) {
    struct Trellis trellis[9] = {
        {"00", "S0"},
        {"11", "S3"},
        {"10", "S2"},
        {"00", "S0"},
        {"01", "S1"},
        {"10", "S2"},
        {"01", "S1"},
        {"11", "S3"},
        {"00", "S0"}
    };

    char state_sequence[10] = "S0";  // Start with the initial state
    char decoded_output_sequence[10] = "";

    for (int i = 0; i < sequence_length; i++) {
        char* input_symbol = input_sequence[i];
        char* output_symbol = expected_output_sequence[i];
        char* current_state = &state_sequence[i * 2];

        // Calculate branch metrics for the current input and output symbols
        int branch_metric_0 = branch_metrics[i][0];
        int branch_metric_1 = branch_metrics[i][1];

        // Calculate the accumulated path metrics for each possible state transition
        double path_metrics[4] = {0.0};
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                if (trellis[k].state[0] == current_state[0] && trellis[k].state[1] == current_state[1]) {
                    path_metrics[k] = branch_metric_0 * transition_probs[j][k];
                    path_metrics[k] += branch_metric_1 * transition_probs[j + 2][k];
                }
            }
        }

        // Choose the state with the maximum path metric
        int max_index = 0;
        double max_metric = path_metrics[0];
        for (int j = 1; j < 4; j++) {
            if (path_metrics[j] > max_metric) {
                max_index = j;
                max_metric = path_metrics[j];
            }
        }

        // Update the state sequence and decoded output sequence
        state_sequence[(i + 1) * 2] = trellis[max_index].state[0];
        state_sequence[(i + 1) * 2 + 1] = trellis[max_index].state[1];
        decoded_output_sequence[i] = trellis[max_index].state[1];
    }

    // Print the decoded sequence
    printf("Decoded sequence: %s\n", decoded_output_sequence);
}

int main() {
    char* input_sequence[] = {"0x0", "0x3", "0x2", "0x0", "0x1", "0x2", "0x1", "0x3", "0x0"};
    char* expected_output_sequence[] = {"0x0", "0x1", "0x0", "0x1", "0x1", "0x1", "0x0", "0x0", "0x0"};
    int sequence_length = sizeof(input_sequence) / sizeof(input_sequence[0]);

    viterbi_decoder(input_sequence, expected_output_sequence, sequence_length);

    return 0;
}
