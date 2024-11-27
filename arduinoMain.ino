#include <AccelStepper.h>
#include <Servo.h>

// Pin definitions
#define STEP 14
#define DIR 15
#define STEP2 17
#define DIR2 16
#define MICROSTEP1 6
#define MICROSTEP2 7
#define SERVO_PIN 28

int angle1 = 25;  // Angle 1 (min)
int angle2 = 70;  // Angle 2
int angle3 = 100;  // Angle 3
int angle4 = 145; // Angle 4 (max)

// Stepper motor objects
AccelStepper stepper1(AccelStepper::FULL2WIRE, STEP, DIR);
AccelStepper stepper2(AccelStepper::FULL2WIRE, STEP2, DIR2);

// Servo object
Servo myservo;

// Constants
const int rev = 6400;    // Steps per full rotation
const float b = 6400;   // Base width in steps
const float h = 6400;   // Height in steps
const float dp = -3000;   // Displacement distance for pen lift or reset (move back after spinning)
const float d = 2700; // keep going forward a little 

// 90 degrees is around 3900 steps
// Helper function to rotate the robot in degrees
void rotate(int degrees) {
    // Positive degrees rotate CCW, negative rotate CW
        stepper1.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);
    int steps = 4000;
    stepper1.moveTo(steps);
    stepper2.moveTo(steps);
    while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) {
       stepper1.run();
        stepper2.run();
    }
}

void rotateRoof1() {
    stepper1.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);
    int steps = 1333;
    stepper1.moveTo(steps);
    stepper2.moveTo(steps);
    while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) {
       stepper1.run();
        stepper2.run();
    }
}

void rotateRoof2() {
  stepper2.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);

  int steps = 5332;
    stepper1.move(steps);
    stepper2.move(steps);
    while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) {
      stepper1.run();
      stepper2.run();
    }
}
 
// Helper function to move the robot straight in a direction
void moveStraight(float distance) {
    stepper1.setCurrentPosition(0);
    stepper2.setCurrentPosition(0);
    stepper1.moveTo(-distance);
    stepper2.moveTo(distance);
    while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) {
        stepper1.run();
        stepper2.run();
    }
}

// Helper function to lift or lower the pen
void penControl(bool lift, int slot) {
    if (lift) {
        // Lift the pen (e.g., position 0 degrees)
        myservo.write(angle3); // Adjust value based on servo's behavior
    } else {
        // Lower the pen (e.g., position 90 degrees)

        if (slot == 1) {
          myservo.write(angle1);  // Adjust value based on servo's behavior
        } else if (slot == 2) {
          myservo.write(angle2);  // Adjust value based on servo's behavior

        } else if (slot == 4) {
          myservo.write(angle4);  // Adjust value based on servo's behavior
        }
    }
}

void setup() {
    // Stepper motor configuration
    stepper1.setMaxSpeed(3200.0);
    stepper1.setAcceleration(3200.0);
    stepper2.setMaxSpeed(3200.0);
    stepper2.setAcceleration(3200.0);

    // Microstepping configuration
    pinMode(MICROSTEP1, OUTPUT);
    digitalWrite(MICROSTEP1, HIGH);

    pinMode(MICROSTEP2, OUTPUT);
    digitalWrite(MICROSTEP2, HIGH);


    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    // Servo initialization
    myservo.attach(SERVO_PIN);
    myservo.write(100); // Initialize with pen lifted
}

int stepsNum = 0;
void loop() {
    static int counter = 0; // Persistent state variable for the loop
    int steps1 = 1333;

    switch (counter) {
        case 0: // Move b units to the right
            // straight
            penControl(false, 1);

            digitalWrite(LED_BUILTIN, HIGH);
            moveStraight(b);
            counter++;
            break;

        case 1: // Rotate 90 degrees CCW, lift pen
            digitalWrite(LED_BUILTIN, LOW);
            // turns to the right
            penControl(true, 3);
            moveStraight(d);
            rotate(90);
            // go back a little
            moveStraight(dp);
            counter++;
            break;

        case 2: // Move b units to the right
            // straight
            penControl(false, 1);

            digitalWrite(LED_BUILTIN, HIGH);
            moveStraight(b);
            counter++;
            break;

        case 3: // Rotate 90 degrees CCW, lift pen
            digitalWrite(LED_BUILTIN, LOW);
            // turns to the right
            penControl(true, 3);
            moveStraight(d);
            rotate(90);
            // go back a little
            moveStraight(dp);
            counter++;
            break;
        
        case 4: // Move b units to the right
            // straight
            penControl(false, 1);

            digitalWrite(LED_BUILTIN, HIGH);
            moveStraight(b);
            counter++;
            break;

        case 5: // Rotate 90 degrees CCW, lift pen
            digitalWrite(LED_BUILTIN, LOW);
            // turns to the right
            penControl(true, 3);
            moveStraight(d);
            rotate(90);
            // go back a little
            moveStraight(dp);
            counter++;
            break;

        case 6: // Move h units up
            digitalWrite(LED_BUILTIN, HIGH);

            penControl(false, 1);
            // go straight
            moveStraight(b);
            counter++;
            break;

        case 7: // Rotate 60 degrees CCW, move h units at angle
            digitalWrite(LED_BUILTIN, LOW);

            // build roof
            //rotate(60);
            penControl(true, 3);
            moveStraight(d);
            rotateRoof1();
            moveStraight(dp);
            penControl(false, 4);
            moveStraight(h);
            counter++;
            break;

        case 8: // Rotate 120 degrees CCW, move h units at angle
            digitalWrite(LED_BUILTIN, HIGH);
            // go down (build right side of roof)
            //rotate(120);
            penControl(true, 3);
            delay(1000);
            moveStraight(d);
            rotateRoof2();
            moveStraight(dp);
            delay(1000);
            penControl(false, 4);
            moveStraight(h);
            counter++;
            break;

        // case 5: // Rotate 30 degrees CCW to return to vertical, move down
        //     digitalWrite(LED_BUILTIN, LOW);
        //     //rotate(30);
        //     //rotateRoof1();
        //     stepper2.setCurrentPosition(0);
        //     stepper2.setCurrentPosition(0);

        //     penControl(true);
        //     moveStraight(d);
        //     stepper1.move(steps1);
        //     stepper2.move(steps1);
        //     while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) {
        //       stepper1.run();
        //       stepper2.run();
        //       }
        
        //     penControl(true);
        //     moveStraight(dp);
        //     penControl(false);
        //     moveStraight(b); // Move back down to start
        //     counter++;      // Restart loop or halt
        //     break;

        

        default:
            break;
    }
}
