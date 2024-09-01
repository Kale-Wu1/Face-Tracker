//Face tracker turret motor control
#include <AccelStepper.h>
#include <MultiStepper.h>

String inputString = "";
bool strComplete = false;

//Video input height
int FRAME_WIDTH;
int FRAME_HEIGHT;

double targetX = 0;
double targetY = 0;

//Stepper Motor Control
MultiStepper control;
AccelStepper yAxis(1, 3, 2);

int MAX_SPEED = 300;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(0.1);

  //Stepper config
  yAxis.setMaxSpeed(MAX_SPEED);

  //Test LEDs
  pinMode(6, OUTPUT); //Left
  pinMode(7, OUTPUT); //Right
  pinMode(8, OUTPUT); //XCorrect
  pinMode(13, OUTPUT); //On Target

}

void loop() {
  yAxis.runSpeed();
}

void clearInString() {
  inputString = "";
  strComplete = false;
}

//Serial input listener
void serialEvent() {
  while(Serial.available()) {
    char inChar = (char)Serial.read();

    if (inChar == '\n') {
      strComplete = true;

      //Serial.println("Received String: " + inputString);

      //Process string
      targetX = inputString.substring(0, inputString.indexOf(' ')).toDouble();
      targetY = inputString.substring(inputString.indexOf(' '), inputString.length()).toDouble();

      //Serial.print("Converted to values: ");
      //Serial.print(targetX);
      //Serial.print(", ");
      //Serial.println(targetY);

      
      //X Target LED
      /*
      if(targetX > 0.6) {
        digitalWrite(6, LOW);
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
      } else if(targetX < 0.4) {
        digitalWrite(6, HIGH);
        digitalWrite(7, LOW);
        digitalWrite(8, LOW);
      } else {
        digitalWrite(6, LOW);
        digitalWrite(7, LOW);
        digitalWrite(8, HIGH);
      }
      */

      //Y Target LED
      if(targetY > 0.6) {
        yAxis.setSpeed(MAX_SPEED);
      } else if(targetY < 0.4) {
        yAxis.setSpeed(-MAX_SPEED);
      } else {
        yAxis.setSpeed(0);
      }
      

      //On target LED
      //if(yAxis && digitalRead(5) == HIGH) {
      //  digitalWrite(13, HIGH);
      //} else {
      //  digitalWrite(13, LOW);
      //}

      clearInString();

    } else {
      inputString += inChar;
    }
  }
}

