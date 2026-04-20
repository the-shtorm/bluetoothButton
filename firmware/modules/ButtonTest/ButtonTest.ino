#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  pinMode(13, INPUT_PULLUP);
  Serial.println("Hardware Test: Touch Pin 13 to GND");
}

void loop() {
  if (digitalRead(13) == LOW) {
    Serial.println("GPIO 13 is LOW (Button Pressed)");
    delay(200); // Simple debounce
  }
}