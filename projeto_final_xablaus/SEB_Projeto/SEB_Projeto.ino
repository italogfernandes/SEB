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
float saida;
int
entrada,
erro,
proporcional,
integral = 0,
controlador,
controladorA,
controladorB,
cont = 0;
float
kp = 1,//6,
ki = 0;//-3,
int
t1 = 0,  // tempo de inicio do loop
t2 = 0,  // tempo de im do loop
t3 = 0;    //diferenca de tempo

unsigned long actual_time;
unsigned long last_print_time;
void setup()
{
  pinMode(pot, INPUT);
  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);//esvaziar
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  Serial.begin(115200);
  lcd.begin(16, 2);
}

void loop()
{

  float cmMsec;
  long microsec = ultrasonic.timing();
  saida = ultrasonic.convert(microsec, Ultrasonic::CM);
  saida = (-43.6 * (saida)) + 649.0;
  saida = (int)saida;
  entrada = analogRead(pot);
  entrada = map(entrada, 0, 1023, 0, 500);
  erro = entrada - saida;
  

  proporcional = erro * kp;

  integral += (erro * ki) * (t3 / 1000);
  if (integral > 500)
    integral = 500;
  if (integral < -500)
    integral = -500;
  controlador = proporcional + integral;


  t1 = millis();
  if (cont == 0)
    cont++;
  else
    t3 = t1 - t2;
  t2 = millis();




  if (controlador < -500)
    controlador = -500;
  if (controlador > 500)
    controlador = 500;
  controladorA = controlador;
  controladorB = controlador;
  if (controladorB > 0)
    controladorB = 0.;
  if (controladorA < 0)
    controladorA = 0;
  controladorA = map(controladorA, 0, 500, 0, 255);
  controladorB = map(controladorB, -500, 0, 255, 0);
  if (controladorA < 54)
    controladorA = 54;
  if (erro == 0)
    controladorA = 0;
  if (controladorB > 212)
    controladorB = 212;
  analogWrite(motorA, controladorA);
  analogWrite(motorB, controladorB);

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Entrada");
  lcd.setCursor(3, 1);
  lcd.print(String(entrada) + "ml");

  actual_time = millis();
  if(actual_time  - last_print_time > 250){
    last_print_time = actual_time;
    Serial.print(saida);Serial.print(" mlatual\t ");
    Serial.print(entrada);Serial.print(" mldesejado\t MotorEncher: ");
    Serial.print(controladorA);Serial.print(" pwm\t MotorEsvaziar: ");
    Serial.print(controladorB);Serial.print(" pwm\t");
    Serial.println();
  }
}

