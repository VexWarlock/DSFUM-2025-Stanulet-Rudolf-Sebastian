void setup() {
  for (int i = 13; i >= 8; i--) {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  digitalWrite(8, (millis() / 200) & 1);
  digitalWrite(9, (millis() / 1000) & 1);

  int frame = (millis() / 500) % 8;
  int powered;
  if (frame <= 4)
    powered = frame;
  else 
    powered = 8 - frame;

  for (int led = 0; led < 4; led++) {
    digitalWrite(13 - led, (led < powered));
  }
}
