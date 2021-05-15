#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ArduinoBlue.h>

/**
 * CLion PlatformIO vs CMake Important Notes:
 * These points apply to projects created in CLion using the PlatformIO Plug-In and a standalone (Homebrew)
 * installation of platformio. CMake is not used for builds but there are make CMake files and directories
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


void setup() {
// write your initialization code here
    pinMode(LED_BUILTIN, OUTPUT);
}


void loop() {
// write your code here
    // Three quick flashes then a long flash
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
