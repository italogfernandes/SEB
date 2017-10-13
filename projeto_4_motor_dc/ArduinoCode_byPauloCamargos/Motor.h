
/* DOCUMENTACAO CLASSE MOTOR
 *  Classe desenvolvida por Paulo C. Silva, outubro/2017.
 *  Descri��o: Classe para controle de motores utilizando o modulo l298n.
 *  Atributos:
 *    _pinin1 - Pino IN1 conectado do modulo.
 *    _pinin2 - Pino IN2 conectado do modulo.
 *    _pinenable - Pino ENABLE do m�dulo.
 *    _sentido  - variavel do tipo enum para armazenamento o sentido de rotaacao
 *                do motor.
 *    _last_time - Variavel auxiliar para funcao millis();
 *  Metodos:
 *    getters() e setters();
 *    void rodaMotor(float f_tensao) - Inicia a rotacao de acordo com a tensao f_tensao.
 *      - Para f_tensao = 0, motor parado.
 *      - Para f_tensao >= 3.3/2, sentido horario.
 *      - Para f_tensao <= 3.3/2, sentido anti-horario.
 *    void dancaMotor(int tempo) - Gira motor com sentido variado e intervalo definido (tempo).
 *    void giraHorario(float tensao) - Gira motor girar em sentido horario.
 *    void giraAntiHorario(float tensao) - Gira motor girar em sentido anti-horario.
 *    void stopMotor(float tensao) - Para o motor.
 *    void desligaMotor() - Desativa (0) os pinos conectados ao madulo.
 */
//Biblioteca geral do arduino para ter acesso as funções millis(), digitalWrite(), etc...
#include<Arduino.h>

//Protecao contra incluir 2 vezes a biblioteca
#ifndef "Motor_H" //Se ainda nao foi incluida
#define "Motor_H" //Define essa flag para avisar q agr esta sendo incluida

typedef enum sentido_t {ANTI_HORARIO, HORARIO, PARADO};

class Motor {
  private:
    uint8_t _pinin1;
    uint8_t _pinin2;
    uint8_t _pinenable;
    sentido_t _sentido;
    uint32_t _last_time; //Variavel auxiliar para a fun��o millis(), ignorar.
  public:
    Motor(uint8_t in1, uint8_t in2, uint8_t enable);
    int getIN1(); void setIN1(uint8_t in1);
    int getIN2(); void setIN2(uint8_t in2);
    int getEnable(); void setEnable(int veloc);
    sentido getSentido(); void setSentido(sentido s);
    void rodaMotor(float f_tensao);
    void dancaMotor(float f_tensao, int tempo);
    void giraHorario(float tensao);
    void giraAntiHorario(float tensao);
    void stopMotor(float tensao);
    void desligaMotor();
};

Motor::Motor(uint8_t in1, uint8_t in2, uint8_t enable) {
  this->_pinin1 = in1;
  this->_pinin2 = in2;
  this->_pinenable = enable;
  this->_last_time = 0;
}

int Motor::getIN1() {
  return this->_pinin1;
}

int Motor::getIN2() {
  return this->_pinin2;
}

int Motor::getEnable() {
  return this->_pinenable;
}

sentido_t Motor::getSentido() {
  return this->_sentido;
}

void Motor::setIN1(uint8_t in1) {
  this->_pinin1 = in1;
}

void Motor::setIN2(uint8_t in2) {
  this->_pinin2 = in2;
}

void Motor::setEnable(uint8_t enable) {
  this->_pinenable = enable;
}
void Motor::setSentido(sentido_t s) {
  this->_sentido = s;
}

void Motor::giraHorario(float f_tensao) {
  int tensao = f_tensao * 100; //Cuidado com esses ints declarados aqui
  //Nao sei se existe Garbage colector nessa linguagem
  //Talvez você esteja acumulando lixo
  int cicloPWM = map(tensao, 330 / 2, 330, 0, 255); //Nao entendi mto bem mas deve funcionar
  setSentido(HORARIO);
  digitalWrite(_pinin2, 0);
  digitalWrite(_pinin1, 1);
  analogWrite(_pinenable, cicloPWM);
  Serial.println("-- SENTIDO HORARIO | TENSAO: " + new String(f_tensao,2) + " V");
}

void Motor::giraAntiHorario(float f_tensao) {
  int tensao = f_tensao * 100;
  int cicloPWM = map(tensao, 0, 330 / 2, 0, 255);
  setSentido(ANTI_HORARIO);
  digitalWrite(_pinin1, 0);
  digitalWrite(_pinin2, 1);
  analogWrite(_pinenable, cicloPWM);
  Serial.println("-- SENTIDO ANTI-HORARIO | TENSAO: " + new String(f_tensao,2) + " V");
}

void Motor::stopMotor(float f_tensao) { // Freio magn�tico do motor
  setSentido(PARADO);
  digitalWrite(_pinin1, 1);
  digitalWrite(_pinin2, 1);
  analogWrite(_pinenable, 0);
  Serial.println("-- MOTOR PARADO | TENSAO: " + new String(f_tensao) + " V");
}

void Motor::rodaMotor(float f_tensao) {
  int tensao = f_tensao * 100;
  if (tensao == 330 / 2)
    stopMotor(tensao);
  else
    tensao >= 330 / 2 ? giraHorario(tensao) : giraAntiHorario(tensao);
}

void Motor::dancaMotor(float f_tensao, int tempo) { //Haha
  int tensao = f_tensao * 100;
  if (millis() - _last_time > tempo) {
    _last_time = millis();
    _sentido ? giraHorario(map(tensao, 0, 330, 165, 330)) : giraAntiHorario(map(tensao, 0, 330, 0, 165));
  }
}

void Motor::desligaMotor(){
  setSentido(PARADO);
  digitalWrite(_pinin1, 0);
  digitalWrite(_pinin2, 0);
  analogWrite(_enable, 0);
  Serial.println("-- MOTOR DESLIGADO ---------- ");
}
#endif /*Motor_h*/
// --------------------------- FIM DA CLASSE Motor.h -----------------
