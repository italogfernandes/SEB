
#define pinLed1 8
#define pinLed2 9
#define pinLed3 10
#define pinLed4 11
#define pinPotenciometro A0

#define Para_Roda 12
#define pinChaveSeq 13

int i = 2;
int j = 5;
int valor;
int indicadorSeq = 0;
int tempo;
unsigned long tempopassado = 0;
String string1, string2, string3, string4, frase, frase2;

void Seq1(int i)
{
  if(i == 1)
  {
    analogWrite(pinLed1, 51);
    analogWrite(pinLed2, 0);
    analogWrite(pinLed3, 0);
    analogWrite(pinLed4, 51);
  }
  else
  {
    analogWrite(pinLed1, 0);
    analogWrite(pinLed2, 204);
    analogWrite(pinLed3, 204);
    analogWrite(pinLed4, 0);
  }
}

void Seq2(int j)
{
  if (j == 5)
  {
    analogWrite(pinLed1, 0);
    analogWrite(pinLed2, 0);
    analogWrite(pinLed3, 0);
    analogWrite(pinLed4, 0);
  }
  else
  {
    if (j == 4)
    {
      analogWrite(pinLed1, 255);
      analogWrite(pinLed2, 255);
      analogWrite(pinLed3, 255);
      analogWrite(pinLed4, 255);
    }
    else
    {
      if( j == 3)
      {
        analogWrite(pinLed1, 178);
        analogWrite(pinLed2, 178);
        analogWrite(pinLed3, 178);
        analogWrite(pinLed4, 0);
      }
      else
      {
        if (j == 2)
        {
          analogWrite(pinLed1, 102);
          analogWrite(pinLed2, 102);
          analogWrite(pinLed3, 0);
          analogWrite(pinLed4, 0);
        }
        else
        {
          analogWrite(pinLed1, 51);
          analogWrite(pinLed2, 0);
          analogWrite(pinLed3, 0);
          analogWrite(pinLed4, 0);
        }
      }
    }
  }
}

void desliga()
{
  analogWrite(pinLed1, 0);
  analogWrite(pinLed2, 0);
  analogWrite(pinLed3, 0);
  analogWrite(pinLed4, 0);
  indicadorSeq = 0;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  pinMode(pinLed1, OUTPUT);
  pinMode(pinLed2, OUTPUT);
  pinMode(pinLed3, OUTPUT);
  pinMode(Para_Roda, INPUT_PULLUP);
  pinMode(pinChaveSeq, INPUT_PULLUP);
  string1 = "Sequencia: ";
  string2 = "    Tempo: ";
  string3 = " ms";
  string4 = "Tensão: ";
  attachInterrupt(digitalPinToInterrupt(Para_Roda),desliga,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

  valor = analogRead(pinPotenciometro);
  tempo = map(valor, 0, 1023, 100, 1000);

  unsigned long tempoatual = millis();
  frase = string1+ indicadorSeq + string2 + tempo + string3;
  Serial.println(frase);

  if(tempoatual - tempopassado >=tempo)
  {
    if(!digitalRead(pinChaveSeq)) //se a chave Seq estiver em 0
    {
      i = i+1;
      if(i == 3)
     {
      i = 1;
     }

      Seq1(i);
      indicadorSeq = 1;
      Serial.println(i);
    }
    else //caso a chave Seq estiver em 1
    {
      j = j+1;
      if (j  == 6)
      {
        j = 1;
      }

      Seq2(j);
      Serial.println(j);
      indicadorSeq = 2;
    }

   tempopassado = tempoatual;
  }
}
