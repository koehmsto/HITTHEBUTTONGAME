#include <Adafruit_CircuitPlayground.h>

int playerScore = 0;
int gameSpeed = 500;
int constantPixel;
bool isGameRunning = true;
volatile bool buttonInterrupt = false;
volatile bool button2Interrupt = false;
volatile bool switchFlag = true;
int missAmount = 0;
bool forward = 1;

void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(5), buttonISR, RISING);
  attachInterrupt(digitalPinToInterrupt(4), button2ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(7), switchISR, CHANGE);

  randomSeed(analogRead(A6));  // seeds the random number with the signal from A6
  constantPixel = random(0, 10);
  CircuitPlayground.setPixelColor(constantPixel, 0x00FF00);  // Green color for constant pixel
  Serial.println("Loop Start setup");
}

void loop() {

  while (isGameRunning) {  // game is in playing state
      
    for (int i = 0; i < 10; i++) {  // makes dot go in circle
      
      int p;

      if (forward)
        p = i;
      else
        p = 9 - i;

      CircuitPlayground.setPixelColor(p, 0xFF0000);  // Red color for the moving pixel

      if (constantPixel == p) {
        CircuitPlayground.setPixelColor(constantPixel, 0x00FF00);  // Makes the constant pixel stay green while the moving red goes over it
      }

      int previousPixel;  // Turn off the previous pixel
      if (forward)
        previousPixel = p - 1;
      else
        previousPixel = p + 1;

      if (previousPixel != constantPixel) {
        CircuitPlayground.setPixelColor(previousPixel, 0x000000);  // Reset previous pixel
      }

      delay(gameSpeed);

      // Check if the button is pressed
      if (buttonInterrupt == true) {

        if (p == constantPixel) {
          playerScore++;

          Serial.println("Hit!");
          //Serial.println(playerScore);
          CircuitPlayground.playTone(440, 200);  // Play a sound to indicate a hit
          //Set a new constant pixel
          CircuitPlayground.setPixelColor(constantPixel, 0x000000);
          constantPixel = random(0, 10);
          CircuitPlayground.setPixelColor(constantPixel, 0x00FF00);  // Green color for new constant pixel
          gameSpeed = gameSpeed - 50;                                //increases game speed for each time the green dot is hit
          if (gameSpeed < 50) {        // end game if speed gets to 0
            CircuitPlayground.clearPixels();
            CircuitPlayground.playTone(500, 100); //win tone
            CircuitPlayground.playTone(700, 200);
            CircuitPlayground.playTone(500, 100);
            isGameRunning = false;
            CircuitPlayground.clearPixels();
          }
          buttonInterrupt = false;
          forward = !forward;
          break;
        } else {
          missAmount++;  // increment miss amount
          CircuitPlayground.playTone(200, 200);
          Serial.println("Miss!");
          buttonInterrupt = false;
        }
      }
      /*if(!switchFlag) {
        CircuitPlayground.clearPixels();
        gameSpeed = 500;
         CircuitPlayground.setPixelColor(constantPixel, 0x000000);
        constantPixel = random(0, 10);
        CircuitPlayground.setPixelColor(constantPixel, 0x00FF00);  // Green color for constant pixel
        break;

      } */
      if (missAmount > 2) {  // if more than 2 misses, the game will end and execute missedAmount
        isGameRunning = false;
        missedAmount();
        gameSpeed = 500;
        break;
      }
    }
    if (isGameRunning == true) {
      if (forward && 9 != constantPixel) {
        CircuitPlayground.setPixelColor(9, 0x000000);
      } else if (0 != constantPixel) {
        CircuitPlayground.setPixelColor(0, 0x000000);
      }
    }
  }
  if (button2Interrupt == true) {

    missAmount = 0;
    gameSpeed = 500;
    Serial.println("Interrupt");
    CircuitPlayground.setPixelColor(constantPixel, 0x000000);
    constantPixel = random(0, 10);
    CircuitPlayground.setPixelColor(constantPixel, 0x00FF00);  // Green color for constant pixel
    isGameRunning = true;
    button2Interrupt = false;
  }
}

void missedAmount() {
  CircuitPlayground.playTone(200, 100);
  CircuitPlayground.playTone(200, 100);
  CircuitPlayground.playTone(200, 100);
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 0xFF0000);
    delay(100);
    CircuitPlayground.clearPixels();
  }
  for (int i = 10; i > -1; i--) {
    CircuitPlayground.setPixelColor(i, 0xFF0000);
    delay(100);
    CircuitPlayground.clearPixels();
  }
}

void buttonISR() {
  buttonInterrupt = true;
}

void button2ISR() {
  Serial.println("Button2ISR");
  button2Interrupt = true;
}

void switchISR() {
    switchFlag = !switchFlag; // Toggle the switch state
}