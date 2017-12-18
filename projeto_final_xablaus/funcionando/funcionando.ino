/*
        Projeto Final de SEB

***** Regulador de nivel de agua*****

  Grupo: Ana Carolina Torres
        Henrique Andrade Barbosa
        Paulo Eduardo Alves

  Uberlândia
  Dezembro de 2017


*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Ultrasonic.h>
#define pino_trigger 6
#define pino_echo 7
#define motorA 10
#define motorB 9
#define pot A3


LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
Ultrasonic ultrasonic(pino_trigger, pino_echo);

unsigned long tempoUltimoPrint;

float entrada; //Volume desejado,
float saida; //Volume lido
float erro; //Erro = setpoint - valor_atual

float kp, ki;
float resposta; //Saida o Controlador PI
float proporcional; //Saida o Controlador proporcional
float integral; //Saida o Controlador integral
float dt; //Delta Tempo

unsigned long tempoAgora;
unsigned long tempoUltimaVez;


unsigned long ultimaVerificacao;

void setup()
{
  kp = 1;
  ki = 0.5;
  pinMode(pot, INPUT);
  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);//esvaziar
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH); //5V do Sensor ultrason


  pinMode(4, OUTPUT);
  digitalWrite(4, LOW); // GND do potenciometro
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH); //5V do potenciometro

  Serial.begin(115200);
  lcd.begin(16, 2);
}

void loop()
{
  if (millis() - ultimaVerificacao >= 100) { //Verifica e corrige de 100 em 100 ms
    ultimaVerificacao = millis();

    //Realiza leitura do volume atual
    float cmMsec;
    long microsec = ultrasonic.timing();
    saida = ultrasonic.convert(microsec, Ultrasonic::CM);
    saida = (-43.6 * (saida)) + 629.0;


    //Realiza leitura do setPoint utilizando regra de 3
    entrada = analogRead(pot) * 300.0 / 1024.0 + 100; //Convertendo para de 50 ate 450mls


    //Calculo do erro, deltaErro e dt
    erro = entrada - saida;

    tempoAgora = micros();
    dt = (float) (tempoAgora - tempoUltimaVez ) / 1000000.0;

    tempoUltimaVez = tempoAgora;


    //Calcula as respostas P, I e D
    proporcional = kp * erro ; //Multiplicaçao
    integral = ki * erro * dt + integral; //Integração

    resposta = proporcional + integral;

    //Limitando o sinal num maximo de enchimento e esvaziamento
    if (resposta > 50) {
      resposta = 50;
    }
    if (resposta < -50) {
      resposta = -50;
    }

    //Joga o sinal na saida do sistema
    if (resposta > 0) {//Se for pra encher
      analogWrite(motorA, (uint8_t) abs(resposta));//E liga o de encher com a resposta do PI
      analogWrite(motorB, 0);//Desliga o de esvaziar
    } else {//Se nao for pra encher (esvaziar)
      analogWrite(motorA, 0); //Desliga o de encher
      analogWrite(motorB, (uint8_t) abs(resposta));//E ativa o de esvaziar
    }
  }



  //Mostra no serial
  if (millis()  - tempoUltimoPrint > 100) {
    tempoUltimoPrint = millis();
    Serial.print("lido: "); Serial.print(saida); Serial.print(" ml\t");
    Serial.print("desejado: "); Serial.print(entrada); Serial.print(" ml\t");
    Serial.print("erro: "); Serial.print(erro); Serial.print(" ml\t");
    Serial.print("pi: "); Serial.print(resposta); Serial.print(" pwm\t  ");
    Serial.println();

    //Mostra no lcd
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Entrada: "+ String(entrada,0) + " ml");
    lcd.setCursor(0, 1);
    lcd.print("Saida: "+ String(saida,0) + " ml");
  }
}
