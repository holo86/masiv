#include <Arduino_LED_Matrix.h>
ArduinoLEDMatrix ser;
int y = 4;
int x = 9;
int left;
int right;
int up;
int down;
int i;
int j;
int score = 0;
int scorast;
int povorot;

int angle;
byte frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte frame1[8][12] = {
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 }
};
byte b[3][2] = {
  { 1, 1 },
  { 1, 1 },
  { 1, 1 }
};

void clear_line() {
  bool full = true;
  for (int i = 0; i < 8; i++)
    if (frame[i][0] == 0) {
      full = false;
      break;
    }
  if (full) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 11; j++)
        frame[i][j] = frame[i][j + 1];
      frame[i][11] = 0;
    }
    score += 100;
  }

  Serial.print("Score: ");
  Serial.println(score);
}

void game_over() {
  for (int i = 0; i < 8; i++) {
    bool full = true;
    for (int j = 0; j < 11; j++)
      if (frame[i][j] == 0) {
        full = false;
        break;
      }
    if (full) {
      ser.renderBitmap(frame1, 8, 12); 
      Serial.println("Game over:");
      delay(10000);
      break;
    }
  }
}


void kn(int data) {
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 2; j++)
      switch (angle) {
        case 0:
          {
            if ((b[i][j] == 1)&&((y + i)<8)&&((x+j)<12))
              frame[y + i][x + j] = data;
            break;
          }
        case 1:
          {
            if ((b[2 - i][j] == 1)&&((y + j)<8)&&((x+i)<12))
              frame[y + j][x + i] = data;

            break;
          }
        case 2:
          {
            if ((b[2 - i][1-j] == 1)&&((y + i)<8)&&((x+j)<12))
              frame[y + i][x + j] = data;

            break;
          }
        case 3:
          {
            if ((b[i][1-j] == 1)&&((y + j)<8)&&((x+i)<12))
              frame[y + j][x + i] = data;
            break;
          }
        default:
          {
            break;
          }
      }
}

void setup() {

  Serial.begin(9600);
  ser.begin();
  ser.renderBitmap(frame, 8, 12);
  ser.renderBitmap(frame1, 8, 12);
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
}

void loop() {
  kn(0);
  i++;
  int axel;
  Serial.println(digitalRead(2));
  if (digitalRead(2) == 1) axel = 1;
  else axel = 0;
  if (i >= (10 + axel * 90)) {

    x--;
    i = 0;
  }

  //up
  if (digitalRead(1) == 0) {
    if (up == 0)
      if ((y > 0) && (frame[y - 1][x] == 0))
        y--;

    up = 1;

  } else up = 0;

  //down
  if (digitalRead(0) == 0) {
    if (down == 0)
      if ((y < 7) && (frame[y + 1][x] == 0))
        y++;
    down = 1;
  } else down = 0;
  if (digitalRead(3) == 0) {
    if (povorot == 0) {
      angle++;
      angle %= 4;
      povorot = 1;
    }
  } else povorot = 0;
  kn(1);
  delay(10);
  if ((x <= 0) || (frame[y][x - 1] == 1)) {
    clear_line();
    game_over();
    x = 11;
    y = 1;
    frame[y][x] = 1;
  }



  ser.renderBitmap(frame, 8, 12);
}
