//Face tracker turret motor control

String inputString = "";
bool strComplete = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(0.1);
}

void loop() {}

void clearInString() {
  inputString = "";
  strComplete = false;
}

//Serial input listener
void serialEvent() {
  while(Serial.available()) {
    char inChar = (char)Serial.read();

    if (inChar == '\n') {
      Serial.println("Received input string: " + inputString);
      clearInString();
    } else {
      inputString += inChar;
    }
  }
}
