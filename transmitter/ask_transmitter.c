#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pigpio.h>

#define TX_PIN 17          // Broadcom (BCM) GPIO Pin 17
#define BIT_PERIOD_US 2000 // Duration of 1 bit in microseconds (500 baud)

// Transmits a single byte using Amplitude Shift Keying (OOK)
void transmit_byte(char byte) {
    // Transmit bits from Most Significant Bit (MSB) to Least Significant Bit (LSB)
    for (int i = 7; i >= 0; i--) {
        int bit = (byte >> i) & 1;

        if (bit == 1) {
            gpioWrite(TX_PIN, 1); // Carrier ON (Amplitude = Max)
        } else {
            gpioWrite(TX_PIN, 0); // Carrier OFF (Amplitude = 0)
        }
        
        gpioDelay(BIT_PERIOD_US); // Hold the state for the duration of the bit
    }
}

// Transmits an entire string message with a simple preamble
void transmit_message(const char *message) {
    int len = strlen(message);

    // 1. Preamble/Sync Pulse: Helps the receiver's automatic gain control (AGC) stabilize
    printf("Sending preamble...\n");
    for (int i = 0; i < 4; i++) {
        gpioWrite(TX_PIN, 1);
        gpioDelay(BIT_PERIOD_US);
        gpioWrite(TX_PIN, 0);
        gpioDelay(BIT_PERIOD_US);
    }

    // 2. Transmit the actual payload data
    printf("Transmitting data: '%s'\n", message);
    for (int i = 0; i < len; i++) {
        transmit_byte(message[i]);
    }

    // 3. Ensure the pin is left LOW after transmission finishes
    gpioWrite(TX_PIN, 0);
}

int main(int argc, char *argv[]) {
    // Initialize the pigpio library
    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio initialization failed.\n");
        return EXIT_FAILURE;
    }

    // Set the transmission pin as an OUTPUT
    gpioSetMode(TX_PIN, PI_OUTPUT);
    gpioWrite(TX_PIN, 0); // Initialize to LOW

    const char *payload = "HELLO";
    
    // Transmit the payload in a continuous loop
    while (1) {
        transmit_message(payload);
        gpioDelay(2000000); // Wait 2 seconds before repeating
    }

    // Clean up and terminate the library (unreachable in infinite loop)
    gpioTerminate();
    return EXIT_SUCCESS;
}
