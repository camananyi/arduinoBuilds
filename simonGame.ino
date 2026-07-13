struct Color {
  int buttonPin;
  int ledPin;
  int tone;
};

Color colors[4] = {
  { 6, 2, 500 },  // red
  { 7, 3, 650 },  // yellow
  { 8, 4, 800 },  // blue
  { 9, 5, 950 },  // green
};

const int buzzerPin = 11; 

int ledOrder[100]; 
int playerIndex;
int count = 0;


bool waitforbutton = false;
bool checkSequence = false;

int buttonpressed;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  randomSeed(analogRead(A1));

  for (int i = 0; i < 4; i++) {
    pinMode(colors[i].buttonPin, INPUT);
  }

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  for (int f = 0; f < 3; f++)
  {
    for (int i = 0; i < 4; i++) digitalWrite(colors[i].ledPin, HIGH);
    delay(250);
    for (int i = 0; i < 4; i++) digitalWrite(colors[i].ledPin, LOW);
    delay(250);
  }
}

void loop() {
  bool gameOver = false;
  int randomLed = random(0,4);
  ledOrder[count] = randomLed;

  for (int i = 0; i < count + 1; i++)
  {
    digitalWrite(colors[ledOrder[i]].ledPin, HIGH);
    tone(buzzerPin, colors[ledOrder[i]].tone); 
    // Serial.println("ledon");
    delay(500);
    noTone(buzzerPin);
    digitalWrite(colors[ledOrder[i]].ledPin, LOW);
    delay(500);
  }

  //while (checkSequence == false)
  //{
      for (int i = 0; i < count + 1; i++)
      {
        buttonpressed = -1;
        while (buttonpressed == -1)
        {
          buttonpressed = checkButton();
        }
        if (buttonpressed != ledOrder[i])
        {
          gameOver = true;
          // Serial.println("Wrong");
          break; 
        }
        else
        {
          // Serial.println("Correct");
        }
      }
  // }

  if (gameOver)
  {
    Serial.println("Gameover");
    count = 0;
    endSequence();
    delay(500);
  }
  else
  {
    count++;
    delay(1500);
    // Serial.print("Score:");
    // Serial.println(count);
    // Serial.println("next round");
    displayScoreboard();
  // checkSequence = false;
  }
}

void displayScoreboard() 
{
  Serial.println("================================");
  Serial.println("          Simon Game");
  Serial.println("================================");

  Serial.print("SCORE: ");
  Serial.println(count);

  Serial.println("================================");
  Serial.println();
}

void endSequence()
{
  while (true)   // never exits — board is frozen here until you reset/unplug
  {
    for (int i = 0; i < 4; i++) digitalWrite(colors[i].ledPin, HIGH);
    delay(250);
    for (int i = 0; i < 4; i++) digitalWrite(colors[i].ledPin, LOW);
    delay(250);
  }

}

int checkButton()
{

  for (int i = 0; i < 4; i++) {
    if (digitalRead(colors[i].buttonPin) == HIGH) {
      tone(buzzerPin, colors[i].tone);
      delay(250);
      noTone(buzzerPin);
      delay(50);
      return i;   // returns 0-3, matching ledOrder values
    }
  }
  return -1;
}
