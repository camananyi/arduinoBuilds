#include <LiquidCrystal.h>
bool toClose = false;
String advice[] = {
  "Keep your eyes away from the screen",
  "Good posture is beneficial",
  "Take care of your eyes",
  "Take care of your posture",
  "Avoid back pain"
};
int randomNumber = 0;
int buttonPin = 8;
int distance;
long duration;

#define TRIG_PIN 9
#define ECHO_PIN 10
#define BUZZER_PIN 7
const int DISTANCE_THRESHOLD = 50;


const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int SCREEN_WIDTH = 16;

void setup() {
  lcd.begin(16, 2);
  pinMode(buttonPin, INPUT)
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

}

void loop() {
  int distance;
  digitalWrite(TRIG_PIN, LOW);
  delay(2);
  digitalWrite(TRIG_PIN, HIGH);
  delay(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034/2;


  if (distance > 0 && distance < DISTANCE_THRESHOLD)
  {
    toClose = true;
  }
  else
  {
    digitalWrite(BUZZER_PIN, LOW);
    toClose = false;
  }
  delay(500);

  if (toClose) {
    digitalWrite(BUZZER_PIN, HIGH);
    int randomNumber = random(0, 5);
    displayMessage(advice[randomNumber], 0, 200);
  } else {
    lcd.clear();
  }
}


void displayMessage(String message, int row, int delayTime) {
  int textLength = message.length();
  
  // Pad the front with spaces so it starts off-screen to the right
  String spaces = "";
  for (int i = 0; i < SCREEN_WIDTH; i++) {
    spaces += " ";
  }
  String fullScrollText = spaces + message;
  
  // Total loops needed = text length + screen width
  int totalSteps = textLength + 1; // try deleting SCREEN_WIDTH
  long timeOn = millis();

  for (int position = 0; position < totalSteps; position++) {
    distance = duration * 0.034/2;
    if (position >= 10)
    {
      digitalWrite(BUZZER_PIN, LOW);
    }
    lcd.setCursor(0, row);
    lcd.print("                ");
    lcd.setCursor(0, row);
    
    // Grab a slice of text exactly equal to the screen width
    String currentFrame = fullScrollText.substring(position, position + SCREEN_WIDTH);
    
    lcd.print(currentFrame);
    delay(delayTime); // Controls scrolling speed (lower is faster)
  }
}

