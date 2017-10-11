#include<led_rgb.h>
#include<cores_rgb.h>

#define PIN_SEQ   6
#define PIN_ONOFF 7

const int red_leds[4] = {2,3,4,5};

led_rgb status_led(11, 12, 13); //R_pin,G_pin,B_pin

typedef enum {
  ESTADO_I,
  ESTADO_II,
  ESTADO_III,
  ESTADO_IV,
  ESTADO_V
} estados_t;

estados_t estado_atual = ESTADO_I;
unsigned long tempo_atual, tempo_desejado, tempo_para_printar;
uint16_t tempo_espera, tempo_serial;

void setup() {
  tempo_serial = 100;

  status_led.init();
  for(int i = 0; i < 4; i++){
    pinMode(red_leds[i],OUTPUT);
  }
  for(int i = 0; i < 4; i++){
    digitalWrite(red_leds[i],1);
    delay(500);
    digitalWrite(red_leds[i],0);
  }
  pinMode(PIN_SEQ,INPUT_PULLUP);
  pinMode(PIN_ONOFF,INPUT_PULLUP);

  Serial.begin(115200);
  Serial.println("Iniciado com Sucesso.");
}

void loop() {
  tempo_espera = map(analogRead(A0),0,1023,100,1000);
  tempo_atual = millis();

  if(tempo_atual >= tempo_desejado){
    tempo_desejado = tempo_atual + tempo_espera;
    doRoutine();
  }

  if(tempo_atual >= tempo_para_printar){
    tempo_para_printar = tempo_atual + tempo_serial;
    showStatus();

    digitalRead(PIN_SEQ)?status_led.acender(LED_COLOR_AQUA):status_led.acender(LED_COLOR_YELLOW);
    if (!digitalRead(PIN_ONOFF)){
      status_led.acender(LED_COLOR_RED);
    }

  }
}

void showStatus(){
  Serial.print("Sequencia: ");
  digitalRead(PIN_SEQ)? Serial.print("1"):Serial.print("2");
  Serial.print("\t");
  Serial.print("Tempo: ");
  Serial.print(tempo_espera);
  Serial.print(" ms\t-\t");
  Serial.print("Status: ");
  digitalRead(PIN_ONOFF)? Serial.print("Off"):Serial.print("On");
  Serial.print("\n");
}

void doRoutine(){
  if(digitalRead(PIN_ONOFF)){
    digitalRead(PIN_SEQ)?doSeq1():doSeq2();
  }
}

void doSeq1(){
  switch (estado_atual) {
    case ESTADO_I:
    analogWrite(red_leds[0], 51); //20%
    analogWrite(red_leds[3], 51);
    digitalWrite(red_leds[1],0);
    digitalWrite(red_leds[2],0);
    estado_atual = ESTADO_II;
    break;
    case ESTADO_II:
    analogWrite(red_leds[1], 204); //80%
    analogWrite(red_leds[2], 204);
    digitalWrite(red_leds[0],0);
    digitalWrite(red_leds[3],0);
    estado_atual = ESTADO_I;
    break;
    case ESTADO_III:
    estado_atual = ESTADO_I;
    break;
    case ESTADO_IV:
    estado_atual = ESTADO_I;
    break;
    case ESTADO_V:
    estado_atual = ESTADO_I;
    break;
    default:
    estado_atual = ESTADO_I;
    break;
  }
}

void doSeq2(){
  switch (estado_atual) {
    case ESTADO_I:
    digitalWrite(red_leds[0],0);
    digitalWrite(red_leds[1],0);
    digitalWrite(red_leds[2],0);
    digitalWrite(red_leds[3],0);
    estado_atual = ESTADO_II;
    break;
    case ESTADO_II:
    analogWrite(red_leds[0],51);
    digitalWrite(red_leds[1],0);
    digitalWrite(red_leds[2],0);
    digitalWrite(red_leds[3],0);
    estado_atual = ESTADO_III;
    break;
    case ESTADO_III:
    analogWrite(red_leds[0],102);
    analogWrite(red_leds[1],102);
    digitalWrite(red_leds[2],0);
    digitalWrite(red_leds[3],0);
    estado_atual = ESTADO_IV;
    break;
    case ESTADO_IV:
    analogWrite(red_leds[0],178);
    analogWrite(red_leds[1],178);
    analogWrite(red_leds[2],178);
    digitalWrite(red_leds[3],0);
    estado_atual = ESTADO_V;
    break;
    case ESTADO_V:
    digitalWrite(red_leds[0],1);
    digitalWrite(red_leds[1],1);
    digitalWrite(red_leds[2],1);
    digitalWrite(red_leds[3],1);
    estado_atual = ESTADO_I;
    break;
    default:
    estado_atual = ESTADO_I;
    break;
  }
}
