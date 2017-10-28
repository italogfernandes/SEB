//Se estiver usando o arduino UNO, verificar se esses pinos sao PWM
#define pinLed1 8
#define pinLed2 9
#define pinLed3 10
#define pinLed4 11
#define pinPotenciometro A0

#define Para_Roda 12
#define pinChaveSeq 13 //Ele funciona como INPUT_PULLUP mas nao eh recomentado pelo led em paralelo

int i = 2;
int j = 5;
int valor;
volatile int indicadorSeq = 0; //Deve ser volatil por causa do millis
int tempo;
unsigned long tempopassado = 0;
unsigned long tempoatual;
String string1, string2, string3, string4, frase, frase2;

void Seq1(int i) //a variavel de argumento tem o mesmo nome de uma variavel global
//tomar cuidado com isso pq poder confundir
{
  if(i == 1)
  {
    Serial.println("\t\t\tLeds:\t51\t0\t0\t51"); //Nao estou com coisas para montar entao vou debugar com esses leds
    analogWrite(pinLed1, 51);
    analogWrite(pinLed2, 0);
    analogWrite(pinLed3, 0);
    analogWrite(pinLed4, 51);
  }
  else
  {
    Serial.println("\t\t\tLeds:\t0\t204\t204\t0");
    analogWrite(pinLed1, 0);
    analogWrite(pinLed2, 204);
    analogWrite(pinLed3, 204);
    analogWrite(pinLed4, 0);
  }
}

void Seq2(int j) //Mesma coisa da variavel de argumento
{
  if (j == 5) {
    Serial.println("\t\t\tLeds:\t0\t0\t0\t0");
    analogWrite(pinLed1, 0);
    analogWrite(pinLed2, 0);
    analogWrite(pinLed3, 0);
    analogWrite(pinLed4, 0);
  } else if (j == 4) {
    Serial.println("\t\t\tLeds:\t255\t255\t255\t255");
    analogWrite(pinLed1, 255);
    analogWrite(pinLed2, 255);
    analogWrite(pinLed3, 255);
    analogWrite(pinLed4, 255);
  } else if( j == 3) {
    Serial.println("\t\t\tLeds:\t178\t178\t178\t0");
    analogWrite(pinLed1, 178);
    analogWrite(pinLed2, 178);
    analogWrite(pinLed3, 178);
    analogWrite(pinLed4, 0);
  } else if (j == 2) {
    Serial.println("\t\t\tLeds:\t102\t102\t0\t0");
    analogWrite(pinLed1, 102);
    analogWrite(pinLed2, 102);
    analogWrite(pinLed3, 0);
    analogWrite(pinLed4, 0);
  } else {
    Serial.println("\t\t\tLeds:\t51\t0\t0\t0");
    analogWrite(pinLed1, 51);
    analogWrite(pinLed2, 0);
    analogWrite(pinLed3, 0);
    analogWrite(pinLed4, 0);
  }
}

void desliga()
{
  Serial.println("Destilar called - \t\t\tLeds:\t0\t0\t0\t0");
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
  pinMode(pinLed4, OUTPUT); //Faltou o 4 aqui
  pinMode(Para_Roda, INPUT_PULLUP);
  pinMode(pinChaveSeq, INPUT_PULLUP);
  string1 = "Sequencia: ";
  string2 = "    Tempo: ";
  string3 = " ms";
  string4 = "Tensão: ";
  //Olha q chique essa menina usando interrupt
  //Coisas para se olhar antes de usar interrupt:
  // Se tiver no uno os pinos sao o 2 e o 3
  // no due sao todas
  // todas as variaveis globais dentro do interrupt devem ser volatile
  // cuidado com o millis e delay dentro de interrupt, la so funciona o micros
  attachInterrupt(digitalPinToInterrupt(Para_Roda),desliga,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

  valor = analogRead(pinPotenciometro);
  tempo = map(valor, 0, 1023, 100, 1000);

  tempoatual = millis(); //Cuidado com as declarações de variaveis dentro do loop
  //Nao sei se o C++ do arduino tem garbage colector
  frase = string1 + indicadorSeq + string2 + tempo + string3; //Isso da concatenando certo?
  //Se nao tiver trocar indicadorSeq por String(indicadorSeq)
  //Serial.println(frase);

  if(tempoatual - tempopassado >= tempo)
  {
    Serial.println(frase); //Coloquei pra printar aqui pq estava complicado aquilo escrevendo tao rapido
    if(!digitalRead(pinChaveSeq)) //se a chave Seq estiver em 0
    {
      i = i+1;
      if(i == 3)
      {
        i = 1;
      }

      Seq1(i);
      indicadorSeq = 1;
      Serial.println("i: " + String(i));
    }
    else //caso a chave Seq estiver em 1
    {
      j = j+1;
      if (j  == 6)
      {
        j = 1;
      }

      Seq2(j);
      Serial.println("j: " + String(j));
      indicadorSeq = 2;
    }

    tempopassado = tempoatual;
  }
}
