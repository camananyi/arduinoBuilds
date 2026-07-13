#include <LiquidCrystal.h>

const int NUM_MOLES = 8;

// pins
int leds[NUM_MOLES] = {37, 39, 41, 43, 45, 49, 51, 53};
int buttons[NUM_MOLES] = {36, 38, 40, 42, 44, 48, 50, 52};

// LCD
// const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
// LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// game variables
int score = 0;
unsigned long gameStartTime;
bool gameOver = false;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < NUM_MOLES; i++) {
    pinMode(leds[i], OUTPUT);
    pinMode(buttons[i], INPUT); 
  }

  randomSeed(analogRead(A0));

  // lcd.begin(16, 2);
  // lcd.print("Whack a Mole!");

  // lcd.clear();
  // lcd.print("Score: 0");

  gameStartTime = millis();
}

void displayScoreboard() {

  Serial.println("================================");
  Serial.println("      WHACK-A-MOLE ARCADE");
  Serial.println("================================");

  Serial.print("SCORE: ");
  Serial.println(score);

  Serial.println("================================");
  Serial.println();
}

void updateScore() {
  // lcd.clear();
  // lcd.setCursor(0, 0);
  // lcd.print("Score: ");
  // lcd.print(score);

  // Serial.println(score);
  displayScoreboard();
}

void loop() {

  // timer
  if (millis() - gameStartTime >= 20000) {
    gameOver = true;
  }

  // game over flashing
  if (gameOver) {
    for (int i = 0; i < NUM_MOLES; i++) {
      digitalWrite(leds[i], HIGH);
    }
    delay(250);

    for (int i = 0; i < NUM_MOLES; i++) {
      digitalWrite(leds[i], LOW);
    }
    delay(250);

    return;
  }

  // pick random LED
  int activeIndex = random(0, NUM_MOLES);
  digitalWrite(leds[activeIndex], HIGH);

  delay(30); // small stability delay

  unsigned long startTime = millis();
  int randomOnTime = random(350, 700);

  bool alreadyPressed = false;

  while (millis() - startTime < randomOnTime) {

    for (int i = 0; i < NUM_MOLES; i++) {

      if (digitalRead(buttons[i]) == HIGH && !alreadyPressed) {

        delay(20); // debounce

        if (digitalRead(buttons[i]) == HIGH) {

          alreadyPressed = true;

          if (i == activeIndex) {
            // Serial.print("Correct: ");
            score++; // correc
            // Serial.println(score);
          }
          updateScore();
          digitalWrite(leds[activeIndex], LOW);

          // wait until button released
          while (digitalRead(buttons[i]) == HIGH);

          break;
        }
      }
    }

    if (alreadyPressed) break;
  }

  // turn off LED (no penalty if missed)
  digitalWrite(leds[activeIndex], LOW);

  delay(random(100, 300));
}