const int adcPin_1 = 1;
const int adcPin_2 = 2;
const int buttonPin_1 = 41;
const int buttonPin_2 = 42;
const int ledPin1 = 4;
const int ledPin2 = 5;
const int ledPin3 = 6;
const int sendInterval = 30;  // ms between data lines

bool buttonState_1 = 0;
bool buttonState_2 = 0;

int adcRead_1 = 0;
int adcRead_2 = 0;
int inByte = 0;

char currentMode = 'M';          // 'F' = FireMode, 'M' = MenuMode, boots in menu
unsigned long lastSendTime = 0;  // millis() at the last data line

void setup() 
{
  pinMode(buttonPin_1, INPUT_PULLUP);
  pinMode(buttonPin_2, INPUT_PULLUP);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  digitalWrite(ledPin1, 0);
  digitalWrite(ledPin2, 0);
  digitalWrite(ledPin3, 0);  // turn LED OFF
  analogReadResolution(10);  // set ADC range of 0 to 1023
  Serial.begin(115200);      // open serial port at 115200bps
}

void loop() 
{
  if(Serial.available()) //if serial buffer is greater than zero...
  {
      inByte = Serial.read();
      if(inByte == 'F')
    {
      currentMode = 'F';
    }
      else if (inByte == 'M')
    {
      currentMode = 'M';
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, LOW);
    }
      else if (inByte == '1' || inByte == '2' || inByte == '3' || inByte == '0')
    {
      if(currentMode == 'F')
      {
        recieveNumber();
      }
    }
  }

  if(millis() - lastSendTime >= sendInterval)
  {
      lastSendTime = millis();
      adcRead_1 = analogRead(adcPin_1); //read pot 1
      adcRead_2 = analogRead(adcPin_2); //read pot 2
      buttonState_1 = !digitalRead(buttonPin_1);  //read button 1
      buttonState_2 = !digitalRead(buttonPin_2);  //read button 2
      Serial.print(buttonState_1);  //send 1st value
      Serial.print(',');            //send comma ASCII char to separate values
      Serial.print(buttonState_2);  //send 2nd value
      Serial.print(',');            //send comma ASCII char to separate values
      Serial.print(adcRead_1);      //send 3rd value
      Serial.print(',');            //send comma ASCII char to separate values
      Serial.print(adcRead_2);      //send 4th value
      Serial.print('\n');           //send newline ASCII char to frame end of message
  }
}

void recieveNumber()
{
      if(inByte == '1')
        {
          digitalWrite(ledPin1, HIGH);
          digitalWrite(ledPin2, LOW);
          digitalWrite(ledPin3, LOW);
        }
      else if(inByte == '2')
        {
          digitalWrite(ledPin1, HIGH);
          digitalWrite(ledPin2, HIGH);
          digitalWrite(ledPin3, LOW);
        }
      else if(inByte == '3')
        {
          digitalWrite(ledPin1, HIGH);
          digitalWrite(ledPin2, HIGH);
          digitalWrite(ledPin3, HIGH);
        }
      else if(inByte == '0')
        {
          digitalWrite(ledPin1, LOW);
          digitalWrite(ledPin2, LOW);
          digitalWrite(ledPin3, LOW);
        }
}