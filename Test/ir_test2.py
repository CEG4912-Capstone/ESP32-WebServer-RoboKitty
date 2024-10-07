void setup() {
  pinMode(13, OUTPUT);  // internal LED
  pinMode(2, INPUT);    // IR input
}

void loop() {
  int v = digitalRead(2);
  digitalWrite(13, !v);	// the ! symbol inverts the value
  delay(50);
}