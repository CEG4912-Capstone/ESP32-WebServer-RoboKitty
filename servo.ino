
#include <ESP32Servo.h>
 
Servo myservo;  // create servo object to control a servo
// 16 servo objects can be created on the ESP32
 
int pos = 0;    // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
int servoPin = 13;
int inputPos = 0;
//Position 0 through 3
//Pos 0 -> no pen
//Pos 1 -> 1st pen colour
//Pos 2 -> 2nd pen colour
//Pos 3 -> 3rd pen colour
 
void setup() {
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(servoPin, 500, 2400); // attaches the servo on pin 18 to the servo object
  // using default min/max of 1000us and 2000us
  // different servos may require different min/max settings
  // for an accurate 0 to 180 sweep
}
 
void loop() {
 
  // Change pos according to input
 
  if (inputPos == 1) {
    //Pen position 1 (1st pen colour) -> between 90-135 deg
    pos = 112.5;
    myservo.write(pos); //Tell servo to go to 157.5 degrees
  }
  else if (inputPos == 2) {
    //Pen pos 2 (2nd colour) -> between 90-45 
    pos = 67.5;
    myservo.write(pos); //Tell servo to go to 157.5 degrees
  }
  else if (inputPos == 3) {
    //Pen pos 3 (3rd pen coloue) -> between 0 and 45
    pos = 22.5;
    myservo.write(pos); //Tell servo to go to 157.5 degrees
  }
  else {
    //No pen (pos 0)-> between 135 and 180
    pos = 157.5;
    myservo.write(pos); //Tell servo to go to 157.5 degrees
  }
}
 
}