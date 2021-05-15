#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ArduinoBlue.h>

const unsigned long BAUD_RATE = 9600;

// The bluetooth tx and rx pins must be supported by software serial.
// Visit https://www.arduino.cc/en/Reference/SoftwareSerial for unsupported pins.
// * Checking this doc shows that on the Mega 2560, we are OK to use both pins 10 and 11 for either RX or TX.
// Bluetooth TX -> Arduino D8
const int BLUETOOTH_TX_PIN = 10; // Transmit FROM BT TO Arduino
const int BLUETOOTH_RX_PIN = 11; // Receive AT BT FROM Arduino
// BLUETOOTH_RX_PIN is the one which you likely have your voltage divider applied to, in order to convert the Arduino
// logic out at 5V down to the HM-10 logic in at 3.3V. (Better have this voltage divider or you may fry your HM-10.)

// Avoiding use of pin 13 because we may use the on board LED so I think that pin has to remain unused for that.
// TODO: Clarify on LED and pin 13.

SoftwareSerial bluetooth(BLUETOOTH_TX_PIN, BLUETOOTH_RX_PIN);
ArduinoBlue phone(bluetooth); // pass reference of bluetooth object to ArduinoCommander.

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    // Start serial communications.
    // The baud rate must be the same for both serials.
    Serial.begin(BAUD_RATE);
    bluetooth.begin(BAUD_RATE);

    Serial.println("setup complete");
}


void loop() {
    // Four quick flashes then a long flash
    digitalWrite(LED_BUILTIN, HIGH);
    delay(20);
    digitalWrite(LED_BUILTIN, LOW);
    delay(20);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(20);
    digitalWrite(LED_BUILTIN, LOW);
    delay(20);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(20);
    digitalWrite(LED_BUILTIN, LOW);
    delay(20);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(20);
    digitalWrite(LED_BUILTIN, LOW);
    delay(20);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(600);
}

/**
 * CLion PlatformIO vs CMake Important Notes:
 * These points apply to projects created in CLion using the PlatformIO Plug-In and a standalone (Homebrew)
 * installation of platformio. CMake is not used for builds but there are CMake files and directories
 * created and used by the CLion IDE, but not by PlatformIO. This is important to understand.
 * Although all build, upload, debug and related actions are performed entirely by PlatformIO, the CLion IDE
 * itself uses the CMake system simultaneously for providing real-time code-analysis feedback in the Editor
 * and potentially other features. While you should not need to maintain much in CMake files, for includes
 * to work correctly you will at least need to create a CMakeListsUser.txt file and add a little code to
 * it so that the IDE/CMake can locate the includes. For example, for the addition of the ArduinoBlue library
 * in main.cpp (specifically for #include <ArduinoBlue.h>, we also need the following added to a
 * new CMakeListsUser.txt file:
 * if (CMAKE_BUILD_TYPE MATCHES "megaatmega2560")
 *     include_directories("${CMAKE_CURRENT_LIST_DIR}/.pio/libdeps/megaatmega2560/ArduinoBlue/src")
 */
