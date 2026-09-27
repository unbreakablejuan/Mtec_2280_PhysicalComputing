/*
DICE 

NEED TO MAKE RANDOMIZING LOGIC
ANIMATION 
BUTTON STATES - press 1 button to start animiation press 2nd button to "roll" - triggers random display of num function for delay. 
*/

const int buttonPin = 4;
const int LED_PIN_1 = 1;
const int LED_PIN_2 = 2;
const int LED_PIN_3 = 42;
const int LED_PIN_4 = 41;
const int LED_PIN_5 = 40;
const int LED_PIN_6 = 39;
const int LED_PIN_7 = 38;
const int LED_PIN_8 = 37;
const int LED_PIN_9 = 36;

const int DEL = 75;

bool buttonState = 0;     //current button state
bool lastButtonState = 0; //previous button state
bool toggle = 0;          //toogle bool for rising edge
bool fallToggle = 0;      //toggle bool for falling edge

void setup() 
{
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  pinMode(LED_PIN_4, OUTPUT);
  pinMode(LED_PIN_5, OUTPUT);
  pinMode(LED_PIN_6, OUTPUT);
  pinMode(LED_PIN_7, OUTPUT);
  pinMode(LED_PIN_8, OUTPUT);
  pinMode(LED_PIN_9, OUTPUT);

  Serial.begin(115200);
}







// switch (random(0,6))
//   {
//     case 0: numOne(); delay(DEL);
//     case 1: numTwo(); delay(DEL);
//     case 2: numThree(); delay(DEL);
//     case 3: numFour(); delay(DEL);
//     case 4: numFive(); delay(DEL);
//     case 5: numSix(); delay(DEL);

//   }


void numOne()
  {
    digitalWrite(LED_PIN_1, LOW);
    digitalWrite(LED_PIN_2, LOW);
    digitalWrite(LED_PIN_3, LOW);
    digitalWrite(LED_PIN_4, LOW);
    digitalWrite(LED_PIN_5, HIGH);
    digitalWrite(LED_PIN_6, LOW);
    digitalWrite(LED_PIN_7, LOW);
    digitalWrite(LED_PIN_8, LOW);
    digitalWrite(LED_PIN_9, LOW);
  }



void numTwo()
  {
    digitalWrite(LED_PIN_1, HIGH);
    digitalWrite(LED_PIN_2, LOW);
    digitalWrite(LED_PIN_3, LOW);
    digitalWrite(LED_PIN_4, LOW);
    digitalWrite(LED_PIN_5, LOW);
    digitalWrite(LED_PIN_6, LOW);
    digitalWrite(LED_PIN_7, LOW);
    digitalWrite(LED_PIN_8, LOW);
    digitalWrite(LED_PIN_9, HIGH);
  }

void numThree()
  {
    digitalWrite(LED_PIN_1, HIGH);
    digitalWrite(LED_PIN_2, LOW);
    digitalWrite(LED_PIN_3, LOW);
    digitalWrite(LED_PIN_4, LOW);
    digitalWrite(LED_PIN_5, HIGH);
    digitalWrite(LED_PIN_6, LOW);
    digitalWrite(LED_PIN_7, LOW);
    digitalWrite(LED_PIN_8, LOW);
    digitalWrite(LED_PIN_9, HIGH);
  }

void numFour()
  {
    digitalWrite(LED_PIN_1, HIGH);
    digitalWrite(LED_PIN_2, LOW);
    digitalWrite(LED_PIN_3, HIGH);
    digitalWrite(LED_PIN_4, LOW);
    digitalWrite(LED_PIN_5, LOW);
    digitalWrite(LED_PIN_6, LOW);
    digitalWrite(LED_PIN_7, HIGH);
    digitalWrite(LED_PIN_8, LOW);
    digitalWrite(LED_PIN_9, HIGH);
  }

void numFive()
  {
    digitalWrite(LED_PIN_1, HIGH);
    digitalWrite(LED_PIN_2, LOW);
    digitalWrite(LED_PIN_3, HIGH);
    digitalWrite(LED_PIN_4, LOW);
    digitalWrite(LED_PIN_5, HIGH);
    digitalWrite(LED_PIN_6, LOW);
    digitalWrite(LED_PIN_7, HIGH);
    digitalWrite(LED_PIN_8, LOW);
    digitalWrite(LED_PIN_9, HIGH);
  }

void numSix()
  {
    digitalWrite(LED_PIN_1, HIGH);
    digitalWrite(LED_PIN_2, LOW);
    digitalWrite(LED_PIN_3, HIGH);
    digitalWrite(LED_PIN_4, HIGH);
    digitalWrite(LED_PIN_5, LOW);
    digitalWrite(LED_PIN_6, HIGH);
    digitalWrite(LED_PIN_7, HIGH);
    digitalWrite(LED_PIN_8, LOW);
    digitalWrite(LED_PIN_9, HIGH);
  }



  void (*randomFunctions[])() =
  {
  numOne, 
  numTwo,
  numThree,
  numFour,
  numFive,
  numSix, 
  };

  void totalFunction(){
    
  }

  void loop() 
{



  buttonState = !digitalRead(buttonPin);

  //RISING EDGE DETECTION
  //Detects the PRESS of a button
  if (buttonState && !lastButtonState) //if button went from LOW to HIGH
  {
    toggle = !toggle; //flip toggle bool using NOT logic
  }

  //FALLING EDGE DETECTION
  //Detects the RELEASE of a button
  // if (!buttonState && lastButtonState) //if button went from HIGH to LOW
  // {
  //   fallToggle = !fallToggle; //flip fallToggle bool using NOT logic
  // }

  // lastButtonState = buttonState;  //store current button state for logic comparison at beginning of next loop

  //digitalWrite(ledPin, toggle);   //turn LED on & off with toggle

  //Serial.printf("Button = %i _ Toggle = %i _ Falling = %i \n", buttonState, toggle, fallToggle);

  int randomIndex = random(totalFunctions);

  randomFunctions[randomIndex]();
  
  delay(DEL);

}
