const int OFF = 0, BLINK = 1, FADE_IN = 2, FADE_OUT = 3, ON_STATIC = 4, FADE_LOOP = 5;
const int PINS[] = {8, 9, 10, 11, 12};
const int NUM_PINS = sizeof(PINS) / sizeof(PINS[0]);

int modes[NUM_PINS];
int brightness[NUM_PINS];
unsigned long timers[NUM_PINS];
unsigned long prevMillis[NUM_PINS];
bool ledStates[NUM_PINS];

const int MAX_STRING_LENGTH = 30;
char serialBuffer[MAX_STRING_LENGTH];
int bufferIndex = 0;
bool commandReceived = false;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_PINS; i++) {
    pinMode(PINS[i], OUTPUT);
    digitalWrite(PINS[i], LOW);
    modes[i] = OFF;
    brightness[i] = 0;
    timers[i] = 0;
    prevMillis[i] = 0;
    ledStates[i] = LOW;
  }
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      serialBuffer[bufferIndex] = '\0';
      commandReceived = true;
      break;
    }
    if (bufferIndex < MAX_STRING_LENGTH - 1)
      serialBuffer[bufferIndex++] = c;
  }

  if (commandReceived) {
    commandReceived = false;
    bufferIndex = 0;
    Serial.print("Received command: ");
    Serial.println(serialBuffer);

    char temp[MAX_STRING_LENGTH];
    strcpy(temp, serialBuffer);
    char* range = strtok(temp, ",");
    char* modeStr = strtok(NULL, ",");
    char* timerStr = strtok(NULL, ",");

    if (!range || !modeStr || !timerStr) {
      Serial.println("Invalid command format.");
      return;
    }

    int startIndex, endIndex;
    int mode = OFF;
    int timer = atoi(timerStr);

    if (strchr(range, '-')) {
      startIndex = atoi(strtok(range, "-"));
      endIndex = atoi(strtok(NULL, "-"));
    } else {
      startIndex = endIndex = atoi(range);
    }

    if (startIndex < 0 || endIndex >= NUM_PINS || startIndex > endIndex) {
      Serial.println("Invalid index range!");
      return;
    }

    if      (!strcmp(modeStr, "blink"))   mode = BLINK;
    else if (!strcmp(modeStr, "fadein"))  mode = FADE_IN;
    else if (!strcmp(modeStr, "fadeout")) mode = FADE_OUT;
    else if (!strcmp(modeStr, "on"))      mode = ON_STATIC;
    else if (!strcmp(modeStr, "off"))     mode = OFF;
    else if (!strcmp(modeStr, "fade"))    mode = FADE_LOOP;
    else {
      Serial.println("Unknown mode!");
      return;
    }

    unsigned long currentTime = millis();
    for (int i = startIndex; i <= endIndex; i++) {
      int pin = PINS[i];
      modes[i] = mode;
      timers[i] = timer;
      prevMillis[i] = currentTime;
      ledStates[i] = LOW;
      brightness[i] = (mode == FADE_OUT || mode == FADE_LOOP) ? 255 : 0;

      analogWrite(pin, brightness[i]);
      digitalWrite(pin, LOW);

      Serial.print("Pin ");
      Serial.print(pin);
      Serial.print(": Mode ");
      Serial.print(modeStr);
      Serial.print(", Timer ");
      Serial.println(timer);
    }

    memset(serialBuffer, 0, MAX_STRING_LENGTH);
  }

  unsigned long now = millis();

  for (int i = 0; i < NUM_PINS; i++) {
    int pin = PINS[i];

    switch (modes[i]) {
      case BLINK:
        if (now - prevMillis[i] >= timers[i]) {
          prevMillis[i] = now;
          ledStates[i] = !ledStates[i];
          digitalWrite(pin, ledStates[i]);
        }
        break;

      case FADE_IN:
        if (brightness[i] < 255) {
          brightness[i] = map(now - prevMillis[i], 0, timers[i], 0, 255);
          analogWrite(pin, constrain(brightness[i], 0, 255));
          if (brightness[i] >= 255) {
            modes[i] = ON_STATIC;
          }
        }
        break;

      case FADE_OUT:
        if (brightness[i] > 0) {
          brightness[i] = map(now - prevMillis[i], 0, timers[i], 255, 0);
          analogWrite(pin, constrain(brightness[i], 0, 255));
          if (brightness[i] <= 0) {
            modes[i] = OFF;
          }
        }
        break;

      case FADE_LOOP:
        if (brightness[i] <= 0) {
          prevMillis[i] = now;
          modes[i] = FADE_IN;
        } else if (brightness[i] >= 255) {
          prevMillis[i] = now;
          modes[i] = FADE_OUT;
        }

        if (modes[i] == FADE_IN) {
          brightness[i] = map(now - prevMillis[i], 0, timers[i], 0, 255);
          analogWrite(pin, constrain(brightness[i], 0, 255));
        } else if (modes[i] == FADE_OUT) {
          brightness[i] = map(now - prevMillis[i], 0, timers[i], 255, 0);
          analogWrite(pin, constrain(brightness[i], 0, 255));
        }
        break;

      case ON_STATIC:
        analogWrite(pin, 255);
        break;

      case OFF:
        digitalWrite(pin, LOW);
        break;
    }
  }
}
