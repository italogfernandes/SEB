/*
 Ronaldo Sena
 ronaldo.sena@outlook.com
 April 2017
 Use it as you please. If we meet some day, and you think
 this stuff was helpful, you can buy me a beer

 Segundo projeto da disciplina Sinais e Sistemas em Engenharia Biomédica
 "Utilização das portas analógicas do Arduino"
 */

DO THIS SHIT USING CLASSES NOW MFK

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