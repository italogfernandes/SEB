#include <Ultrasonic.h>
#define pino_trigger 6
#define pino_echo 7

Ultrasonic ultrasonic(pino_trigger, pino_echo);



void setup()
{
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void loop()
{
  Serial.print(ultrasonic.Ranging(CM)); // CM or INC
  Serial.println(" cm" );
  delay(100);
}

