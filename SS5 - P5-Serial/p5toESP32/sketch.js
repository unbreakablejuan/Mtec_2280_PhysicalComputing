let port; // variable for instance of the webserial library
let baudRate = 115200; // change the baud rate to match your Arduino code
let outByte = 0;  // 8-bit data to send to microcontroller

let cr = 10; // corner radius for rectangle
let trackTop, trackBottom, trackLeft, trackRight; // sliders for top and bottom of rectangle

let handleY;
let angle;

let handleMin, handleMax;

function setup() {
    createCanvas(1500, 1000);
    trackLeft = width/2 - 50;
    trackRight = width/2 + 50;
    trackTop = height/2 - 150;
    trackBottom = height/2 + 150;
    handleMin = trackTop + 20;
    handleMax = trackBottom - 20;
    handleY = (trackTop + trackBottom) / 2;
    angle = 90;
    textAlign(CENTER, CENTER);
    rectMode(CENTER);
    textSize(32);
    noStroke();

    //mic = new p5.AudioIn(); // create a new instance of the p5.AudioIn class to get mic input

    port = createSerial();                    // make a new serial port instance
    let used = usedSerialPorts();             // list ports used on this page before
    if (used.length > 0)                      // if we have connected to a port before...
        {
    port.open(used[0], baudRate);           // ...reconnect to it automatically
        }
    console.log(trackLeft, trackRight, trackTop, trackBottom);
}

function draw() {
    background(220);
    stroke(0);
    fill(255);
    rect((trackLeft + trackRight) / 2, (trackTop + trackBottom) / 2, (trackRight - trackLeft), (trackBottom - trackTop), cr, cr, cr, cr);
    fill(0);
    rect((trackLeft + trackRight) / 2, handleY, 90, 40, cr/2, cr/2, cr/2, cr/2);
    textSize(32);
    text('Angle:', width/2 - 100, height/2 - 20);
    text(angle, width/2 - 100, height/2 + 20);

    fill(255);
    circle(width/2 + 200, height/2, 100);
    
}


function mousePressed() 
{
  if (!port.opened()) //if the port isn't open yet...
  {
    port.open(baudRate); //...let the user pick a port by pressing any key
  }

  if ((dist(mouseX, mouseY, width/2 + 200, height/2) ) < 50)  {
     console.log("circle clicked");
     angle = 90;                             // reset the angle
     handleY = (trackTop + trackBottom) / 2; // move the handle back to the center
     serialWrite();                          // move the servo to 90 too
    }

updateHandle();
}

function mouseDragged() {
    updateHandle();
}


function updateHandle() {
  if (mouseX > trackLeft && mouseX < trackRight && mouseY > trackTop && mouseY < trackBottom) 
    {
    handleY = mouseY
    angle = round(map(handleY, handleMin, handleMax, 0, 180));
    angle = constrain(angle, 0, 180);
    serialWrite();
    }

    console.log(angle);
}

function serialWrite() {
  if (port.opened()) {
    port.write(angle); // send the angle value to the microcontroller
  }
  console.log("sent", angle);
}