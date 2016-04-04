const int motorControlPin = 38; // pumpEnable on breadboard // TODO: Remove after assembly 
const int pumpEnable = 53; // pumpEnable on schematic
const int numPouches = 15; // TODO: Change to 13?
const int pouchPinOffset = 22;
const int minPouchDelay = 5400; // TODO: Verify this is smallest imperceptible delay 
const int pressureReadPin = 14; // Breadboard pin 
unsigned short currentTankPressure;
unsigned long currentTime;
int current[numPouches] = {0};
int target[numPouches] = {0};

const int maxOffset = 10;
const int minOffset = -10;
int offset;
const int maxTankPressure = 250;
const int minTankPressure = 200;

const int sensorOffset = 30;
const int sensorScalar = 2;
bool busy[numPouches];
int done[numPouches];
bool inflating = false;

int calculatedDeflate;
int calculatedInflate;
char messageBody[numPouches];
char command;
char protocolVersion;
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

// TODO: Time reads, writes, and serial


void setup() {
  Serial.begin(9600);
  for (int pouchCounter = 0; pouchCounter < numPouches; pouchCounter++) {
    pinMode(inflatePins[pouchCounter], OUTPUT);
    pinMode(deflatePins[pouchCounter], OUTPUT);
  }
  pinMode(motorControlPin, OUTPUT);
  Serial.println("Ready");
}

void loop() {
  currentTime = micros();
  for (int pouchCounter = 0; pouchCounter < numPouches; pouchCounter++) { // closes valves if done
    if (busy[pouchCounter] && (done[pouchCounter] < currentTime)) {
      busy[pouchCounter] = false;
      digitalWrite(inflatePins[pouchCounter], LOW);
      digitalWrite(deflatePins[pouchCounter], LOW); // Lazy could only close open valve
    }
  }

  // Main inflation loop
  for (int pouchCounter = 0; pouchCounter < numPouches; pouchCounter++) { // inflates/deflates pouches
    if (!busy[pouchCounter]) {
      current[pouchCounter] = analogRead(pressureSensorPins[pouchCounter]); // Read pressure measurements from pouches that are not busy 
      offset = target[pouchCounter] - current[pouchCounter]; // Either adjust currentReads to fit scale or adjust target to fit Scale 
      if (offset > maxOffset) {
        calculatedDeflate = 8000; // TODO: Define more concrete calculations 
        busy[pouchCounter] = true;
        done[pouchCounter] = micros() + calculatedInflate;
        digitalWrite(deflatePins[pouchCounter], HIGH);
      }
      else if (offset < minOffset) {
        calculatedInflate = 8000; // TODO: Define more concrete calculations 
        busy[pouchCounter] = true;
        done[pouchCounter] = micros() + calculatedInflate;
        digitalWrite(inflatePins[pouchCounter], HIGH);
      }
      else if (offset < 0) {
        // Small Inflate
        busy[pouchCounter] = true;
        done[pouchCounter] = micros() + minPouchDelay;
        digitalWrite(inflatePins[pouchCounter], HIGH);
      } // Assuming if its > 0 slow leak, do nothing
    }
  }

  // Get tank pressure
  currentTankPressure = analogRead(pressureReadPin);
  if (inflating && (currentTankPressure > maxTankPressure)) {
    inflating = false;
    digitalWrite(pumpEnable, LOW);
  }
  else if (!inflating && (currentTankPressure < minTankPressure)) {
    inflating = true;
    digitalWrite(pumpEnable, HIGH);
  }

  // Serial read/parse
  // TODO: Test
  while (Serial.available()) { // Parse serial until no serial left to read
    Serial.readBytesUntil('\n', &protocolVersion, 1);
    if (protocolVersion == '1') {
      Serial.readBytesUntil('\n', &command, 1);
      if (command == '1') {
        Serial.readBytesUntil('\n', messageBody, numPouches);
        for (int pouchCounter = 0; pouchCounter < numPouches; pouchCounter++) {
          target[pouchCounter] = (messageBody[pouchCounter] - '0') * sensorScalar + sensorOffset;
        }
      }
    }
  }

  //TIME LOOP Microseconds
  
}