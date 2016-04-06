#define adcPrescaler 16
#define baudRate 115300
#define inflate true
#define deflate false

const unsigned char PS_16 = (1 << ADPS2);
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

const unsigned long halfUnsignedLong = 2000000000;

const int motorControlPin = 38; // pumpEnable on breadboard // TODO: Remove after assembly 
const int pumpEnable = 53; // pumpEnable on schematic
const int maxNumPouches = 15;
const int numPouches = 13;
const int pouchPinOffset = 22;
const unsigned long minPouchDelay = 5400; // TODO: Verify this is smallest imperceptible delay 
const int pressureReadPin = 14; // Breadboard pin 
unsigned short currentTankPressure;
unsigned long currentTime;
unsigned long times[7];

int current[numPouches] = {0};
int target[numPouches] = {0};
bool valve[numPouches];

const int maxOffset = 10;
const int minOffset = -10;
int offset;
const int maxTankPressure = 250;
const int minTankPressure = 200;

const int sensorOffset = 30;
const int sensorScalar = 2;
bool busy[numPouches];
unsigned long done[numPouches];
bool inflating = false;

unsigned int calculatedDeflate;
unsigned int calculatedInflate;

// Serial Variables
const int maxMessageLength = (numPouches / 2) + 2; // 1 for version and type, 1 for newline
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

bool lessThan(unsigned long a, unsigned long b) {
  return (a < b) ? (b - a < halfUnsignedLong) : (a - b > halfUnsignedLong);
}

bool greaterThan(unsigned long a, unsigned long b) {
  return lessThan(b, a);
}

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

bool safeToContinue;
unsigned long delayTime;
unsigned long nextClose;
void closeValves(int nextBlockingTime) {
  do {
    safeToContinue = true;
    currentTime = micros();
    int pouchCounter;
    for (pouchCounter = 0; pouchCounter < numPouches; pouchCounter++) { // 100us
      if (busy[pouchCounter]) {
        nextClose = done[pouchCounter];
        safeToContinue = false;
        break;
      }
    }
    if (!safeToContinue) {
      for ( ; pouchCounter < numPouches; pouchCounter++) {
        if ((busy[pouchCounter]) && (lessThan(done[pouchCounter], nextClose))) {
          nextClose = done[pouchCounter];
        }
      }
      // Serial.println(micros() - currentTime);
      currentTime = micros();
      delayTime = nextClose - currentTime;
      if (lessThan(delayTime, nextBlockingTime)) {
        if (delayTime < halfUnsignedLong) {
          delayMicroseconds(delayTime); // TODO: add additional time here?
        }
        for (pouchCounter = 0; pouchCounter < numPouches; pouchCounter++) {
          if ((busy[pouchCounter]) && (lessThan(done[pouchCounter], currentTime))) {
            busy[pouchCounter] = false;
            if (valve[pouchCounter] == inflate) {
              digitalWrite(inflatePins[pouchCounter], LOW);
            }
            else {
              digitalWrite(deflatePins[pouchCounter], LOW);
            }
          }
        }
      }
      else {
        safeToContinue = true;
      }
    }
  } while(!safeToContinue);
}

void loop() {
  // Main inflation loop
  // 128 ADC Scaler // Default
  // Best:     0us
  // One:    130us
  // Worst: 1640us
  
  // 16 ADC Scaler // Fastest
  // Best:     0us
  // One:     32us
  // Worst:  480us

  times[0] = micros();
  
  closeValves(480);
  
  times[1] = micros();
  
  // Open Valves
  for (int pouchCounter = 0; pouchCounter < numPouches; pouchCounter++) {
    if (!busy[pouchCounter]) {
      current[pouchCounter] = analogRead(pressureSensorPins[pouchCounter]); // Read pressure measurements from pouches that are not busy 
      offset = target[pouchCounter] - current[pouchCounter]; // Either adjust currentReads to fit scale or adjust target to fit Scale 
      if (offset > maxOffset) {
        calculatedDeflate = 8000; // TODO: Define more concrete calculations 
        busy[pouchCounter] = true;
        valve[pouchCounter] = deflate;
        done[pouchCounter] = micros() + calculatedDeflate;
        digitalWrite(deflatePins[pouchCounter], HIGH);
      }
      else if (offset < minOffset) {
        calculatedInflate = 8000; // TODO: Define more concrete calculations 
        busy[pouchCounter] = true;
        valve[pouchCounter] = inflate;
        done[pouchCounter] = micros() + calculatedInflate;
        digitalWrite(inflatePins[pouchCounter], HIGH);
      }
      else if (offset < 0) {
        // Small Inflate
        busy[pouchCounter] = true;
        valve[pouchCounter] = inflate;
        done[pouchCounter] = micros() + minPouchDelay;
        digitalWrite(inflatePins[pouchCounter], HIGH);
      } // Assuming if its > 0 slow leak, do nothing
    }
  }
  
  times[2] = micros();
  
  // Get tank pressure
  // 20us - 30us
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
  
  closeValves(600);
  
  times[4] = micros();
  
  // Read Serial
  while (Serial.available()) { // Parse serial until no serial left to read
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
  times[5] = micros();

//  Serial.print("Close: ");
//  Serial.println(times[1] - times[0]);
//  Serial.print("Open: ");
//  Serial.println(times[2] - times[1]);
//  Serial.print("Tank: ");
//  Serial.println(times[3] - times[2]);
//  Serial.print("Close: ");
//  Serial.println(times[4] - times[3]);
}
