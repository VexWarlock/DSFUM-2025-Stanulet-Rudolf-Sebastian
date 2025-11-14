const int latchPin = 8;
const int clockPin = 12;
const int dataPin  = 11;

byte digits[10] = {
  B00111111,
  B00000110,
  B01011011,
  B01001111,
  B01100110,
  B01101101,
  B01111101,
  B00000111,
  B01111111,
  B01101111
};

byte positions[4] = {
  B00001000,
  B00000100,
  B00000010,
  B00000001
};

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, digits[i]);
    shiftOut(dataPin, clockPin, LSBFIRST, positions[0]);
    digitalWrite(latchPin, HIGH);
    delay(1000);
  }
}
