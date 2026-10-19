const FIRE_WIDTH = 150;
const FIRE_HEIGHT = 150;
let fireBase = 36; // Maximum intensity of the fire (0-36)

let firePixels = new Array(FIRE_WIDTH * FIRE_HEIGHT).fill(0);
let palette = [];

function setup() {
  createCanvas(windowWidth, windowHeight);
  noStroke();

  // 37 colors, intensity 0 (black) to 36 (white): black -> red -> orange -> yellow -> white
  let stops = [color(0), color(200, 0, 0), color(255, 120, 0), color(255, 230, 0), color(255)];
  for (let i = 0; i <= 36; i++) {
    let t = (i / 36) * (stops.length - 1);
    let s = min(floor(t), stops.length - 2);
    palette.push(lerpColor(stops[s], stops[s + 1], t - s));
  }

}

function draw() {
     for (let x = 0; x < FIRE_WIDTH; x++) {
    firePixels[(FIRE_HEIGHT - 1) * FIRE_WIDTH + x] = fireBase;
  }

// Update and render the fire
  updateFire();
  renderFire();
}

// function updatebaseFire(){

//     //let firebase = map(mouseY, 0, height, 0, 36); // Adjust fire intensity based on mouse Y position

//      // Bottom row of fire pixels starts at max intensity
//   for (let x = 0; x < FIRE_WIDTH; x++) {
//     firePixels[(FIRE_HEIGHT - 1) * FIRE_WIDTH + x] = fireBase;
//   }

// }

function updateFire() {


    
  for (let x = 0; x < FIRE_WIDTH; x++) {
    for (let y = 1; y < FIRE_HEIGHT; y++) {
      spreadFire(y * FIRE_WIDTH + x);
    }
    //updatebaseFire();
  }
}

function spreadFire(src) {
  let pixel = firePixels[src];
  if (pixel === 0) {
    firePixels[src - FIRE_WIDTH] = 0;
  } else {
    // Random offset (0 to 3) shifts the flame sideways and decays it
    let rand = Math.floor(Math.random() * 4);
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
