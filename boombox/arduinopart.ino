#include <Wire.h>

// 0x4B is the default i2c address
#define MAX9744_I2CADDR 0x4B

int8_t thevol = 31;

void setup() {
  Serial.begin(9600);
  delay(3000);

  Serial.println("Step 1: Arduino started");

  Wire.begin();
  Serial.println("Step 2: I2C started");

  if (!setvolume(thevol)) {
    Serial.println("Step 3: Amplifier not found!");
    while (1);
  }

  Serial.println("Step 4: Amplifier found!");
}


boolean setvolume(int8_t v) {
  // cant be higher than 63 or lower than 0
  if (v > 63) v = 63;
  if (v < 0) v = 0;
  
  Serial.print("Setting volume to ");
  Serial.println(v);
  Wire.beginTransmission(MAX9744_I2CADDR);
  Wire.write(v);
  if (Wire.endTransmission() == 0) 
    return true;
  else
    return false;
}

// Read in + and - characters to set the volume.
void loop() {
  if (!Serial.available()) return;

  // read one whole message, up to the newline
  String message = Serial.readStringUntil('\n');
  message.trim();

  // ignore empty messages
  if (message.length() == 0) return;

  if (message == "+") {
    thevol++;
  }
  else if (message == "-") {
    thevol--;
  }
  else {
    thevol = message.toInt();
  }

  if (thevol > 63) thevol = 63;
  if (thevol < 0) thevol = 0;

  setvolume(thevol);
}