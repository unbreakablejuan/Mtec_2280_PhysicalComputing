let serial;                              // instance of the serialport library
let portName = '/dev/tty.usbmodem101';   // fill in your serial port name here
let options = { baudRate: 9600 };        // match the baud rate in your Arduino code

let inData = 0;     
let sensors = [0, 0, 0, 0]; //array to hold sensor b1, b2, pot1, pot2

let r,g,b;
let diameter = 100;
let grow = 5;

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

    createCanvas(800, 600);
}

function draw() {   
    background(220);
}

// Serial callbacks

function printList(portList) {
  print('List of Available Serial Ports:');
  for (let i = 0; i < portList.length; i++) {
    print(i + ': ' + portList[i]);
  }
}

function serverConnected() {
  print('CONNECTED TO SERVER');
}

function portOpen() {
  print('SERIAL PORT OPEN');
}

function serialEvent() {
  let inString = serial.readStringUntil('\n');
  //if (!inString) return;          // nothing complete yet

  //inString = inString.trim();     // strips the \r from Arduino's println
  if (inString.length === 0) return;

  sensors = inString.split(',');
  //sensors = split(inString, ',');

  for (let i = 0; i < sensors.length; i++) {
    sensors[i] = Number(sensors[i]);
  }
  print(sensors);

 
}

function serialError(err) {
  print('ERROR: ' + err);
}

function portClose() {
  print('*____SERIAL PORT CLOSED');
}
