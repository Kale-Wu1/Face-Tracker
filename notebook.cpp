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