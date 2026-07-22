#include <RH_ASK.h>
#include <pigpio.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) 
{
    // Initialize the pigpio library
    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio initialisation failed\n");
        return 1;
    }

    // Set up RH_ASK driver
    // Parameters: speed (bps), rxPin, txPin, pttPin, pttInverted
    // Default Raspberry Pi pins: RX = 11, TX = 12
    RH_ASK driver(2000, 11, 12, 10, false);

    // Initialize the RadioHead driver
    if (!driver.init()) {
        fprintf(stderr, "RadioHead init failed\n");
        return 1;
    }

    printf("RadioHead ASK transmitter initialized.\n");

    // Message to send
    const char *msg = "Hello World";
    
    while (1) {
        printf("Sending: %s\n", msg);
        
        // Send the message (cast to uint8_t*)
        driver.send((uint8_t *)msg, strlen(msg));
        
        // Wait until the entire message is transmitted
        driver.waitPacketSent();
        
        // Wait 1 second before sending the next packet
        sleep(1);
    }

    // Clean up pigpio resources before exiting
    gpioTerminate();
    return 0;
}