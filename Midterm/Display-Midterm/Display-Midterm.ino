#include <Arduino_GFX_Library.h>
#include "happyface.h"

const int potPin1 = 4;
const int trigPin = 9;
const int echoPin = 10;


float duration, distance;

#define TFT_SCLK 12  // CLK, orange
#define TFT_MOSI 11  // DIN, green
#define TFT_CS   14  // CS, yellow
#define TFT_DC   17  // DC, blue
#define TFT_RST  18  // RST, brown
#define TFT_BL   21  // BL, grey

const int numReads = 16;    // number of readings to average
float reading[numReads];    // last 16 distances, in cm
int count = 0;

Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, GFX_NOT_DEFINED);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, TFT_RST, 0 /* rotation */, true /* IPS panel */);

enum Face { DRY = 0, HAPPY = 1, SUNBURNED = 2, WET = 3 };  
const char *faceNames[] = {"dry", "happy", "sunburned", "wet"};

void showFace(Face f) {
  uint32_t t = millis();
  gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)epd_bitmap_allArray[f], 240, 240);
  Serial.printf("%s face drawn in %lu ms\n", faceNames[f], (unsigned long)(millis() - t));
}

void setup() {
  Serial.begin(115200);
  pinMode(TFT_BL, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  gfx->begin();

  analogReadResolution(2); //knocking resolution back to 8-bit to reduce jumpy pot read
  //showFace((Face)analogRead(potPin1)); //
}

void loop() {
  // --- 1. backlight fade: runs every pass, moves brightness toward targetBright (~1 s) ---
  static int brightness = 0;               // where the backlight is now, 0 to 255
  static int targetBright = 0;             // where it's heading
  static uint32_t lastFadeStep = 0;
  const uint32_t FADE_STEP_MS = 8;         // <tweak length of fade here

  int steps = (millis() - lastFadeStep) / FADE_STEP_MS;   // steps owed since last time
  if (steps > 0) {
    lastFadeStep += steps * FADE_STEP_MS;
    if (brightness < targetBright)      brightness = min(brightness + steps, targetBright);
    else if (brightness > targetBright) brightness = max(brightness - steps, targetBright);
    analogWrite(TFT_BL, brightness * brightness / 255);   // curve so the fade looks even
  }

  // --- 2. everything below runs every 100 ms (replaces delay(100)) ---
  static uint32_t lastSample = 0;
  if (millis() - lastSample < 100) return;  // not time yet; the fade above keeps running
  lastSample = millis();

  // --- 3. distance ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000); // give up after 30 ms (about 5 m)
  if (duration == 0) distance = 999;        // no echo = nobody there
  else distance = (duration * 0.0343) / 2;

  reading[count] = distance;                // store this distance (cm) first
  count++;
  if (count >= numReads) count = 0;

  float sum = 0;                            // then average all 16
  for (int i = 0; i < numReads; i++) sum += reading[i];
  float avgDistance = sum / numReads;       // smoothed distance in cm

  // --- 4. pot picks the face ---
  int faceNum = analogRead(potPin1);        // 0 to 3
  if (faceNum >= epd_bitmap_allArray_LEN) faceNum = epd_bitmap_allArray_LEN - 1;  // stay inside the list

  // screen: fades on when someone is near, stays on 10s
  const uint32_t HOLD_MS = 10000;
  static bool screenOn = false;
  static uint32_t lastSeen = 0;             // millis() when someone was last near
  static int lastFace = -1;

  bool someoneThere = (avgDistance < 75);
  if (someoneThere) lastSeen = millis();    // keep resetting the clock while they're there

  if (someoneThere && !screenOn) {          // someone just walked up
    screenOn = true;
    targetBright = 255;                     // fade on
    lastFace = -1;                          // force a draw on wake
  }

  if (screenOn && faceNum != lastFace) {    // knob moved, even during the 20 s hold
    showFace((Face)faceNum);
    lastFace = faceNum;
  }

  if (screenOn && !someoneThere && millis() - lastSeen >= HOLD_MS) {
    screenOn = false;
    targetBright = 0;                       // fade off
  }

  Serial.printf("Face: %s   Distance: %.1f   Avg: %.1f   Light: %d\n", faceNames[faceNum], distance, avgDistance, brightness);
}