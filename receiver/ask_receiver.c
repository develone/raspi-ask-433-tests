#include <wiringPi.h>
#include <stdio.h>

#define RX_PIN 0 // Corresponds to WiringPi pin 0 (GPIO 17 / Physical Pin 11)

int main() {
    if (wiringPiSetup() == -1) {
        printf("Setup wiringPi failed!");
        return 1;
    }

    pinMode(RX_PIN, INPUT);
    printf("Listening for ASK signals...\n");

    unsigned long lastTime = micros();
    
    while (1) {
        // Read the current state of the receiver's data pin
        int state = digitalRead(RX_PIN);
        
        // If the pin goes HIGH (carrier wave detected)
        if (state == HIGH) {
            unsigned long duration = micros() - lastTime;
            printf("HIGH signal received for %lu microseconds\n", duration);
            lastTime = micros();
            while(digitalRead(RX_PIN) == HIGH); // Wait for pin to go LOW
        } 
        // If the pin goes LOW (no carrier wave)
        else {
            unsigned long duration = micros() - lastTime;
            printf("LOW signal received for %lu microseconds\n", duration);
            lastTime = micros();
            while(digitalRead(RX_PIN) == LOW); // Wait for pin to go HIGH
        }
    }
    return 0;
}