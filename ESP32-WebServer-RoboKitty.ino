#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

// Network settings
const char* ssid = "Kavin";
const char* password = "12345678";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80); // Create AsyncWebServer on port 80

// Servo control setup
Servo myservo;
int servoPin = 13;
int pos = 0;  // Variable to store the servo position

// Stepper motor pins
const int DIR1 = 2;   // Direction pin for first stepper
const int STEP1 = 0;  // Step pin for first stepper
const int DIR2 = 14;  // Direction pin for second stepper
const int STEP2 = 12; // Step pin for second stepper

const int delay_time = 2000; // Stepper delay time

// Motor control functions
void forward(int steps) {
  digitalWrite(DIR1, HIGH);  // Motor 1 clockwise (forward)
  digitalWrite(DIR2, LOW);   // Motor 2 counter-clockwise (backward)
  Serial.println("Moving Forward...");

  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP1, HIGH);
    digitalWrite(STEP2, HIGH);
    delayMicroseconds(delay_time);
    digitalWrite(STEP1, LOW);
    digitalWrite(STEP2, LOW);
    delayMicroseconds(delay_time);
  }
}

void backward(int steps) {
  digitalWrite(DIR1, LOW);   // Motor 1 counter-clockwise (backward)
  digitalWrite(DIR2, HIGH);  // Motor 2 clockwise (forward)
  Serial.println("Moving Backward...");

  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP1, HIGH);
    digitalWrite(STEP2, HIGH);
    delayMicroseconds(delay_time);
    digitalWrite(STEP1, LOW);
    digitalWrite(STEP2, LOW);
    delayMicroseconds(delay_time);
  }
}

void left(int steps) {
  digitalWrite(DIR1, LOW);  // Motor 1 counter-clockwise
  digitalWrite(DIR2, HIGH); // Motor 2 clockwise
  Serial.println("Turning Left...");

  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP1, HIGH);
    digitalWrite(STEP2, HIGH);
    delayMicroseconds(delay_time);
    digitalWrite(STEP1, LOW);
    digitalWrite(STEP2, LOW);
    delayMicroseconds(delay_time);
  }
}

void right(int steps) {
  digitalWrite(DIR1, HIGH); // Motor 1 clockwise
  digitalWrite(DIR2, LOW);  // Motor 2 counter-clockwise
  Serial.println("Turning Right...");

  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP1, HIGH);
    digitalWrite(STEP2, HIGH);
    delayMicroseconds(delay_time);
    digitalWrite(STEP1, LOW);
    digitalWrite(STEP2, LOW);
    delayMicroseconds(delay_time);
  }
}

void handlePost(AsyncWebServerRequest *request) {
  // Extract the parameters from the POST request
  String movementType = request->getParam("movementType")->value();
  String direction = request->getParam("direction")->value();
  int steps = 0;
  
  // Check for stepper motor commands
  if (movementType == "Move") {
    steps = request->getParam("steps")->value().toInt();
    if (direction == "Forward") {
      forward(steps);
    } else if (direction == "Backward") {
      backward(steps);
    } else if (direction == "Right") {
      right(steps);
    } else if (direction == "Left") {
      left(steps);
    }
  }
  // Handle pen selection via servo
  else if (movementType == "Pen") {
    if (direction == "Color1") {
      pos = 92.5;  // Pen position 1 (1st pen color)
      myservo.write(pos);
      Serial.println("Pen: Color 1 selected");
    } else if (direction == "Color2") {
      pos = 55.5;  // Pen position 2 (2nd pen color)
      myservo.write(pos);
      Serial.println("Pen: Color 2 selected");
    } else if (direction == "Color3") {
      pos = 16.5;  // Pen position 3 (3rd pen color)
      myservo.write(pos);
      Serial.println("Pen: Color 3 selected");
    } else if (direction == "NoPen") {
      pos = 157.5;  // No pen position
      myservo.write(pos);
      Serial.println("Pen: No pen selected");
    }
  }

  // Send a response back to the client
  request->send(200, "text/plain", "Command processed");
}


void setup() {
  Serial.begin(115200);

  // Initialize stepper motor pins
  pinMode(STEP1, OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(STEP2, OUTPUT);
  pinMode(DIR2, OUTPUT);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  // Servo setup
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    
  myservo.attach(servoPin, 500, 2400); // Attach the servo to pin 13

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Stepper Motor Control");
  });

  // Handle POST requests to control movement
  server.on("/move", HTTP_POST, [](AsyncWebServerRequest *request) {
    handlePost(request);
  });

  // Start the server
  server.begin();
}

void loop() {
  // No need to handle clients manually, everything is handled asynchronously
}
