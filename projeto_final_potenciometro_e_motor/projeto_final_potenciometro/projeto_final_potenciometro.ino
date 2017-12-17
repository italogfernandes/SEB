/* UNIVERSIDADE FEDERAL DE UBERLANDIA
  Biomedical Engineering
  Autors: Ítalo G S Fernandes
  Paulo Camargos Silva
  contact: italogsfernandes@gmail.com
  URLs: https://github.com/italogfernandes/SEB
  Este codigo faz parte da disciplina de sinais e sistemas
  para engenhara biomedica e visa controlar o angulo de
  Os sinais de entrada e saida sao enviados para a interface Serial
  podendo ser visualizado pelo serial plotter.
  Esquema de montagem:
  Arduino - Dispositivo
  A0    - Potenciometro para configurar o setPoint de 0 a 360 graus

  A1    - Potenciometro para configurar o Kp de 0 a 3
  A2    - Potenciometro para configurar o Ki de 0 a 3
  A3    - Potenciometro para configurar o Kd de 0 a 3

  VCC - Potenciometros entrada
  GND - Potenciometros GND

  6 - Pino Enable ou de Velocidade da PonteH
  5 - Pino In1 da PonteH
  4 - Pino In2 da PonteH

  A4 - SCL do MPU66050
  A5 - SDA do MPU6050

  Obs: O sentido do motor
*/

#define ANALOG_ANGULO A0

#define PINO_MOTOR_A    3
#define PINO_IN1        4
#define PINO_IN2        5
#define PINO_IN3        8
#define PINO_IN4        7
#define PINO_MOTOR_B    9

float set_point; //Angulo desejado,
float angulo_lido; //Angulo lido do sensor de 45º a 315ª no potenciometro
float erro_angulo; //Erro = setpoint - valor_atual
float last_erro;
float deltaErro; //Erro(t) - Erro(t-1)

float Kp, Ki, Kd;
float res_pid; //Saida o Controlador PID
float res_proporcional; //Saida o Controlador proporcional
float res_integral; //Saida o Controlador integral
float res_derivativo; //Saida o Controlador derivativo
float deltaT;
unsigned long actualTime;
unsigned long lastTime;

unsigned long last_print_time;

void setup() {
  Kp = 1;
  Ki = 2;
  
  Serial.begin(115200);//Inicia a Serial
  pinMode(PINO_MOTOR_A, OUTPUT);
  pinMode(PINO_IN1, OUTPUT);
  pinMode(PINO_IN2, OUTPUT);
  pinMode(PINO_MOTOR_B, OUTPUT);
  pinMode(PINO_IN3, OUTPUT);
  pinMode(PINO_IN4, OUTPUT);
  setDirection(true);
}


String cmdserial;
void loop() {
  //----------------------------------------------------
  //Ajusta Valor das constanste Kp,Ki e Kd se necessario
  if (Serial.available()) {
    cmdserial = Serial.readStringUntil('\n');
    if (cmdserial.startsWith("?")) {
      Serial.print("Kp setado para: ") +
      Serial.println(Kp);
      Serial.print("Ki setado para: ") +
      Serial.println(Ki);
      Serial.print("Kd setado para: ") +
      Serial.println(Kd);
    } else if (cmdserial.startsWith("kp")) {
      Kp = cmdserial.substring(2).toFloat();
      Serial.print("Kp setado para: ") +
      Serial.println(Kp);
    } else if (cmdserial.startsWith("ki")) {
      Ki = cmdserial.substring(2).toFloat();
    } else if (cmdserial.startsWith("kd")) {
      Kd = cmdserial.substring(2).toFloat();
    }
  }

  angulo_lido = analogRead(ANALOG_ANGULO) * 270.0f / 1024.0f + 45.0f;; //Angulo lido de 45 ate 315

  if (angulo_lido > 260) {
    analogWrite(PINO_MOTOR_B, 0);
  }
  if (angulo_lido < 100) {
    analogWrite(PINO_MOTOR_B, 0);
  }
  //----------------------------------------------------
  //Realiza leitura do setPoint
  set_point =  180; // determina o setpoint

  //----------------------------------------------------
  //Calculo do erro, deltaErro e DeltaT
  erro_angulo = angulo_lido - set_point;
  deltaErro = erro_angulo - last_erro;

  actualTime = micros();
  deltaT = (float) (actualTime - lastTime ) / 1000000.0;

  last_erro = erro_angulo;
  lastTime = actualTime;

  //----------------------------------------------------
  //Calcula as respostas P, I e D
  res_proporcional = Kp * erro_angulo ; //Multiplicaçao
  res_integral = Ki * erro_angulo * deltaT + res_integral; //Integração
  res_derivativo = Kd * deltaErro / deltaT; //Derivação

  res_pid = res_proporcional + res_integral + res_derivativo;
  res_pid = res_pid < -255 ? -255 : res_pid; //Limite minimo de -255
  res_pid = res_pid > 255 ? 255 : res_pid; //Limite maximo de 255

  //----------------------------------------------------
  //Joga o sinal na saida do sistema

  //analogWrite(PINO_MOTOR, (uint8_t) abs(res_pid));

  setDirection(res_pid < 0);
  //analogWrite(PINO_MOTOR_A, (uint8_t) abs(res_pid));
  analogWrite(PINO_MOTOR_B, (uint8_t) abs(res_pid));
  if (angulo_lido > 260) {
    analogWrite(PINO_MOTOR_B, 0);
  }
  if (angulo_lido < 100) {
    analogWrite(PINO_MOTOR_B, 0);
  }

  if (millis() - last_print_time >= 100) {
    last_print_time = millis();
    Serial.println(
      String(set_point, 2) + "\t" +
      String(angulo_lido, 2) + "\t" 
      //String(erro_angulo, 2)  + "\t" +
      //String(res_pid, 2)
      //String(Kp, 2) //multipliquei por 10 para ficar mais facil de visualizar no serial Plotter
      //String(euler_degrees[2], 2)
    );
  }
}

void setDirection(bool is_clockwise) {
  digitalWrite(PINO_IN1, is_clockwise);
  digitalWrite(PINO_IN2, !is_clockwise);
  digitalWrite(PINO_IN3, is_clockwise);
  digitalWrite(PINO_IN4, !is_clockwise);
}
