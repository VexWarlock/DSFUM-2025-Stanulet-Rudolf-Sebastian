//REALIZAT INTEGRAL DE GEMINI SI CHATGPT AI


#include <LedControl.h>
#include <avr/pgmspace.h>

// --- NOTE MUZICALE ---
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988

// --- MELODIE GAME OVER (TETRIS THEME - KOROBEINIKI) ---
// Stocata in PROGMEM
const int melody[] PROGMEM = {
  NOTE_E5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4,
  NOTE_A4, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
  NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
  NOTE_C5, NOTE_A4, NOTE_A4, 0,
  NOTE_D5, NOTE_F5, NOTE_A5, NOTE_G5, NOTE_F5,
  NOTE_E5, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
  NOTE_B4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
  NOTE_C5, NOTE_A4, NOTE_A4, 0
};

const int noteDurations[] PROGMEM = {
  4, 8, 8, 4, 8, 8,
  4, 8, 8, 4, 8, 8,
  4, 8, 4, 4,
  4, 4, 4, 4,
  4, 8, 4, 8, 8,
  4, 8, 4, 8, 8,
  4, 8, 8, 4, 4,
  4, 4, 4, 4
};

// --- HARDWARE ---
#define DIN_PIN 11
#define CLK_PIN 13
#define CS_PIN 10
#define NUM_DEVICES 8 

#define SR_DATA 9   
#define SR_LATCH 8  
#define SR_CLOCK 7  

const int pinRot = 41;   
const int pinLeft = 43;  
const int pinRight = 45; 
const int pinDown = 47;  
const int pinBuzzer = 49;

LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, NUM_DEVICES);

// --- CONFIGURARE JOC ---
const int WIDTH = 16;  
const int HEIGHT = 32; 
uint8_t field[HEIGHT][WIDTH];

unsigned long dropInterval = 600;
unsigned long lastDrop = 0;
unsigned long lastInput = 0;
bool melodyPlayed = false; // Flag pentru a nu repeta melodia la infinit

// Variabile display 7 segmente
unsigned long lastSegRefresh = 0;
int currentDigitIdx = 0;
byte displayDigits[4] = {0,0,0,0}; 

// --- PIESE ---
const uint8_t pieces[7][4][16] PROGMEM = {
  // I, O, T, L, J, S, Z definition...
  {{0,0,0,0, 1,1,1,1, 0,0,0,0, 0,0,0,0}, {0,0,1,0, 0,0,1,0, 0,0,1,0, 0,0,1,0}, {0,0,0,0, 0,0,0,0, 1,1,1,1, 0,0,0,0}, {0,1,0,0, 0,1,0,0, 0,1,0,0, 0,1,0,0}},
  {{0,0,0,0, 0,1,1,0, 0,1,1,0, 0,0,0,0}, {0,0,0,0, 0,1,1,0, 0,1,1,0, 0,0,0,0}, {0,0,0,0, 0,1,1,0, 0,1,1,0, 0,0,0,0}, {0,0,0,0, 0,1,1,0, 0,1,1,0, 0,0,0,0}},
  {{0,0,0,0, 1,1,1,0, 0,1,0,0, 0,0,0,0}, {0,1,0,0, 1,1,0,0, 0,1,0,0, 0,0,0,0}, {0,1,0,0, 1,1,1,0, 0,0,0,0, 0,0,0,0}, {0,1,0,0, 0,1,1,0, 0,1,0,0, 0,0,0,0}},
  {{0,0,0,0, 1,1,1,0, 1,0,0,0, 0,0,0,0}, {1,1,0,0, 0,1,0,0, 0,1,0,0, 0,0,0,0}, {0,0,1,0, 1,1,1,0, 0,0,0,0, 0,0,0,0}, {0,1,0,0, 0,1,0,0, 0,1,1,0, 0,0,0,0}},
  {{0,0,0,0, 1,1,1,0, 0,0,1,0, 0,0,0,0}, {0,1,0,0, 0,1,0,0, 1,1,0,0, 0,0,0,0}, {1,0,0,0, 1,1,1,0, 0,0,0,0, 0,0,0,0}, {0,1,1,0, 0,1,0,0, 0,1,0,0, 0,0,0,0}},
  {{0,0,0,0, 0,1,1,0, 1,1,0,0, 0,0,0,0}, {1,0,0,0, 1,1,0,0, 0,1,0,0, 0,0,0,0}, {0,0,0,0, 0,1,1,0, 1,1,0,0, 0,0,0,0}, {1,0,0,0, 1,1,0,0, 0,1,0,0, 0,0,0,0}},
  {{0,0,0,0, 1,1,0,0, 0,1,1,0, 0,0,0,0}, {0,1,0,0, 1,1,0,0, 1,0,0,0, 0,0,0,0}, {0,0,0,0, 1,1,0,0, 0,1,1,0, 0,0,0,0}, {0,1,0,0, 1,1,0,0, 1,0,0,0, 0,0,0,0}}
};

const byte digitPatterns[] = { 0xAF, 0x28, 0xCD, 0x6D, 0x2A, 0x67, 0xE7, 0x0C, 0xEF, 0x6F };
const byte digitSelectMask[] = { 0x20, 0x40, 0x80, 0x01 };

int curPiece = 0;
int curRot = 0;
int curX = 6; 
int curY = 0;
bool gameOver = false;
int scoreLines = 0;

// --- FUNCTII AUDIO ---

void playMoveSound() {
  tone(pinBuzzer, 400, 20); // Bip scurt
}

void playLineClearSound() {
  // 3 note muzicale (Do-Mi-Sol) rapid
  tone(pinBuzzer, NOTE_C5, 80); delay(80);
  tone(pinBuzzer, NOTE_E5, 80); delay(80);
  tone(pinBuzzer, NOTE_G5, 100); delay(100);
}

void playGameOverMelody() {
  // Citim din PROGMEM si redam
  int len = sizeof(melody) / sizeof(melody[0]);
  for (int thisNote = 0; thisNote < len; thisNote++) {
    // Citim nota si durata din memoria Flash
    int note = pgm_read_word_near(melody + thisNote);
    int durationType = pgm_read_word_near(noteDurations + thisNote);
    
    // Calcul durata (1000 ms / tipul notei)
    int noteDuration = 1000 / durationType;
    
    // La game over ne permitem delay blocant
    if(note == 0) {
      noTone(pinBuzzer);
      delay(noteDuration);
    } else {
      tone(pinBuzzer, note, noteDuration);
      delay(noteDuration * 1.30); // Mica pauza intre note
    }
  }
}

// --- FUNCTII JOC ---

void updateScore(int val) {
  if(val > 9999) val = 9999;
  displayDigits[0] = (val / 1000) % 10;
  displayDigits[1] = (val / 100) % 10;
  displayDigits[2] = (val / 10) % 10;
  displayDigits[3] = val % 10;
}

void refresh7Seg() {
  digitalWrite(SR_LATCH, LOW);
  shiftOut(SR_DATA, SR_CLOCK, MSBFIRST, digitSelectMask[currentDigitIdx]); 
  shiftOut(SR_DATA, SR_CLOCK, MSBFIRST, digitPatterns[displayDigits[currentDigitIdx]]);   
  digitalWrite(SR_LATCH, HIGH);
  currentDigitIdx = (currentDigitIdx + 1) % 4;
}

uint8_t getPiecePixel(int piece, int rot, int idx) {
  return pgm_read_byte(&pieces[piece][rot][idx]);
}

bool readButton(int pin) { return digitalRead(pin) == HIGH; }

void mapToDisplay(int gx, int gy, int &dev, int &row, int &col) {
  if (gx < 0 || gx >= WIDTH || gy < 0 || gy >= HEIGHT) {
    dev = -1; return;
  }
  int rowBlock = gy / 8;
  int localY = gy % 8;

  if (gx >= 8) {
    dev = rowBlock; 
    int localX = gx - 8; 
    row = localX; col = localY; 
  } else {
    dev = 7 - rowBlock; 
    int localX = gx;
    row = 7 - localX; col = 7 - localY; 
  }
}

bool collide(int testX, int testY, int testRot) {
  for(int py=0; py<4; py++){
    for(int px=0; px<4; px++){
      if(getPiecePixel(curPiece, testRot, py*4+px)){
        int gx = testX + px;
        int gy = testY + py;
        if(gx < 0 || gx >= WIDTH || gy >= HEIGHT) return true;
        if(gy >= 0 && field[gy][gx]) return true;
      }
    }
  }
  return false;
}

void spawnPiece() {
  curPiece = random(0, 7);
  curRot = 0;
  curX = 6; 
  curY = -2;
  if(collide(curX, curY, curRot)) {
    gameOver = true;
  }
}

void lockPiece() {
  for(int py=0; py<4; py++){
    for(int px=0; px<4; px++){
      if(getPiecePixel(curPiece, curRot, py*4+px)){
        int gx = curX + px;
        int gy = curY + py;
        if(gy >= 0 && gx >= 0 && gx < WIDTH && gy < HEIGHT) {
          field[gy][gx] = 1;
        }
      }
    }
  }

  int lines = 0;
  for(int y = HEIGHT - 1; y >= 0; y--) {
    bool full = true;
    for(int x = 0; x < WIDTH; x++) if(!field[y][x]) { full = false; break; }

    if(full) {
      lines++;
      for(int yy = y; yy > 0; yy--) {
        for(int x = 0; x < WIDTH; x++) field[yy][x] = field[yy-1][x];
      }
      for(int x = 0; x < WIDTH; x++) field[0][x] = 0;
      y++; 
    }
  }

  if(lines > 0) {
    scoreLines += lines * 10; 
    updateScore(scoreLines);
    
    // --- CERINTA: 3 NOTE MUZICALE LA LINIE ---
    playLineClearSound();
    
    dropInterval = max(100, 600 - (scoreLines * 2)); 
  }
}

void render() {
  for(int d=0; d<NUM_DEVICES; d++) {
    for(int r=0; r<8; r++) lc.setRow(d, r, 0);
  }
  for(int y=0; y<HEIGHT; y++) {
    for(int x=0; x<WIDTH; x++) {
      bool on = field[y][x];
      if(!on && !gameOver) {
        int py = y - curY;
        int px = x - curX;
        if(py>=0 && py<4 && px>=0 && px<4) {
          if(getPiecePixel(curPiece, curRot, py*4+px)) on = true;
        }
      }
      if(on) {
        int dev, r, c;
        mapToDisplay(x, y, dev, r, c);
        if(dev >= 0) lc.setLed(dev, r, c, true);
      }
    }
  }
}

void resetGame() {
  memset(field, 0, sizeof(field));
  scoreLines = 0;
  updateScore(0);
  dropInterval = 600;
  gameOver = false;
  melodyPlayed = false; // Resetam flag-ul pentru melodie
  spawnPiece();
}

void setup() {
  randomSeed(analogRead(A0)); 

  for(int i=0; i<NUM_DEVICES; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 4);
    lc.clearDisplay(i);
  }

  pinMode(pinRot, INPUT);
  pinMode(pinLeft, INPUT);
  pinMode(pinRight, INPUT);
  pinMode(pinDown, INPUT);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(SR_DATA, OUTPUT);
  pinMode(SR_LATCH, OUTPUT);
  pinMode(SR_CLOCK, OUTPUT);

  resetGame();
}

void loop() {
  unsigned long now = millis();

  if(now - lastSegRefresh > 3) { 
    refresh7Seg();
    lastSegRefresh = now;
  }

  if(gameOver) {
    // --- CERINTA: MELODIE LA FINAL ---
    if(!melodyPlayed) {
      playGameOverMelody();
      melodyPlayed = true; // Cantam o singura data
    }

    if(readButton(pinRot)) resetGame();
    return;
  }

  if(now - lastInput > 120) {
    if(readButton(pinLeft)) {
      if(!collide(curX - 1, curY, curRot)) { 
        curX--; 
        render(); 
        playMoveSound(); // --- CERINTA: SUNET LA MISCARE ---
      }
      lastInput = now;
    }
    if(readButton(pinRight)) {
      if(!collide(curX + 1, curY, curRot)) { 
        curX++; 
        render(); 
        playMoveSound(); // --- CERINTA: SUNET LA MISCARE ---
      }
      lastInput = now;
    }
    if(readButton(pinRot)) {
      int newRot = (curRot + 1) % 4;
      if(!collide(curX, curY, newRot)) { 
        curRot = newRot; 
        render(); 
        playMoveSound(); // --- CERINTA: SUNET LA MISCARE ---
      }
      lastInput = now;
    }
  }

  bool fast = readButton(pinDown);
  unsigned long currentSpeed = fast ? 50 : dropInterval;
  
  // Sunet la coborare manuala (Fast Drop)
  if(fast && (now - lastDrop > currentSpeed)) playMoveSound();

  if(now - lastDrop > currentSpeed) {
    lastDrop = now;
    if(!collide(curX, curY + 1, curRot)) {
      curY++;
      render();
    } else {
      lockPiece();
      spawnPiece();
      render();
    }
  }
}
