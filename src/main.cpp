#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    delay(2000);

    Serial.println("================================");
    Serial.println("ESP32-C6 SERIAL TEST");
    Serial.println("================================");
}

void loop() {
    Serial.println("ESP32 is running!");
    delay(1000);
}