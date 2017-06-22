/*
	Codigo Temporario do projeto 7 de SEB
	implementação parcial do controlador proporcional
	falta testar
*/

#define pinReferenia	A0
#define pinValorLido	A1
#define pinControlePWM	12
#define pinControleDAC	DAC1

int referencia;
int P;
int kP = 1;
int erro;
int PID;
int bias = 512;

void setup()
{
	Serial.begin(115200);
	analogWriteResolution(10);
	analogReadResolution(10);
	referencia = analogRead(pinReferenia);
}

void loop()
{
	referencia = analogRead(pinReferenia);
	erro = lerErro(referencia);

	P = erro * kP;

	PID = P + bias;

	if(PID >= 1023)
		PID = 1023;
	if(PID <= 0)
		PID = 0;

	analogWrite(pinControlePWM, PID);
	analogWrite(pinControleDAC, PID);

	debug();
}

int lerErro(int referencia)
{
	int _valorLido;
	int _erro;
	_valorLido = analogRead(pinValorLido);
	_erro = referencia - _valorLido;
	_erro = map(_erro, -1023, 1023, -512, 512);
	return _erro;
}

void debug()
{
	static 	long tempoEvento;
	long tempoAtual = millis();
	
	if(tempoAtual - tempoEvento >= 1000)
	{
	    tempoEvento = tempoAtual;
	    print("\n\nRefenrecia:");
	    println(referencia);
	    print("Valor Lido:");
	    println(analogRead(pinValorLido));
	}
}