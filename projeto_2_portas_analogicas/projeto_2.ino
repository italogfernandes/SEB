/*
 Ronaldo Sena
 ronaldo.sena@outlook.com
 April 2017
 Use it as you please. If we meet some day, and you think
 this stuff was helpful, you can buy me a beer

 Segundo projeto da disciplina Sinais e Sistemas em Engenharia Biomédica
 "Utilização das portas analógicas do Arduino"
 */

//Using classes without creating headers and cpp files
class Potentiometer
{
	private:
		int _pinPOT;
		int _analogResolution = 1023; 
		int _voltage;
	public:
		//Default method with optitional parameters	
		Potentiometer(int pin, int voltage = 5, int analogResolution = 1023) 
		{
			_pinPOT = pin;
			_voltage = voltage; //as default sets to 5 volts
			_analogResolution = analogResolution; //as default sets to 2^10 bits
		}

		float readValue()
		{
			return analogRead(_pinPOT);
		}

		float readVoltage()
		{
			return ((float)analogRead(_pinPOT) * ((float)_voltage/(float)_analogResolution));
		}
};

class LED
{
	private:
		int _pinLED;
		bool _state;

	public:
		void setLED(int pin)
		{
			_state = false;
			_pinLED = pin;
			pinMode(_pinLED, OUTPUT);
			digitalWrite(_pinLED, _state);
		}

		void turnOn()
		{
			_state = true;
			digitalWrite(_pinLED, _state);
		}

		void turnOff()
		{
			_state = false;
			digitalWrite(_pinLED, _state);
		}
};

LED ledArray[3];
Potentiometer pot(0);

void setup()
{
	for(int i = 0, pin = 13; i < 3; i++, pin--)
	    ledArray[i].setLED(pin);
	Serial.begin(9600);
}

void loop()
{
	for(int i = 0; i < 3; i++)
	{
		if(pot.readVoltage() < (float)i)
			ledArray[i].turnOff();
		else
			ledArray[i].turnOn();
	}
}

/* Or you could just do this:

#define pinPOT	 	 A0
#define pinRED 		 13
#define pinYELLOW	 12
#define pinGREEN 	 11

float tensaoPot = 0;

void setup()
{
	pinMode(pinRED,OUTPUT);
	pinMode(pinYELLOW,OUTPUT);
	pinMode(pinGREEN,OUTPUT);
	Serial.begin(9600);
}

void loop()
{
	if (tensaoPot < 1.0)
	{
		digitalWrite(pinRED, HIGH);
		digitalWrite(pinYELLOW, LOW);
		digitalWrite(pinGREEN, LOW);	    
	}
	else
	{
		if(tensaoPot < 2.0)
		{
			digitalWrite(pinYELLOW, HIGH);
			digitalWrite(pinGREEN, LOW);
		}
		else
		{
			if(tensaoPot < 3.0)
			{
				digitalWrite(pinGREEN, HIGH);
			}
		}
	}
	
	Serial.print("Tensao: ");
	Serial.print(tensaoPot);
	delay(300);

	//There is a reason for it to be here!
	tensaoPot = analogRead(pinPOT);
	tensaoPot = tensaoPot * (5.0/1023.0);
}
But where is the fun in that? */
