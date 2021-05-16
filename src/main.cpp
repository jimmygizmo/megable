#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ArduinoBlue.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo

const unsigned long BAUD_RATE = 9600;

const int ANALOG_SERVO_AS1_PWM_PIN = 5;  // On the Mega2560, pin 5 is a PWM pin.

const int ANALOG_SERVO_AS1_MINIMUM_ANGLE = 4; // To minimize or stop chatter at the minimum servo position,
// we'll limit the lowest angle value to the value configured here. This value may vary slightly depending on
// the individual servo, the exact voltage being used and other factors. Tune/adjust this for each specific servo.

const int ANALOG_SERVO_AS1_MAXIMUM_ANGLE = 170; // Actual servo travel is almost 195 degrees in my case, so we will
// limit it to right at about 180-182 degrees and having that extra travel will also help with eliminating chatter
// that always seems to occur with all servos at or near their minimum position.
// TODO: Address the semantics/meaning of the 'ANGLE' in the variable name now at this point. Since we know
// most servos can be very inaccurate in terms of true angle values, we might want to just call this 'position'
// and then mention in a few places that it roughly corresponds to angle, but that we are modifying (limiting
// and or scaling etc.) values to achieve the desired behavior and simplest/clearest code as well.
// So the TODO might be to just replace the word 'ANGLE' with 'POSITION' and add some appropriate comments.
// The terminology 'ANGLE' can be used near the input and more accurately this is 'DESIRED ANGLE' or 'REQUESTED ANGLE'.
// It is good to pay attention to the semantics of variable naming. Downstream developers will thank you for it and
// you will avoid confusion and even bugs by have well-named variables, objects, functions, classes etc.

const int ANALOG_SERVO_AS1_INITIAL_ANGLE = ANALOG_SERVO_AS1_MINIMUM_ANGLE;  // initialize at the minimum angle

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
        // delay(100);  // Experimental delay does not seem to affect the chattering during slow value change.
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

// Experimental delay to try to get rid of "chatter during slow movement", which is the big problem at the moment.
// (Very soon now I will start investigating electromagnetic interference, possibly from the Bluetooth traffic,
// affecting the servo signal or the servo in some other way. Currently these are very close to each other and the
// servo wiring is long and going in some big loops nearby so interference is a very real possibility.
delay(200);

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
