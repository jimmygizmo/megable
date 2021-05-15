#include <Arduino.h>

/**
 * Prior to any HM-10/BLE code, let's just get a Blink program going since this is a brand new IDE and build setup.
 */

void setup() {
// write your initialization code here
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
// write your code here
    // Three quick flashes then a long flash
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
}
