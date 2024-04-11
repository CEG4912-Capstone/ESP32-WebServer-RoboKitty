#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// IR Sensor
bool estop = false;
int irSensor = 16;
int irSensor1 = 17;


// add your network settings
const char* ssid = "Kavin";
const char* password = "12345678";

WebServer server(80);

const int ledPin26 = 26;
const int ledPin27 = 27;

bool backwardState = false;


// Motor variables
// Motor A

int motor1Pin1 = 27;

int motor1Pin2 = 26;

int enable1Pin = 14;

int motor2Pin1 = 25;

int motor2Pin2 = 33;

int enable2Pin = 15;

// Setting PWM properties

const int freq = 50000;

const int pwmChannel = 0;
const int pwmChannel1 = 1;

const int resolution = 8;

int dutyCycle = 190;

void IRAM_ATTR ISR() {
  Serial.println("Interrupt: Motor stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);

  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  estop = true;
}

// handle preflight request options
void handleOptions() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  server.send(204); // No Content
}

void handlePost() {
  //allow origin
  server.sendHeader("Access-Control-Allow-Origin", "*");

  if (server.hasArg("plain") == false) { // check if body received
    server.send(200, "application/json", "{\"error\":\"Body not received\"}");
    return;
  }

  String message = server.arg("plain"); // get the body
  Serial.print("POST Body: ");
  Serial.println(message);

  // get json response
  DynamicJsonDocument doc(1024 * 4);
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    server.send(400, "text/plain", "Invalid JSON");
    return;
  }

  //list of commands
  const char* test = doc["movementType"];
  Serial.println(test);
  JsonArray commands = doc.as<JsonArray>();

  for (JsonObject command : commands) {
    detectIR();

    if (estop)
      break;
    // important command data
    int steps = (int) command["steps"];
    const char* movementType = command["movementType"]; // 'Move' | 'Turn'
    const char* directionType = command["direction"]; // 'Forward' | 'Backward' | 'Right' | 'Left'

    // gotta process command after command to preserve the sequence
    if (strcmp(movementType, "Move") == 0) {
      if (strcmp(directionType, "Forward") == 0) {

        // robot.forward(steps)
        Serial.println(steps);
        forward(steps);


      } else if (strcmp(directionType, "Backward") == 0) {

        // robot.backward(steps)
        backward(steps);
        //Serial.println("Backward: "+steps);

      }

    } else if (strcmp(movementType, "Turn") == 0) {
      if (strcmp(directionType, "Right") == 0) {

        // robot.right(steps)
        right(steps);
        //Serial.println("Turn right: "+steps);

      } else if (strcmp(directionType, "Left") == 0) {

        // robot.left(steps)
        left(steps);
        Serial.println("Turn left: " + steps);

      }
    }

  }
  server.send(200, "application/json", "{\"message\":\"request received\"}");

}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin26, OUTPUT);
  pinMode(ledPin27, OUTPUT);

  pinMode(irSensor, INPUT_PULLUP);
  pinMode(irSensor1, INPUT_PULLUP);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", getHtml());
  });

  server.on("/control", HTTP_GET, []() {
    if (server.hasArg("led") && server.hasArg("state")) {
      int ledPin = server.arg("led") == "26" ? ledPin26 : ledPin27;
      digitalWrite(ledPin, server.arg("state") == "on" ? HIGH : LOW);
    }
    server.sendHeader("Location", "/", true); // redirect back
    server.send(303);
  });

  // request from website
  server.on("/post", HTTP_POST, handlePost);
  server.on("/post", HTTP_OPTIONS, handleOptions); // Handle preflight requests


  attachInterrupt(irSensor, ISR, RISING);
  attachInterrupt(irSensor1, ISR, RISING);

  // start
  server.begin();

  // sets the pins as outputs:

  pinMode(motor1Pin1, OUTPUT);

  pinMode(motor1Pin2, OUTPUT);

  pinMode(enable1Pin, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);

  pinMode(motor2Pin2, OUTPUT);

  pinMode(enable2Pin, OUTPUT);

  // configure LED PWM functionalitites

  ledcSetup(pwmChannel, freq, resolution);

  // attach the channel to the GPIO to be controlled

  ledcAttachPin(enable1Pin, pwmChannel);

  ledcAttachPin(enable2Pin, pwmChannel1);

  Serial.begin(115200);

  // testing

  Serial.print("Testing DC Motor...");
}

void loop() {



  if (!estop) {
    server.handleClient();

    if (backwardState)  {
      ledcWrite(pwmChannel, 235); // right
      ledcWrite(pwmChannel1, 245); // left
    } else {
      ledcWrite(pwmChannel, 230);
      ledcWrite(pwmChannel1, 250);
    }
  } else {
    stopMotor();
  }


}

void detectIR() {
  int state  = digitalRead(irSensor);
  //Serial.println(state);

  if (state == 1) {
    estop = true;
    Serial.println("Black detected");
  }

  if (estop) {
    stopMotor();
  }
}

void forward(int steps) {

  Serial.println("Moving Forward");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);

  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  delay(steps * 500);

  stopMotor();
}

void stopMotor() {
  Serial.println("Motor stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);

  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}




void backward(int steps) {
  backwardState = true;
  Serial.println("Moving Backwards");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);

  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  delay(steps * 500);

  stopMotor();
  backwardState = false;

}

void right(int steps) {
  Serial.println("Moving Left");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);

  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);

  delay(steps / 90 * 1000);

  stopMotor();
}

void left(int steps) {
  Serial.println("Moving Right");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);

  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);

  delay(steps / 90 * 1000);

  stopMotor();
}



String getHtml() {
  //default web page
  String html = R"=====(
    <!DOCTYPE html>
    <html>
    <head>
      <title>ESP32 LED Control</title>
    </head>
    <body>
      <h2>ESP32 LED Control</h2>
      <p>
        <a href="/control?led=26&state=on"><button>LED 26 ON</button></a>
        <a href="/control?led=26&state=off"><button>LED 26 OFF</button></a>
      </p>
      <p>
        <a href="/control?led=27&state=on"><button>LED 27 ON</button></a>
        <a href="/control?led=27&state=off"><button>LED 27 OFF</button></a>
      </p>
    </body>
    </html>
  )=====";
  return html;
}
