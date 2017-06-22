/*
 Ronaldo Sena
 ronaldo.sena@outlook.com
 June 2017
 Use it as you please. If we meet some day, and you think
 this stuff was helpful, you can buy me a beer

 Sexto projeto da disciplina Sinais e Sistemas em Engenharia Biomédica
 "Gerador de formas de onda"
 */

#define	pin_SENO 		6
#define	pin_TRIANGULAR 	7
#define	pin_QUADRADA 	8
#define	pin_SERRA 		9

float rads, step, seno[4095];
long period = 1;

void setup()
{
	analogWriteResolution(12);
	pinMode(pin_SENO, INPUT_PULLUP);
	pinMode(pin_TRIANGULAR, INPUT_PULLUP);
	pinMode(pin_QUADRADA, INPUT_PULLUP);
	pinMode(pin_SERRA, INPUT_PULLUP);


	rads = 2.0 * 3.14159265359; //360 degrees into rads
	step = rads / 4095.0;

	for (int i = 0; i < 4096;i++)
		seno[i] = (sin(i*step) + 1);//fitting to my needs
}

void loop()
{
	
}

//sinusoid generator
//receives as input:
//period in milliseconds
//and amplitude 
void sinusoid(long period, int amplitude)
{
	//I'm not concerned with period accuracy
	int _period = period/4096;
	for(int i = 0; i<4096; i++)
	{
		analogWrite(DAC0, (amplitude*seno[i]));
		delay(_periodo);
	}
}

void quardrada(long period, int amplitude)
{
	analogWrite(DAC0, amplitude);
	delay(period/2);
	analogWrite(DAC0, 0);
	delay(period/2);
}

void serra(long period)
{
	//I'm not concerned with period accuracy
	long _periodo = period/4096;
	for(int i = 0; i < 4096; i++)
	{
		analogWrite(DAC0, i);
		delay(_periodo);	    
	}
}

void triangular(long period)
{
	//I'm not concerned with period accuracy
	long _periodo = period/(4096*2);
	int i;
	for(i = 0; i < 4096; i++)
	{
		analogWrite(DAC0, i);
		delay(_periodo);	    
	}
	for(i = 4095; i >= 0; i--)
	{
		analogWrite(DAC0, i);
		delay(_periodo);	    
	}
}