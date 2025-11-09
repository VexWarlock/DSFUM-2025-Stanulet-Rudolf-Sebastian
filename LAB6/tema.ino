int MOSI_pin = 11;
int SlaveSelect_pin = 10;
int Clock_pin = 13;

int pauza = 200;

byte max7219_reg_digit0      = 0x01;
byte max7219_reg_digit1      = 0x02;
byte max7219_reg_digit2      = 0x03;
byte max7219_reg_digit3      = 0x04;
byte max7219_reg_digit4      = 0x05;
byte max7219_reg_digit5      = 0x06;
byte max7219_reg_digit6      = 0x07;
byte max7219_reg_digit7      = 0x08;
byte max7219_reg_decodeMode  = 0x09;
byte max7219_reg_intensity   = 0x0a;
byte max7219_reg_scanLimit   = 0x0b;
byte max7219_reg_shutdown    = 0x0c;
byte max7219_reg_displayTest = 0x0f;

int grid[8][8];
int nextGrid[8][8];

void putByte(byte data) {
  for (int i = 7; i >= 0; i--) {
    digitalWrite(Clock_pin, LOW);
    digitalWrite(MOSI_pin, data & (1 << i));
    digitalWrite(Clock_pin, HIGH);
  }
}

void toMax(byte reg, byte col) {
  digitalWrite(SlaveSelect_pin, LOW);
  putByte(reg);
  putByte(col);
  digitalWrite(SlaveSelect_pin, HIGH);
}

void clearDisplay() {
  for (int i = 1; i <= 8; i++) toMax(i, 0);
}

void randomizeGrid(float prob = 0.3) {
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      grid[i][j] = (random(1000) < prob * 1000) ? 1 : 0;
}

int countNeighbors(int r, int c) {
  int count = 0;
  for (int dr = -1; dr <= 1; dr++) {
    for (int dc = -1; dc <= 1; dc++) {
      if (dr == 0 && dc == 0) continue;
      int rr = r + dr, cc = c + dc;
      if (rr >= 0 && rr < 8 && cc >= 0 && cc < 8)
        count += grid[rr][cc];
    }
  }
  return count;
}

void stepGame() {
  bool changed = false;
  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 8; c++) {
      int n = countNeighbors(r, c);
      int alive = grid[r][c];
      int newAlive = (alive && (n == 2 || n == 3)) || (!alive && n == 3);
      nextGrid[r][c] = newAlive;
      if (newAlive != alive) changed = true;
    }
  }
  for (int r = 0; r < 8; r++)
    for (int c = 0; c < 8; c++)
      grid[r][c] = nextGrid[r][c];
  if (!changed) randomizeGrid(0.35);
}

void displayGrid() {
  for (int r = 0; r < 8; r++) {
    byte val = 0;
    for (int c = 0; c < 8; c++)
      val |= (grid[r][7 - c] << c);
    toMax(r + 1, val);
  }
}

void setup() {
  pinMode(MOSI_pin, OUTPUT);
  pinMode(Clock_pin, OUTPUT);
  pinMode(SlaveSelect_pin, OUTPUT);
  digitalWrite(Clock_pin, HIGH);
  toMax(max7219_reg_scanLimit, 0x07);
  toMax(max7219_reg_decodeMode, 0x00);
  toMax(max7219_reg_shutdown, 0x01);
  toMax(max7219_reg_displayTest, 0x00);
  toMax(max7219_reg_intensity, 0x03);
  clearDisplay();
  randomSeed(analogRead(0));
  randomizeGrid(0.35);
}

void loop() {
  displayGrid();
  delay(pauza);
  stepGame();
}
