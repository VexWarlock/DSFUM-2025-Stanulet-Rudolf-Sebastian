#define p_tip_buzzer 4    // Buzzer principal
#define p_r1 5            // LED1
#define p_r2 6            // LED2

#define NOTE_C4 261
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494

#define NOTE_C5 523
#define NOTE_D5 587
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_G5 784

#define REST 0 // Pauză

// Melodia "Happy Birthday to You"
int melody[] = {
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4,
  REST,
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4,
  REST,
  NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4,
  REST,
  NOTE_F4, NOTE_G4, NOTE_F4,
  REST
};


int durations[] = {
  150, 300, 300, 300, 300, 600,
  100,
  150, 300, 300, 300, 300, 600,
  100,
  150, 300, 300, 300, 300, 300, 600,
  100,
  150, 300, 300, 300, 300, 600,
  500
};

const int NUM_NOTES = sizeof(melody)/sizeof(melody[0]);

void setup() {
  pinMode(p_tip_buzzer, OUTPUT);
  pinMode(p_r1, OUTPUT);
  pinMode(p_r2, OUTPUT);
}

void loop() {
  for(int i = 0; i < NUM_NOTES; i++){
    playNote(i);
  }
  delay(1500);
}

void playNote(int idx){
  int noteDuration = durations[idx];

  if(melody[idx] == REST){
    noTone(p_tip_buzzer);
    digitalWrite(p_r1, LOW);
    digitalWrite(p_r2, LOW);
  } else {
    tone(p_tip_buzzer, melody[idx]);
    
    if(idx % 2 == 0){
      digitalWrite(p_r1, HIGH);
      digitalWrite(p_r2, LOW);
    } else {
      digitalWrite(p_r1, LOW);
      digitalWrite(p_r2, HIGH);
    }
  }

  delay(noteDuration * 0.9);

  noTone(p_tip_buzzer);
  digitalWrite(p_r1, LOW);
  digitalWrite(p_r2, LOW);

  delay(noteDuration * 0.1);
}
