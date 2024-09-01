#include <AccelStepper.h>
#include <MultiStepper.h>

String inputString = "";
bool strComplete = false;

//Video input height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

int posX = 0;
int posY = 0;

//Stepper Motor Control
MultiStepper control;
AccelStepper yAxis(1, 3, 2);
AccelStepper xAxis(1, 5, 4);


int MAX_SPEED = 300;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(0.1);

  //Stepper config
  yAxis.setMaxSpeed(MAX_SPEED);
  xAxis.setMaxSpeed(MAX_SPEED);

}

void loop() {
  yAxis.runSpeed();
  xAxis.runSpeed();
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

      //Process string
      posX = inputString.substring(0, inputString.indexOf(' ')).toInt();
      posY = inputString.substring(inputString.indexOf(' '), inputString.length()).toInt();

      //Y Target LED
      yAxis.setSpeed(calcSpeedY(posY - FRAME_HEIGHT/2));
      xAxis.setSpeed(calcSpeedX(posX - FRAME_WIDTH/2));
  
      clearInString();

    } else {
      inputString += inChar;
    }
  }
}

int calcSpeedY(int error) {
  //Movement tolerance threshold
  if(abs(error) < 10) {
    return 0;
  } else {
    return MAX_SPEED/175 * error;
  }
}

int calcSpeedX(int error) {
  //Movement tolerance threshold
  if(abs(error) < 10) {
    return 0;
  } else {
    return -MAX_SPEED/175 * error;
  }
}