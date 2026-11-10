let port;               // variable for serial port instance
let baudRate = 115200;  // must match Serial.begin on the board

let firstContact = false;    // true once a line has arrived from the board
let sensors = [0, 0, 0, 0];  // incoming sensor data
let pot1 = 0;
let pot2 = 0;
let button1 = 0;
let button2 = 0;

let mode = 'M';         // 'M' = menu, 'F' = fire, matches the board's boot mode
let modeSent = false;   // true once the board has been told the mode after the port opened

let angle = 0;
let power = 0;
let fixedLength = 300;  // length of the aim line, only shows the angle

let targetX, targetY;
let targetSpawnTime = 0;
let targetActive = false;
const targetLifetime = 15000;  // 15 seconds in milliseconds

let shotActive = false;
let shotX, shotY, shotVX, shotVY;
let button1WasDown = false;
let button2WasDown = false;
let comboWasDown = true;   // starts true so holding both buttons at boot does nothing
const gravity = 0.5;

let ammo = 3;
let score = 0;
let lastSent = '';

function setup()
{
  createCanvas(1920, 1080);
  angleMode(DEGREES);
  textAlign(CENTER, CENTER);
  textSize(24);

  port = createSerial(); // make a new instance of the serial port

  // if we've connected to a serial port before, reopen it
  let used = usedSerialPorts();
  if (used.length > 0)
  {
    port.open(used[0], baudRate);
  }
}

function draw()
{
  //____SERIAL_Tx_START____
  if (port.opened() && !modeSent)
  {
    port.write(mode);    // sync the board with p5 right after connecting
    modeSent = true;
  }

  if (port.opened() && mode === 'F')
  {
    let out = String(ammo);
    if (out !== lastSent)
    {
      port.write(out);   // sends the character '0' to '3'
      lastSent = out;
    }
  }
  //____SERIAL_Tx_END____

  //____SERIAL_Rx_START____
  let inString = port.readUntil('\n'); // read incoming serial data until newline
  if (inString.length > 0)
  {
    firstContact = true;
    sensors = inString.split(','); // split at commas, store in array

    if (sensors.length >= 4)
    {
      button1 = map(Number(sensors[0]), 0, 1, 0, 255);
      button2 = map(Number(sensors[1]), 0, 1, 0, 255);
      pot1 = floor(map(Number(sensors[2]), 0, 1023, 0, 255));
      pot2 = floor(map(Number(sensors[3]), 0, 1023, 0, 255));
    }
  }
  //____SERIAL_Rx_END____

  angle = floor(map(Number(sensors[2]), 0, 1023, 90, 0));
  power = floor(map(Number(sensors[3]), 0, 1023, 0, 150));

  // button states, and the moment both become pressed together
  let button1Down = button1 > 0;
  let button2Down = button2 > 0;
  let comboDown = button1Down && button2Down;
  let comboPressed = comboDown && !comboWasDown;
  comboWasDown = comboDown;

  if (!firstContact)  // if we have not yet received any data...
  {
    background(0, 0, 127);
    fill(255);
    noStroke();
    text("Waiting for First Contact", width/2, height/4);
    text("Click anywhere to connect", width/2, height/3);
  }
  else if (mode === 'M')
  {
    background(0);
    fill(255);
    noStroke();
    text("Unity Games Control Test (press both buttons to start)", width/2, height/4);
    text("Left Button Fires || Righ Button Reloads", width/2, height/4 + 50);

    // both buttons pressed: start the game
    if (comboPressed)
    {
      setMode('F');
    }
  }
  else  // mode === 'F'
  {
    background(0);

    // both buttons pressed: back to the menu
    if (comboPressed)
    {
      setMode('M');
      return;   // stop this frame so no shot or reload runs
    }

    let startX = 0;
    let startY = height;
    let endX = startX + fixedLength * cos(angle);
    let endY = startY - fixedLength * sin(angle);

    // aim line
    stroke('magenta');
    strokeWeight(5);
    line(startX, startY, endX, endY);

    // readouts
    fill('red');
    noStroke();
    text(`ANGLE = ${angle}`, width/2, 25);
    text(`AMMO = ${ammo}`, width/2, 55);
    text(`SCORE = ${score}`, width/2, 85);
    rect(320, height - (power * 2), 50, (power * 2));

    // target: respawns every 15 seconds, or on a hit
    if (!targetActive || millis() - targetSpawnTime >= targetLifetime)
    {
      targetSpawn();
    }
    fill('orange');
    noStroke();
    circle(targetX, targetY, 50);

    // button 1: shoot
    if (button1Down && !button1WasDown && !shotActive && ammo > 0)
    {
      ammo -= 1;
      shotX = 0;
      shotY = height;
      shotVX = power * 0.3 * cos(angle);
      shotVY = -power * 0.3 * sin(angle);
      shotActive = true;
    }
    button1WasDown = button1Down;

    // shot in flight
    if (shotActive)
    {
      shotVY += gravity;
      shotX += shotVX;
      shotY += shotVY;

      fill('yellow');
      noStroke();
      circle(shotX, shotY, 20);

      // hit check: 25 target radius + 10 shot radius
      if (dist(shotX, shotY, targetX, targetY) < 35)
      {
        score += 1;
        targetSpawn();
        shotActive = false;
      }
      else if (shotX > width || shotY > height)
      {
        shotActive = false;
      }
    }

    // button 2: reload when empty
    if (button2Down && !button2WasDown && ammo === 0)
    {
      ammo = 3;
    }
    button2WasDown = button2Down;
  }
}

function mousePressed()
{
  if (!port.opened())
  {
    port.open(baudRate); // browser pop-up lets you choose the board
  }
}

function setMode(newMode)
{
  mode = newMode;
  lastSent = '';                 // forces the digit to be sent again
  button1WasDown = true;         // stops a held button counting as a new press
  button2WasDown = true;
  if (newMode === 'F')
  {
    ammo = 3;
    score = 0;
    shotActive = false;
  }
  if (port.opened())
  {
    port.write(mode);
  }
}

function keyPressed() // detects key pressed
{
  if (key === 'f') setMode('F');
  if (key === 'm') setMode('M');
}

function targetSpawn()
{
  targetX = random(width);
  targetY = random(height);
  targetSpawnTime = millis();
  targetActive = true;
}