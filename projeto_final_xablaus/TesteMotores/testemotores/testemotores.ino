#define motorA 10
#define motorB 9
#define pot A3


int leitura;
int velocidade;

void setup()
{
  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);//esvaziar
}

void loop() {
  leitura = analogRead(pot);
  velocidade = leitura/4;
  
  analogWrite(motorA, velocidade);
  analogWrite(motorB, velocidade);

}
