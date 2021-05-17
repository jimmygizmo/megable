#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ArduinoBlue.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo

const unsigned long BAUD_RATE = 9600;

const int ANALOG_SERVO_AS1_PWM_PIN = 54;
// On the Mega2560: pin 5 is a PWM pin. pin 54 is analog 0.
// Servo chatter while moving problem seems worse on pin 5 as compared to pin 54. Inconclusive.

const int ANALOG_SERVO_AS1_MINIMUM_ANGLE = 4; // To minimize or stop chatter at the minimum servo position,
// we'll limit the lowest angle value to the value configured here. This value may vary slightly depending on
// the individual servo, the exact voltage being used and other factors. Tune/adjust this for each specific servo.

const int ANALOG_SERVO_AS1_MAXIMUM_ANGLE = 175; // Actual servo travel is almost 195 physical degrees in my case,
// so we will limit the travel in code to right at about 180-182 degrees. It turns out that having that extra travel
// (15 degrees extra to play with) will also help with eliminating chatter that always seems to occur with all
// servos at or near their minimum position. NOTE: The term 'ANGLE' in the variable names is never exact. It refers
// to a decimal value that approximates the desired angular position of the servo which is highly subjective and varies
// widely between servos and conditions. Additionally, we may modify this value during code execution to limit or
// scale it among other things. The only true 'angle' of the position of a servo is one you physically measure and
// calibrate for and even then there will be some variance during operation.

const int ANALOG_SERVO_AS1_INITIAL_ANGLE = ANALOG_SERVO_AS1_MINIMUM_ANGLE;  // initialize at the minimum angle

// The bluetooth tx and rx pins must be supported by software serial.
// Visit https://www.arduino.cc/en/Reference/SoftwareSerial for unsupported pins.
// * Checking this doc shows that on the Mega 2560, we are OK to use both pins 10 and 11 for either RX or TX.
// Bluetooth TX -> Arduino D8
const int BLUETOOTH_TX_PIN = 10; // Transmit FROM the Bluetooth module TO Arduino
const int BLUETOOTH_RX_PIN = 11; // Receive AT the Bluetooth module FROM Arduino
// BLUETOOTH_RX_PIN is the one which you likely have your voltage divider applied to, in order to convert the Arduino
// logic out at 5V down to the HM-10 logic in at 3.3V. (Better have this voltage divider or you may fry your HM-10.)

// Avoiding use of pin 13 because we may use the on board LED so I think pin 13 has to remain unused for that.
// TODO: Clarify details on LED and pin 13.

// Initialize variables for app controls
int prevThrottle = 49;
int prevSteering = 49;
int throttle, steering, sliderVal, button, sliderId;

SoftwareSerial bluetooth(BLUETOOTH_TX_PIN, BLUETOOTH_RX_PIN);
ArduinoBlue phone(bluetooth); // pass reference of bluetooth object to ArduinoCommander.


void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    // Start serial communications.
    // The baud rate must be the same for both serials.
    Serial.begin(BAUD_RATE);
    bluetooth.begin(BAUD_RATE);

    // delay just in case bluetooth module needs time to "get ready".
    delay(200);

    Serial.println("Bluetooth initialization complete.");

    myservo.attach(ANALOG_SERVO_AS1_PWM_PIN);  // attaches the servo on the configured pin to the servo object

    Serial.print("\tMoving servo AS1 to initial (minimum chatter-free) position: ");
    Serial.println(ANALOG_SERVO_AS1_INITIAL_ANGLE);
    myservo.write(ANALOG_SERVO_AS1_INITIAL_ANGLE);

    Serial.println("Analog servo AS1 initialization complete.");
}


void loop() {
    // delay(200); // Experimental delay had no effect on the bad servo chattering.
    // ID of the button pressed.
    button = phone.getButton();

    // Returns the text data sent from the phone.
    // After it returns the latest data, empty string "" is sent in subsequent.
    // calls until text data is sent again.
    String str = phone.getText();

    // If a text from the phone was sent print it to the serial monitor
    if (str != "") {
        Serial.println(str);
    }

    // Throttle and steering values go from 0 to 99.
    // When throttle and steering values are at 99/2 = 49, the joystick is at center.
    throttle = phone.getThrottle();
    steering = phone.getSteering();

    // ID of the slider moved.
    sliderId = phone.getSliderId();

    // Slider value goes from 0 to 200.
    sliderVal = phone.getSliderVal();

    // Display button data whenever its pressed.
    if (button != -1) {
        Serial.print("Button: ");
        Serial.println(button);
    }

    // Display slider data when slider moves
    int servo_angle;
    int adjusted_servo_angle;
    if (sliderId != -1) {
        Serial.print("Slider ID: ");
        Serial.print(sliderId);
        Serial.print("\tValue: ");
        Serial.println(sliderVal);
        servo_angle = map(sliderVal, 0, 200, 0, 180);  // scale to use with the servo (value between 0 and 180)
        Serial.print("\tServo angle: ");
        Serial.println(servo_angle);

        if ( servo_angle < ANALOG_SERVO_AS1_MINIMUM_ANGLE ) {
            adjusted_servo_angle = ANALOG_SERVO_AS1_MINIMUM_ANGLE;
        } else if ( servo_angle > ANALOG_SERVO_AS1_MAXIMUM_ANGLE ) {
            adjusted_servo_angle = ANALOG_SERVO_AS1_MAXIMUM_ANGLE;
        } else {
            adjusted_servo_angle = servo_angle;
        }

        Serial.print("\tAdjusted servo angle: ");
        Serial.println(adjusted_servo_angle);
        myservo.write(adjusted_servo_angle);  // sets the servo position according to the scaled and adjusted value
        delay(15);  // Give a little time for servo movement to occur. Such a delay might need to be tuned.
    }

    // Display throttle and steering data if steering or throttle value is changed
    if (prevThrottle != throttle || prevSteering != steering) {
        Serial.print("Throttle: ");
        Serial.print(throttle);
        Serial.print("\tSteering: ");
        Serial.println(steering);
        prevThrottle = throttle;
        prevSteering = steering;
    }

    // Send string from serial command line to the phone. This will alert the user.
    if (Serial.available()) {
        Serial.write("send: ");
        String strUsbSer = Serial.readString();
        phone.sendMessage(strUsbSer); // phone.sendMessage(strUsbSer) sends the text to the phone.
        Serial.print(strUsbSer);
        Serial.write('\n');
    }


//    old code from flashing the LED
//    delay(500);
//    digitalWrite(LED_BUILTIN, LOW);
//    delay(500);

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
 *
 * Important realization about the interaction. I have seen items in CMake files which refer to items inside of
 * the .pio directory structure, specifically the installed libdeps files for added (3rd party) libraries. Also,
 * Although the build and upload/run buttons are configured via the PlatformIO plugin, the log output when they
 * are run show reference to the cmake installed with the CLion.app inside the bin directory of the app Contents.
 * (Just a side-note, but I also have a slightly /usr/local/bin/cmake which does not appear to be used by CLion
 * and I am guessing it comes from Homebrew and could be a dependency for multiple things.)
 * (Another side-note: I cannot rule out that some mode of using PlatformIO might use this /usr/local/bin/cmake.)
 * So the bottom line is I am seeing that BOTH PlatformIO AND CMake are being used by CLion+PlatformIO projects
 * and currently, when you add a 3rd-party library via the PlatformIO Home/GUI (I don't personally add libraries
 * any other way yet) well then you will have to manually update your own CMakeListsUser.txt file as noted above.
 *
 * I will continue to update here with relevant things I discover about using CLion for Arduino/embedded development.
 *
 */
