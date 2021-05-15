#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ArduinoBlue.h>

/**
 * CLion PlatformIO vs CMake Important Clarification:
 * SMake is not used for builds, debugging and uploading in a CLion PlatformIO Plugin project, HOWEVER,
 * CMake (or at least some of it's files like CMakeListsPrivate.txt) *IS* used but the CLion IDE/Editor
 * for real-time code analysis and suggestions, particularly related to library location.
 * For example, for the IDE to be able to locate the ArduinoBlue.h file, I had to add this line to the
 * appropriate part of the CMakeListsPrivate.txt file:
 * include_directories("${CMAKE_CURRENT_LIST_DIR}/.pio/libdeps/megaatmega2560/ArduinoBlue/src")
 * Other similar steps may be needed for any 3rd-party library added to a project.
 * TODO: Oops! still learning. Not supposed to change this file. PlatformIO can overwrite this file and it
 * will exist in freshly-created empty projects etc. Proper procedure is to create and add changes/additions
 * into a "CMakeListsUser.txt" file. Will fix this soon. This will be a file we will add to the repo. It is not
 * excluded at the moment so all we need to do is add it, but it might need a conditional around the line we
 * added to be logically correct based on what exists currently in CMakeListsPrivate.txt
 */

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
