const int LED_PIN_1 = 4;
const int LED_PIN_2 = 5;
const int LED_PIN_3 = 6;
const int LED_PIN_4 = 7;
const int BUTTON_PIN = 16;

const int DEL = 500;
const int DELRAND = random(1500,4000);

int roundCount = 0;

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  pinMode(LED_PIN_4, OUTPUT);

  Serial.println("Get Ready. Press the button when all lights go off to test your reaction time.");
}

void loop() {
  runLightSequence();

  unsigned long startTime = millis();

  while (digitalRead(BUTTON_PIN) == HIGH) {
  }

  unsigned long reactionTime = (millis() - startTime);
  roundCount++;


  if (reactionTime < 150) {
  Serial.print("We got an F1 Driver over here. ");
  Serial.print("Round ");
  Serial.print(roundCount);
  Serial.print(": ");
  Serial.print(reactionTime);
  Serial.println(" ms");
} else if (reactionTime > 150 && reactionTime < 300) {
  Serial.print("Good. ");
  Serial.print("Round ");
  Serial.print(roundCount);
  Serial.print(": ");
  Serial.print(reactionTime);
  Serial.println(" ms");
} else if (reactionTime > 300 && reactionTime < 500) {
  Serial.print("Nice try. ");
  Serial.print("Round ");
  Serial.print(roundCount);
  Serial.print(": ");
  Serial.print(reactionTime);
  Serial.println(" ms");
} else if (reactionTime > 500) {
  Serial.print("At least you still have a pulse. ");
  Serial.print("Round ");
  Serial.print(roundCount);
  Serial.print(": ");
  Serial.print(reactionTime);
  Serial.println(" ms");
}
  


  delay(2000);
  }


// Counts down like F1 starting grid lights, a random delap is set for lights out to prevent pattern behaviour. 
void runLightSequence() {
  digitalWrite(LED_PIN_1, LOW);
  digitalWrite(LED_PIN_2, LOW);
  digitalWrite(LED_PIN_3, LOW);
  digitalWrite(LED_PIN_4, LOW);
  delay(DEL);

  digitalWrite(LED_PIN_4, HIGH);
  delay(DEL);

  digitalWrite(LED_PIN_3, HIGH);
  delay(DEL);

  digitalWrite(LED_PIN_2, HIGH);
  delay(DEL);

  digitalWrite(LED_PIN_1, HIGH);
  delay(DELRAND);

  // All off = Go Time
  digitalWrite(LED_PIN_1, LOW);
  digitalWrite(LED_PIN_2, LOW);
  digitalWrite(LED_PIN_3, LOW);
  digitalWrite(LED_PIN_4, LOW);


  }