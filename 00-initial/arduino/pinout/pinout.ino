void setup() {
  // put your setup code here, to run once:
  pinMode(A8, OUTPUT);
  pinMode(A9, OUTPUT);
  pinMode(A10, OUTPUT);
  pinMode(A11, OUTPUT);

  pinMode(A12, INPUT);
  pinMode(A13, INPUT);
  pinMode(A14, INPUT);
  pinMode(A15, INPUT);

  Serial.begin(19200);
}

void loop() {
  digitalWrite(A8, digitalRead(A12));
  digitalWrite(A9, digitalRead(A13));
  digitalWrite(A10, digitalRead(A14));
  digitalWrite(A11, digitalRead(A15));
}
