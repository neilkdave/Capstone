const int adcPrescaler = 16;
const unsigned char PS_16 = (1 << ADPS2);
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

const long baudRate = 115200;
const int motorControlPin = 38; // pumpEnable on breadboard // TODO: Remove after assembly 
const int pumpEnable = 53; // pumpEnable on schematic
const int maxNumPouches = 15;
const int numPouches = 13;
const int pouchPinOffset = 22;
const int minPouchDelay = 5400; // TODO: Verify this is smallest imperceptible delay 
const int pressureReadPin = 14; // Breadboard pin 
unsigned short currentTankPressure;
unsigned long currentTime;
unsigned long times[5];

int current[numPouches] = {0};
int target[numPouches] = {0};
bool valve[numPouches]; // true = inflate, false deflate

const int maxOffset = 10;
const int minOffset = -10;
int offset;
const int maxTankPressure = 250;
const int minTankPressure = 200;

const int sensorOffset = 30;
const int sensorScalar = 2;
bool busy[numPouches];
int done[numPouches]; // TODO: unsigned long?
bool inflating = false;

int calculatedDeflate;
int calculatedInflate;

// Serial Variables
const int maxMessageLength = maxNumPouches + 3; // 3 for version, type and newline
char message[maxMessageLength];
char protocolVersion;
char messageType;
char messageValue;
char* messageBody = &message[2]; // 2 for version and type
const int messageBodyLength = maxNumPouches + 1; // 1 for newline

// TODO:  Populate pin arrays
const int inflatePins[] = {
  1, // Pouch #0
  2, // Pouch #1
  3, // Pouch #2
  4, // Pouch #3
  5, // Pouch #4
  6, // Pouch #5
  7, // Pouch #6
  8, // Pouch #7
  9, // Pouch #8
  10, // Pouch #9
  11, // Pouch #10
  12, // Pouch #11
  12, // Pouch #12
  14, // Pouch #13
  15, // Pouch #14
};

const int deflatePins[] = {
  1, // Pouch #0
  2, // Pouch #1
  3, // Pouch #2
  4, // Pouch #3
  5, // Pouch #4
  6, // Pouch #5
  7, // Pouch #6
  8, // Pouch #7
  9, // Pouch #8
  10, // Pouch #9
  11, // Pouch #10
  12, // Pouch #11
  12, // Pouch #12
  14, // Pouch #13
  15, // Pouch #14
};

const int pressureSensorPins[] = {
  16, // Pouch #0
  17, // Pouch #1
  18, // Pouch #2
  19, // Pouch #3
  20, // Pouch #4
  21, // Pouch #5
  22, // Pouch #6
  23, // Pouch #7
  24, // Pouch #8
  25, // Pouch #9
  26, // Pouch #10
  27, // Pouch #11
  28, // Pouch #12
  29, // Pouch #13
  30, // Pouch #14
};

// TODO: Time reads, writes, and serial


void setup() {
  ADCSRA &= ~PS_128;  // Remove bits set by Arduino library

  if (adcPrescaler == 64) {
    ADCSRA |= PS_64; // 64 prescaler
  }
  else if (adcPrescaler == 32) {
    ADCSRA |= PS_32; // 32 prescaler  
  }
  else if (adcPrescaler == 16) {
    ADCSRA |= PS_16; // 16 prescaler
  }
  else {
    ADCSRA |= PS_128; // 128 prescaler
  }
  
  Serial.begin(baudRate);
  for (int pouchCounter = 0; pouchCounter < numPouches; pouchCounter++) {
    pinMode(inflatePins[pouchCounter], OUTPUT);
    pinMode(deflatePins[pouchCounter], OUTPUT);
  }
  pinMode(motorControlPin, OUTPUT);
  Serial.println("Ready");
}

// void closeValves(int nextBlockingTime) {
//   safeToContinue = false;
//   do {
//     currentTime = micros();
//     nextDelay = done[0];
//     for (int pouchCounter = 1; pouchCounter < numPouches; pouchCounter++) {
//       nextDelay = min(nextDelay, done[pouchCounter]);
//     }
//     if (nextDelay - currentTime < nextBlockingTime) { // if we need to open before next blocking time
//       // delay by microseconds // if neg, dont delay
//       // close valves
//     }
//     else {
//       safeToContinue = true;
//     }
//   } while(!safeToContinue);
// }

void loop() {
  times[0] = micros();

  // Close valves that are past their expiration
  // Best:   6us
  // Worst: 140us
  currentTime = micros();
  for (int pouchCounter = 0; pouchCounter < numPouches; pouchCounter++) {
    if (busy[pouchCounter] && (done[pouchCounter] < currentTime)) {
      busy[pouchCounter] = false;
      if (valve[pouchCounter]) {
        digitalWrite(inflatePins[pouchCounter], LOW);
      }
      else {
        digitalWrite(deflatePins[pouchCounter], LOW);
      }
    }
  }
  times[1] = micros();
  
  // Main inflation loop
  // 128 ADC Scaler // Default
  // Best:     0us
  // One:    130us
  // Worst: 1640us
  
  // 16 ADC Scaler // Fastest
  // Best:     0us
  // One:     32us
  // Worst:  480us
  
  // closeValves(480);
  for (int pouchCounter = 0; pouchCounter < numPouches; pouchCounter++) { // Inflate and Deflates Pouches
    if (!busy[pouchCounter]) {
      current[pouchCounter] = analogRead(pressureSensorPins[pouchCounter]); // Read pressure measurements from pouches that are not busy 
      offset = target[pouchCounter] - current[pouchCounter]; // Either adjust currentReads to fit scale or adjust target to fit Scale 
      if (offset > maxOffset) {
        calculatedDeflate = 8000; // TODO: Define more concrete calculations 
        busy[pouchCounter] = true;
        valve[pouchCounter] = false;
        done[pouchCounter] = micros() + calculatedDeflate;
        digitalWrite(deflatePins[pouchCounter], HIGH);
      }
      else if (offset < minOffset) {
        calculatedInflate = 8000; // TODO: Define more concrete calculations 
        busy[pouchCounter] = true;
        valve[pouchCounter] = true;
        done[pouchCounter] = micros() + calculatedInflate;
        digitalWrite(inflatePins[pouchCounter], HIGH);
      }
      else if (offset < 0) {
        // Small Inflate
        busy[pouchCounter] = true;
        valve[pouchCounter] = true;
        done[pouchCounter] = micros() + minPouchDelay;
        digitalWrite(inflatePins[pouchCounter], HIGH);
      } // Assuming if its > 0 slow leak, do nothing
    }
  }
  times[2] = micros();

  // Get tank pressure
  // closeValves(100); // TODO: double check time
  currentTankPressure = analogRead(pressureReadPin);
  if (inflating && (currentTankPressure > maxTankPressure)) {
    inflating = false;
    digitalWrite(pumpEnable, LOW);
  }
  else if (!inflating && (currentTankPressure < minTankPressure)) {
    inflating = true;
    digitalWrite(pumpEnable, HIGH);
  }

  times[3] = micros();
  // closeValves(550);
  bool found = false;
  while (Serial.available()) { // Parse serial until no serial left to read
    found = true;
//    Serial.readBytesUntil('\n', message, maxMessageLength);
    Serial.readBytesUntil(0b11111111, message, maxMessageLength);
    protocolVersion = (message[0] & 0b11000000) >> 6;
    if (protocolVersion == 1) {
      messageType = (message[0] & 0b00110000) >> 4;
      if (messageType == 1) {
        for (int pouchCounter = 0; pouchCounter < numPouches; pouchCounter++) {
          if (0b1 & pouchCounter) {
            messageValue = (message[(pouchCounter + 1) / 2] & 0b01110000) >> 4;
          }
          else {
            messageValue = (message[(pouchCounter + 1) / 2] & 0b00000111); 
          }
          target[pouchCounter] = messageValue * sensorScalar + sensorOffset;
        }
      }
    }
  }
  times[4] = micros();

//  Serial.print("Close Valves: ");
//  Serial.println(times[1] - times[0]);
//  Serial.print("Open Valves: ");
//  Serial.println(times[2] - times[1]);
//  Serial.print("Tank: ");
//  Serial.println(times[3] - times[2]);
//  Serial.print("Serial: ");
//  Serial.println(times[4] - times[3]);
  
  // if (found) {
  //   Serial.print("Serial: ");
  //   Serial.println(times[4] - times[3]);
  // }
}
