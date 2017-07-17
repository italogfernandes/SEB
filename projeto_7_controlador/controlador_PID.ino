/*
 Ronaldo Sena
 ronaldo.sena@outlook.com
 July 2017
 Use it as you please. If we meet some day, and you think
 this stuff was helpful, you can buy me a beer

 Sétimo projeto da disciplina Sinais e Sistemas em Engenharia Biomédica
 "Controlador PID"
  
  TODO: comentar e testar
 */


//Tune this according
#define pinReference A0
#define pinProcess A1
#define pinPWM 12
#define pinDAC DAC1
#define analogResolution 10
#define debugInterval 2000
#define timeToIntegrate 1000
#define timeToDerivate 1000

int analogValue = pow(2,analogResolution);
int controlValue;
float referenceValue;,
      processValue,
      P,
      I,
      D,
      kP = 1.1,
      kI = 1.1,
      kD = 1.1,
      error,
      PID;

void setup()
{
	Serial.begin(115200);
	analogWriteResolution(analogResolution);
	analogReadResolution(analogResolution);
  pinMode(pinPWM, OUTPUT);
}

void loop()
{
  referenceValue = analogRead(pinReference);
  processValue = analogRead(pinProcess);
  error = referenceValue - processValue;

  
  P = calculateProportional();
  I = calculateIntegrative(timeToIntegrate);
  D = calculateDerivative(timeToDerivate);

  controlValue = (int)(referenceValue + P + I + D);

  analogWrite(pinControlePWM, controlValue);
  analogWrite(pinControleDAC, controlValue);

	debug(2000);
}

float calculateProportional() 
{
  float proportional = error * kP;
  proportional = setLimits(proportional);
  return proportional;
}

float calculateIntegrative(long integrationTime) 
{
  static long eventTime;
  static float integral;

  long currentTime = millis();
  
  if(currentTime - eventTime >= integrationTime)
  {
      integral += error * kI;
      integral = setLimits(integral);
      eventTime = currentTime;
  }

  return integral;
}

float calculateDerivative(long derivativeTime) 
{
  static long eventTime;
  static float derivative;
  static float previousError;
  static float deltaError;

  long currentTime = millis();
  
  if(currentTime - eventTime >= integrationTime)
  {
    deltaError = error - previousError;
    derivative = (deltaError * kD)/(float)derivativeTime;
    derivative = setLimits(derivative);
    previousError = error;
    eventTime = currentTime;
  }

  return derivative;
}

float setLimits(float valueToLimit)
{
  if(valueToLimit >= analogValue)
    valueToLimit = analogValue;
  if(valueToLimit <= 0)
    valueToLimit = 0;

  return valueToLimit;
}

void debug(int interval)
{
	static long eventTime;
	long currentTime = millis();
	
	if(currentTime - eventTime >= debugInterval)
	{
	    eventTime = currentTime;
	    print("\n\nReference:");
	    println(referenceValue);
	    print("Process:");
	    println(processValue);
	}
}