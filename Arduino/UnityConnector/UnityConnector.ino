#include <SerialCommand.h>

SerialCommand sCmd;

void setup() {
 
 
   pinMode(13, OUTPUT);
   
  Serial.begin(9600);
  while (!Serial);
  sCmd.addCommand("17800", pingHandler);
  sCmd.addCommand("ECHO", echoHandler);
 
}

void loop() {
  digitalWrite(13,LOW);
  // Your operations here

  if (Serial.available() > 0)
    sCmd.readSerial();

  delay(50);  
}

void pingHandler()
{
  digitalWrite(13,HIGH);
//  delay(500);
 // digitalWrite(13,LOW);
}


void echoHandler ()
{
  char *arg;
  arg = sCmd.next();
  if (arg != NULL)
    Serial.println(arg);
  else
    Serial.println("nothing to echo");
}


