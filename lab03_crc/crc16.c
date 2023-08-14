#include <stdint.h>
#include <stdio.h>

const uint16_t polynomial = 0x8BB7; // Polynomial for CRC-16-T10-DIF

// Function to calculate the CRC-16-T10-DIF checksum
uint16_t calculateCRC(uint16_t *data) {
    size_t length = sizeof(data);
    uint16_t crc = 0xFFFF;

    for (size_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i] << 8; // XOR with next data byte

        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ polynomial; // Perform the XOR with polynomial if MSB is set
            } else {
                crc <<= 1; // Shift left if MSB is not set
            }
        }
    }

    return crc;
}

int main() {
    // Sample data
    uint16_t data = 0xFFFF;

    // Calculate the CRC checksum
    uint16_t crc = calculateCRC(&data);

    // Display the CRC checksum in hexadecimal format
    printf("CRC checksum: 0x%04X\n", crc);

    // Calculate the CRC checksum
    uint16_t crc2 = calculateCRC(&data);

    // Display the CRC checksum in hexadecimal format
    printf("CRC checksum: 0x%04X\n", crc2);

    return 0;
}
