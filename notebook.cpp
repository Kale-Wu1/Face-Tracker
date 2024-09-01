//Concept for auto frame size detection in setup()
//Get frame dimensions
noInterrupts();
while(Serial.available()) {
    char inChar = (char)Serial.read();

    if (inChar == '\n') {
      strComplete = true;

      FRAME_WIDTH = inputString.substring(0, inputString.indexOf(' ')).toDouble();
      FRAME_HEIGHT = inputString.substring(inputString.indexOf(' '), inputString.length()).toDouble();
      
      clearInString();

    } else {
      inputString += inChar;
    }
}
interrupts();




//P control (untested)
//Face tracker turret motor control
#include <AccelStepper.h>
#include <MultiStepper.h>

String inputString = "";
bool strComplete = false;

//Video input height
int FRAME_WIDTH;
int FRAME_HEIGHT;

int posX = 0;
int posY = 0;

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
      posX = inputString.substring(0, inputString.indexOf(' ')).toInt();
      posY = inputString.substring(inputString.indexOf(' '), inputString.length()).toInt();

      //Serial.print("Converted to values: ");
      //Serial.print(posX);
      //Serial.print(", ");
      //Serial.println(posY);

      
      //X Target LED
      /*
      if(posX > 0.6) {
        digitalWrite(6, LOW);
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
      } else if(posX < 0.4) {
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
      yAxis.setSpeed(calcSpeed(posY - FRAME_HEIGHT/2));
      

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

int calcSpeed(int error) {
  //Movement tolerance threshold
  if(abs(error) < 50) {
    return 0;
  } else {
    return MAX_SPEED * error/FRAME_HEIGHT;
  }

}

