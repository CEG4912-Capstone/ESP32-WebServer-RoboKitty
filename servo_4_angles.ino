#include <Servo.h>

// Define the servo pin
#define SERVO_PIN 28

// Create a Servo object
Servo myservo;

// Predefined angles for the servo
int angle1 = 25;  // Angle 1 (min)
int angle2 = 70;  // Angle 2
int angle3 = 100;  // Angle 3
int angle4 = 145; // Angle 4 (max)

void setup() {
  // Attach the servo to the defined pin
  myservo.attach(SERVO_PIN);

  // Set the servo to the initial position (angle1)
  myservo.write(90);
  delay(2000); // Wait for 1 second
}

void loop() {
  // Move the servo to the predefined angles with a delay
  myservo.write(angle1); // Move to angle1
  delay(2000);           // 1 second delay

  myservo.write(angle2); // Move to angle2
  delay(2000);           // 1 second delay

  myservo.write(angle3); // Move to angle3
  delay(2000);           // 1 second delay

  myservo.write(angle4); // Move to angle4
  delay(2000);           // 1 second delay
}
