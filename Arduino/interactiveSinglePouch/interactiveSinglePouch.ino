/*
 Interactive mode responds to single characters in the serial input
 0 deflates the pouch
 1-9 inflate the pouch, for 10 * i milliseconds
 */

int pressureReadPin = 15;
const unsigned short maintainPressure = 250;

const int motorControlPin = 9;

const long inflateInterval = 200;
const long inflateWaitInterval     = 2000;
const long deflateInterval = 2000;
const long deflateWaitInterval     = 2000;

enum ControlStates_t
{
  StateInflate = 0,
  StateInflateWait,
  StateDeflate,
  StateDeflateWait,
};

ControlStates_t controlState = StateInflate;

int inflateState = LOW;
int deflateState = LOW;

char command;
unsigned short val;

const int inflatePin = 13;
const int deflatePin = 12;

unsigned long previousMillis = 0;
unsigned long previousMotorChange = 0;

void setup() {
  Serial.begin(9600);
  pinMode(inflatePin, OUTPUT);
  pinMode(deflatePin, OUTPUT);
  pinMode(motorControlPin, OUTPUT);
  controlState = StateDeflateWait;
}

void loop() {
  unsigned long currentMillis = millis();
  
  unsigned short pressureRead;
  pressureRead = analogRead(pressureReadPin);
  //Serial.println(pressureRead);
  
  if (Serial.available())
  {
    command = Serial.read();
    val = command - '0';
    if (val == 0)
    {
      digitalWrite(inflatePin, LOW);
      digitalWrite(deflatePin, HIGH);
    }
    else if (val < 10)
    {
      digitalWrite(deflatePin, LOW);
      digitalWrite(inflatePin, HIGH);
      delay(10 * val);
      digitalWrite(inflatePin, LOW);
    }
  }
  
  // Control for output pressure
  if ((currentMillis - previousMotorChange) > 1000)
  {
    if (pressureRead < maintainPressure)
    {
      digitalWrite(motorControlPin, HIGH);
      previousMotorChange = currentMillis;
    }
    else
    {
      digitalWrite(motorControlPin, LOW);
      previousMotorChange = currentMillis;
    }
  }
}
