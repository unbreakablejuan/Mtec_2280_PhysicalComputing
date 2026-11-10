/*
P5.JS SERIAL FIRE

Reads an 8-bit (1-byte) value from the serial port using the p5.serialport library
and uses it to control the intensity of a pixel fire effect.

This code is designed to work with the "Arduino_Serial_Write" example sketch.

NOTES:
- You must run and establish a serial connection with p5.serialcontrol app to use this code:
  https://github.com/p5-serial/p5.serialcontrol/releases/tag/0.1.2
- The p5.serialport library must be included in index.html.
- Make sure the baud rate in options matches the baud rate in your Arduino code.
- Remember to change the portName variable to match your own serial port.
*/

// Serial
let port;                       // variable for Web Serial port instance
let baudRate = 115200;          // NOTE: baud rate must match in Arduino sketch
let sensors = [0, 0, 0, 0];     // array for: [button1, button2, pot1, pot2]

// Fire
const FIRE_WIDTH = 160;
const FIRE_HEIGHT = 160;
const MAX_INTENSITY = 64;                // sets base color to white and increases fire in proportion to fire height limit

let firePixels = new Array(FIRE_WIDTH * FIRE_HEIGHT).fill(0);
let palette = [];

function setup() {
  // P5 SerialPort setup
  serial = new p5.SerialPort();
  serial.on('list', printList);
  serial.on('connected', serverConnected);
  serial.on('open', portOpen);
  serial.on('data', serialEvent);
  serial.on('error', serialError);
  serial.on('close', portClose);

  serial.list();
  serial.open(portName, options);

  createCanvas(windowWidth, windowHeight);
  noStroke();

  // Build palette: black -> red -> orange -> yellow -> white
  let stops = [color(0), color(200, 0, 0), color(255, 120, 0), color(255, 230, 0), color(255)];
  for (let i = 0; i <= MAX_INTENSITY; i++) {
    let t = (i / MAX_INTENSITY) * (stops.length - 1);
    let s = min(floor(t), stops.length - 2);
    palette.push(lerpColor(stops[s], stops[s + 1], t - s));
  }
}

function draw() {
  // Scale incoming serial data (0-255) to fire intensity (0-36)
  let fireBase = floor(map(inData, 0, 255, 0, MAX_INTENSITY, true));

  // Feed the bottom row of the fire
  for (let x = 0; x < FIRE_WIDTH; x++) {
    firePixels[(FIRE_HEIGHT - 1) * FIRE_WIDTH + x] = fireBase;
  }
  print(fireBase)

  updateFire();
  renderFire();
}

function updateFire() {
  for (let x = 0; x < FIRE_WIDTH; x++) {
    for (let y = 1; y < FIRE_HEIGHT; y++) {
      spreadFire(y * FIRE_WIDTH + x);
    }
  }
}

function spreadFire(src) {
  let pixel = firePixels[src];
  if (pixel === 0) {
    firePixels[src - FIRE_WIDTH] = 0;
  } else {
    // Random offset (0 to 3) shifts the flame sideways and decays it
    let rand = floor(random(-32));
    let dst = constrain(src - rand + 1, FIRE_WIDTH, firePixels.length - 1);
    firePixels[dst - FIRE_WIDTH] = max(pixel - (rand & 1), 0);
  }
}

function renderFire() {
  let cellW = width / FIRE_WIDTH;
  let cellH = height / FIRE_HEIGHT;
  for (let y = 0; y < FIRE_HEIGHT; y++) {
    for (let x = 0; x < FIRE_WIDTH; x++) {
      fill(palette[firePixels[y * FIRE_WIDTH + x]]);
      rect(x * cellW, y * cellH, cellW, cellH);
    }
  }
}
