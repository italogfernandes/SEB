/*
 Ronaldo Sena
 ronaldo.sena@outlook.com
 July 2017
 Use it as you please. If we meet some day, and you think
 this stuff was helpful, you can buy me a beer

 Sétimo projeto da disciplina Sinais e Sistemas em Engenharia Biomédica
 "Controlador PID"
 */


//Tune this according
#define pinSetPoint A0
#define pinProcess A1
#define pinPWM 12
#define pinDAC DAC1
//Set the analog resolution in bits (i.e. 2^bits)
#define analogResolution 10 
#define debugInterval 1000
#define timeToTakeSample 1000

//Global variables
long eventTime = 0;
int analogValue = pow(2,analogResolution),
      processVariable,
      controlVariable;
float sampleTime,
      setPoint,
      P,
      I,
      D,
      kP = 1.1,
      kI = 1.1,
      kD = 1.1,
      error;

void setup()
{
  //Sets the baudrate as the maximum available
  Serial.begin(115200);
  //Sets both write and read resolution according to what
  //was define in the preprocessor directives
  analogWriteResolution(analogResolution);
  analogReadResolution(analogResolution);
  //not strictly required, but nevertheless
  pinMode(pinPWM, OUTPUT);
}

//Main loop
void loop()
{
  //Taking the time at every loop iteration
  long currentTime = millis();
  //If it is time to calculate de PID output
  if(currentTime - eventTime >= timeToTakeSample)
  {
      eventTime = currentTime;
      runPID();
  }
  //Change this at the preprocessor directives
  debug(debugInterval);
}

//Main function of PID control
void runPID()
{
  //Takes a read of the setpoint
  setPoint = analogRead(pinSetPoint);
  //Takes a read of the process value
  processVariable = analogRead(pinProcess);
  //Calculates the error
  error = referenceValue - processVariable;
  //Set the constants according to the sample time
  setConstants();
  //Calculates each of PID terms
  P = calculateProportional();
  I = calculateIntegrative();
  D = calculateDerivative();
  //Another way to calculates PID, is to add a bias value
  //In this case, bias value is the reference itself
  //controlVariable = (int)(referenceValue + P + I + D);
  controlVariable = (int)(P + I + D);
  //Sends the control value to both PWM and DAC pins
  analogWrite(pinControlePWM, controlVariable);
  analogWrite(pinControleDAC, controlVariable);
  //The DAC pin has a physical limatation (1/6 ~ 5/6 of voltage reference)
  //In the case of arduino Due, a sixth of 3.3 volts
}

//Set the constants according to PID theoretical values
void setConstants()
{
  //Converting to seconds, as the directive was defined in ms
  sampleTime = ((float)timeToTakeSample)/1000.0;
  kP = kP;
  kI = kI * sampleTime;
  kD = kD / sampleTime;
}

//Calculates the proportional value
float calculateProportional() 
{
  float proportional = error * kP;
  //Set the limits according to was predefined
  proportional = setLimits(proportional);
  return proportional;
}

//Calculates the integrative term
float calculateIntegrative() 
{
  //The static variable initializes with zero
  static float integral;
  //According to the integral formula
  integral += error * kI;
  //Set the limits according to was predefined
  integral = setLimits(integral);
  return integral;
}

//Calculates the derivative term
float calculateDerivative() 
{
  static float derivative;
  static float previousError;
  static float deltaError;
  //calculates the delta error
  deltaError = error - previousError;
  //Aplies the calculus derivative definition
  derivative = (deltaError * kD)/(float)sampleTime;
  //Set the limits according to was predefined
  derivative = setLimits(derivative);
  //Save the previous error, as it will be used again
  previousError = error;
  return derivative;
}

//Function the set the limit of each PID term
float setLimits(float valueToLimit)
{
  //Set the maximum value
  if(valueToLimit >= analogValue)
    valueToLimit = analogValue;
  //Set the minimum value
  if(valueToLimit <= 0)
    valueToLimit = 0;
  return valueToLimit;
}

//Shows the main variables in the serial monitor
void debug()
{
	static long eventTime;
	long currentTime = millis();
	if(currentTime - eventTime >= debugInterval)
	{
	    eventTime = currentTime;
	    print("\nSet Point:");
	    println(setPoint);
	    print("Process:");
	    println(processVariable);
	}
}