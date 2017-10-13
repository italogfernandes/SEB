
/* DOCUMENTACAO CLASSE MOTOR
 *  Classe desenvolvida por Paulo C. Silva, outubro/2017.
 *  Descri��o: Classe para controle de motores utilizando o modulo l298n.
 *  Atributos: 
 *    _in1 - Pino IN1 conectado do modulo.
 *    _in2 - Pino IN2 conectado do modulo.
 *    _enable - Pino ENABLE do m�dulo.
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
enum sentido {ANTI_HORARIO, HORARIO, PARADO};

class Motor {
  private:
    int _in1;
    int _in2;
    int _enable;
    sentido _sentido;
    int _last_time; //Variavel auxiliar para a fun��o millis(), ignorar.
  public:
    Motor(int in1, int in2, int veloc);
    int getIN1(); void setIN1(int in1);
    int getIN2(); void setIN2(int in2);
    int getEnable(); void setEnable(int veloc);
    sentido getSentido(); void setSentido(sentido s);
    void rodaMotor(float f_tensao);
    void dancaMotor(float f_tensao, int tempo);
    void giraHorario(float tensao);
    void giraAntiHorario(float tensao);
    void stopMotor(float tensao);
    void desligaMotor();
};

Motor::Motor(int in1, int in2, int enable) {
  this->_in1 = in1;
  this->_in2 = in2;
  this->_enable = enable;
  this->_last_time = 0;
}

int Motor::getIN1() {
  return this->_in1;
}

int Motor::getIN2() {
  return this->_in2;
}

int Motor::getEnable() {
  return this->_enable;
}

sentido Motor::getSentido() {
  return this->_sentido;
}

void Motor::setIN1(int in1) {
  this->_in1 = in1;
}

void Motor::setIN2(int in2) {
  this->_in2 = in2;
}

void Motor::setEnable(int enable) {
  this->_enable = enable;
}
void Motor::setSentido(sentido s) {
  this->_sentido = s;
}

void Motor::giraHorario(float f_tensao) {
  int tensao = f_tensao * 100;
  int cicloPWM = map(tensao, 330 / 2, 330, 0, 255);
  setSentido(HORARIO);
  digitalWrite(getIN2(), 0);
  digitalWrite(getIN1(), 1);
  analogWrite(getEnable(), cicloPWM);
  Serial.println("-- SENTIDO HORARIO | TENSAO: " + (String) f_tensao + " V");
}

void Motor::giraAntiHorario(float f_tensao) {
  int tensao = f_tensao * 100;
  int cicloPWM = map(tensao, 0, 330 / 2, 0, 255);
  setSentido(ANTI_HORARIO);
  digitalWrite(getIN1(), 0);
  digitalWrite(getIN2(), 1);
  analogWrite(getEnable(), cicloPWM);
  Serial.println("-- SENTIDO ANTI-HORARIO | TENSAO: " + (String) f_tensao + " V");
}

void Motor::stopMotor(float f_tensao) { // Freio magn�tico do motor
  setSentido(PARADO);
  digitalWrite(getIN1(), 1);
  digitalWrite(getIN2(), 1);
  analogWrite(getEnable(), 0);
  Serial.println("-- MOTOR PARADO | TENSAO: " + (String) f_tensao + " V");
}

void Motor::rodaMotor(float f_tensao) {
  int tensao = f_tensao * 100;
  if (tensao == 330 / 2)
    stopMotor(tensao);
  else
    tensao >= 330 / 2 ? giraHorario(tensao) : giraAntiHorario(tensao);
}

void Motor::dancaMotor(float f_tensao, int tempo) {
  int tensao = f_tensao * 100;
  if (millis() - _last_time > tempo) {
    _last_time = millis();
    _sentido ? giraHorario(map(tensao, 0, 330, 165, 330)) : giraAntiHorario(map(tensao, 0, 330, 0, 165));
  }
}

void Motor::desligaMotor(){
  setSentido(PARADO);
  digitalWrite(getIN1(), 0);
  digitalWrite(getIN2(), 0);
  analogWrite(getEnable(), 0);
  Serial.println("-- MOTOR DESLIGADO ---------- ");
}
// --------------------------- FIM DA CLASSE Motor.h -----------------
