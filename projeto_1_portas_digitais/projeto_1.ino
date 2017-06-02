/*
 Ronaldo Sena
 ronaldo.sena@outlook.com
 Maio de 2017
 Use it as you please. If we meet some day, and you think
 this stuff was helpful, you can buy me a beer

 Primeiro projeto da disciplina Sinais e Sistemas em Engenharia Biomédica
 "Utilização das portas digitais do Arduino"
 */

#define pinSWITCH 2

//Struct that will store some LED properties
typedef struct
{
	unsigned long onTime;
	unsigned long offTime;
	unsigned long eventTime = 0;
	int pin;
	boolean state = false;
}LED;

//Some global variables
LED redLED;
LED yellowLED;
LED greenLED;
boolean watcher;
unsigned long blinkThisFor;

//Optional parameter "blinkThisFor" is used for intermittent blinking of some LED 
//given a certain condition
void controlLigth(LED *pLED, boolean watcher, unsigned long blinkThisFor = 0)
{
	//If my watcher is activated
	if(!watcher)
	{
		//and the blink interval is different from zero
		if(blinkThisFor != 0)
		{
			unsigned long currentTime = millis();
			if((currentTime - pLED->eventTime >= blinkThisFor) && !pLED->state)
			{
				pLED->state = !pLED->state;
				pLED->eventTime = currentTime;
				digitalWrite(pLED->pin, pLED->state);
			}
		}
		//otherwize just turn it off
		else
		{
			pLED->state = false;
			digitalWrite(pLED->pin, pLED->state);
		}	    
	}
	//Default algorithm to do something on a certain time interval
	else
	{
		unsigned long currentTime = millis();
		if((currentTime - pLED->eventTime >= pLED->offTime) && !pLED->state)
		{
			pLED->state = !pLED->state;
			pLED->eventTime = currentTime;
			digitalWrite(pLED->pin, pLED->state);
		}
		else if((currentTime - pLED->eventTime >= pLED->onTime) && pLED->state)
		{
			pLED->state = !pLED->state;
			pLED->eventTime = currentTime;
			digitalWrite(pLED->pin, pLED->state);		
		}
	}
}

void setup()
{
	//Defining the LED properties
	redLED.onTime = 3000;
	redLED.offTime = 3000;
	redLED.pin = 13;

	yellowLED.onTime = 1000;
	yellowLED.offTime = 5000;
	yellowLED.pin = 12;

	greenLED.onTime = 2000;
	greenLED.offTime = 4000;
	greenLED.pin = 11;

	pinMode(redLED.pin, OUTPUT);
	pinMode(yellowLED.pin, OUTPUT);
	pinMode(greenLED.pin, OUTPUT);
	pinMode(pinSWITCH, INPUT_PULLUP);
}

void loop()
{
	//Setting the blinking interval
	blinkThisFor = 1000;
	//Watcher for the blink
	watcher = digitalRead(pinSWITCH);
	controlLigth(&redLED,watcher);
	controlLigth(&yellowLED,watcher,blinkThisFor);
	controlLigth(&greenLED,watcher);
}

/* Or you could just do this:

#define pinSWITCH	 2
#define pinRED 		 9
#define pinYELLOW	 10
#define pinGREEN 	 11

void setup()
{
	pinMode(pinRED,OUTPUT);
	pinMode(pinYELLOW,OUTPUT);
	pinMode(pinGREEN,OUTPUT);
	pinMode(pinSWITCH,INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(pinSWITCH), intermittent, LOW);
}
void intermittent()
{
	digitalWrite(pinRED,LOW);
	digitalWrite(pinGREEN,LOW);

	digitalWrite(pinYELLOW,LOW);
	delayMicroseconds(50000);
	digitalWrite(pinYELLOW,HIGH);
	delayMicroseconds(50000);
}
void loop()
{
	//Turn on the red LED for 3 seconds
	digitalWrite(pinRED,HIGH);
	delay(3000);
	digitalWrite(pinRED,LOW);

	//Turn on the red LED for 3 seconds
	digitalWrite(pinYELLOW,HIGH);
	delay(1000);
	digitalWrite(pinYELLOW,LOW);
	digitalWrite(pinGREEN,HIGH);
	delay(2000);
	digitalWrite(pinGREEN,LOW);
}

//But where is the fun in that?
*/
