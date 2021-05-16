#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ArduinoBlue.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo

const unsigned long BAUD_RATE = 9600;

const int ANALOG_SERVO_PWM_PIN = 5;

int servo_angle = 0;  // TODO: Maybe don't declare it here. Let's see where it is really used tho.

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

// Initialize variables for app controls
int prevThrottle = 49;
int prevSteering = 49;
int throttle, steering, sliderVal, button, sliderId;

SoftwareSerial bluetooth(BLUETOOTH_TX_PIN, BLUETOOTH_RX_PIN);
ArduinoBlue phone(bluetooth); // pass reference of bluetooth object to ArduinoCommander.


void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    myservo.attach(ANALOG_SERVO_PWM_PIN);  // attaches the servo on pin 9 to the servo object

    // Start serial communications.
    // The baud rate must be the same for both serials.
    Serial.begin(BAUD_RATE);
    bluetooth.begin(BAUD_RATE);

    // delay just in case bluetooth module needs time to "get ready".
    delay(200);

    Serial.println("setup complete");
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
    if (sliderId != -1) {
        Serial.print("Slider ID: ");
        Serial.print(sliderId);
        Serial.print("\tValue: ");
        Serial.println(sliderVal);
        servo_angle = map(sliderVal, 0, 200, 0, 180);  // scale to use with the servo (value between 0 and 180)
        Serial.print("\tServo angle: ");
        Serial.println(servo_angle);
        myservo.write(servo_angle);  // sets the servo position according to the scaled value
        delay(100);
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
 */
