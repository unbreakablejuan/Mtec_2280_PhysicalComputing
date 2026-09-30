/*
DICE 
Press button: start rolling animation
Press again:  slow down and land on a random face
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

const int DEL = 75;       // ms per frame while rolling
const int DEBOUNCE = 30;  // ms the button must be stable

bool buttonState = 0;  // debounced button state
bool lastReading = 0;  // raw reading from last loop
bool toggle = 0;       // 1 = rolling, 0 = stopped
unsigned long lastChangeTime = 0;
unsigned long lastFrameTime = 0;
int currentFace = 0;
const int totalFunctions = 6;

void setup() {
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

  Serial.begin(9600);
}

void numOne() {
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

void numTwo() {
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

void numThree() {
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

void numFour() {
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

void numFive() {
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

void numSix() {
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

void (*randomFunctions[])() = {
  numOne,
  numTwo,
  numThree,
  numFour,
  numFive,
  numSix,
};



// Pick a face different from the current one
void showNewFace() {
  int next;
  do {
    next = random(totalFunctions);
  } while (next == currentFace);
  currentFace = next;
  randomFunctions[currentFace]();
}

// Tumble: frames get slower until it stops on the result
void settle() {
  for (int d = DEL; d < 400; d += 50) {
    showNewFace();
    delay(d);
  }
  Serial.printf("Result: %i \n", currentFace + 1);
}

void loop() {
  bool reading = !digitalRead(buttonPin);

  //track when the raw reading last changed
  if (reading != lastReading) {
    lastChangeTime = millis();
    lastReading = reading;
  }

  //RISING EDGE DETECTION (debounced)d
  if (millis() - lastChangeTime > DEBOUNCE && reading != buttonState) {
    buttonState = reading;

    if (buttonState)  //button was PRESSED
    {
      toggle = !toggle;  //flip toggle bool using NOT logic

      if (toggle) {
        Serial.println("Rolling...");
      } else {
        settle();  //second press: land on a result
      }
    }
  }

  //ANIMATION
  if (toggle && millis() - lastFrameTime >= DEL) {
    lastFrameTime = millis();
    showNewFace();
  }
}