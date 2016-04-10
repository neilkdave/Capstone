const long inflateInterval = 50;
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

const int inflatePin = 13;
const int deflatePin = 12;

unsigned long previousMillis = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(inflatePin, OUTPUT);
  pinMode(deflatePin, OUTPUT);
  controlState = StateDeflateWait;
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  unsigned long delta = currentMillis - previousMillis;

  if (controlState == StateInflate)
  {
    if (delta >= inflateInterval)
    {
      digitalWrite(inflatePin, LOW);
      previousMillis = currentMillis;
      controlState = StateInflateWait;
    }   
  }
  else if (controlState == StateInflateWait)
  {
    if (delta >= inflateWaitInterval)
    {
      digitalWrite(deflatePin, HIGH);
      previousMillis = currentMillis;
      controlState = StateDeflate;
    }
  }

  else if (controlState == StateDeflate)
  {
    if (delta >= deflateInterval)
    {
      digitalWrite(deflatePin, LOW);
      previousMillis = currentMillis;
      controlState = StateDeflateWait;
    }
  }

  else if (controlState == StateDeflateWait)
  {
    if (delta >= deflateWaitInterval)
    {
      digitalWrite(inflatePin, HIGH);
      previousMillis = currentMillis;
      controlState = StateInflate;
    }
  }
}
