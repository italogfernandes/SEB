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
#include<led_rgb.h>
#include<cores_rgb.h>
led_rgb status_led(11, 12, 13); //R_pin,G_pin,B_pin

#define ANALOG_ANGULO A0

#define PINO_MOTOR_A    2
#define PINO_IN1        3
#define PINO_IN2        4
#define PINO_IN3        5
#define PINO_IN4        6
#define PINO_MOTOR_B    7

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

bool executando;

int counter_bt;

void setup() {
  Kp = 0;
  Ki = 0;
  Kd = 0;
  set_point =  180;
  executando = false;

  Serial.begin(115200);//Inicia a Serial
  Serial1.begin(9600);//Inicia a Serial
  pinMode(PINO_MOTOR_A, OUTPUT);
  pinMode(PINO_IN1, OUTPUT);
  pinMode(PINO_IN2, OUTPUT);
  pinMode(PINO_MOTOR_B, OUTPUT);
  pinMode(PINO_IN3, OUTPUT);
  pinMode(PINO_IN4, OUTPUT);
  status_led.init();
  setDirection(true);
}


String cmdserial;
void loop() {
  //----------------------------------------------------
  //Ajusta Valor das constanste Kp,Ki e Kd se necessario

  recebe_comando_serial(); //Atualiza variaveis
  recebe_comando_bluetooth(); //Atualiza variaveis

  //----------------------------------------------------
  //Realiza leitura do angulo e conversao
  angulo_lido = analogRead(ANALOG_ANGULO) * 270.0f / 1024.0f + 45.0f;; //Angulo lido de 45 ate 315

  //----------------------------------------------------
  //Calculo do erro, deltaErro e DeltaT
  erro_angulo = angulo_lido - set_point;
  deltaErro = erro_angulo - last_erro;

  actualTime = micros();
  deltaT = (float) (actualTime - lastTime ) / 1000000.0;

  last_erro = erro_angulo;
  lastTime = actualTime;
  //----------------------------------------------------
  //Executa o controle  PID
  if (executando) {
    controle_pid(); //Executa o controle em si
  } else {
    digitalWrite(PINO_MOTOR_B, 0);
  }

  //----------------------------------------------------
  //Mostra status
  if (millis() - last_print_time >= 100) {
    last_print_time = millis();
    enviar_status();//Mostra o status do sistema
  }
  atualizar_led(); //E muda a cor de um led rgb
}

void recebe_comando_serial() {
  if (Serial.available()) {
    cmdserial = Serial.readStringUntil('\n');
    if (cmdserial.startsWith("?")) {
      mostrar_constantes_serial();
      res_proporcional = 0; res_integral = 0; res_derivativo = 0;
    } else if (cmdserial.startsWith("kp")) {
      Kp = cmdserial.substring(2).toFloat();
      mostrar_constantes_serial();
      res_proporcional = 0; res_integral = 0; res_derivativo = 0;
    } else if (cmdserial.startsWith("ki")) {
      Ki = cmdserial.substring(2).toFloat();
      mostrar_constantes_serial();
      res_proporcional = 0; res_integral = 0; res_derivativo = 0;
    } else if (cmdserial.startsWith("kd")) {
      Kd = cmdserial.substring(2).toFloat();
      mostrar_constantes_serial();
      res_proporcional = 0; res_integral = 0; res_derivativo = 0;
    } else if (cmdserial.startsWith("vai")) {
      executando = true;
      Serial.println("*********EXECUCAO INICIADA********");
      res_proporcional = 0; res_integral = 0; res_derivativo = 0;
    } else if (cmdserial.startsWith("para")) {
      executando = false;
      Serial.println("*********EXECUCAO INTERROMPIDA********");
      res_proporcional = 0; res_integral = 0; res_derivativo = 0;
    } else if (cmdserial.startsWith("set")) {
      //----------------------------------------------------
      //Realiza leitura do setPoint
      set_point = cmdserial.substring(3).toFloat();
      Serial.print("Set Point setado para: ");
      Serial.println(set_point);
      res_proporcional = 0; res_integral = 0; res_derivativo = 0;
    }
  }
}

void recebe_comando_bluetooth() {
  if (Serial1.available()) {
    cmdserial = Serial1.readStringUntil('\n');
    if (cmdserial.startsWith("?")) {
      mostrar_constantes_bt();
      res_proporcional = 0; res_integral = 0; res_derivativo = 0;
    } else if (cmdserial.startsWith("kp")) {
      Kp = cmdserial.substring(2).toFloat();
      mostrar_constantes_bt();
      res_proporcional = 0; res_integral = 0; res_derivativo = 0;
    } else if (cmdserial.startsWith("ki")) {
      Ki = cmdserial.substring(2).toFloat();
      mostrar_constantes_bt();
      res_proporcional = 0; res_integral = 0; res_derivativo = 0;
    } else if (cmdserial.startsWith("kd")) {
      Kd = cmdserial.substring(2).toFloat();
      mostrar_constantes_bt();
      res_proporcional = 0; res_integral = 0; res_derivativo = 0;
    } else if (cmdserial.startsWith("vai")) {
      executando = true;
      Serial1.println("*********EXECUCAO INICIADA********");
      res_proporcional = 0; res_integral = 0; res_derivativo = 0;
    } else if (cmdserial.startsWith("para")) {
      executando = false;
      Serial1.println("*********EXECUCAO INTERROMPIDA********");
      res_proporcional = 0; res_integral = 0; res_derivativo = 0;
    } else if (cmdserial.startsWith("set")) {
      //----------------------------------------------------
      //Realiza leitura do setPoint
      set_point = cmdserial.substring(3).toFloat();
      Serial1.print("Set Point setado para: ");
      Serial1.println(set_point);
      res_proporcional = 0; res_integral = 0; res_derivativo = 0;
    }
  }
}

void mostrar_constantes_serial() {
  Serial.print("Kp setado para: ") +
  Serial.println(Kp);
  Serial.print("Ki setado para: ") +
  Serial.println(Ki);
  Serial.print("Kd setado para: ") +
  Serial.println(Kd);
}
void mostrar_constantes_bt() {
  Serial1.print("Kp setado para: ") +
  Serial1.println(Kp);
  Serial1.print("Ki setado para: ") +
  Serial1.println(Ki);
  Serial1.print("Kd setado para: ") +
  Serial1.println(Kd);
}

void atualizar_led() {
  if (abs(erro_angulo)  > 30 ) {
    status_led.acender(0xFF << 16 | 0x00 << 8);
  } else {
    uint16_t erro_porcento = (abs(erro_angulo) * 255.0f / 30.0f);
    uint8_t vermelho = erro_porcento > 255 ? 255 : erro_porcento;
    vermelho = erro_porcento < 0 ? 0 : erro_porcento;
    uint8_t verde = 255 - vermelho;
    status_led.acender(vermelho << 16 | verde << 8);
  }
}

void enviar_status() {
  Serial.println(
    String(set_point, 2) + "\t" +
    String(angulo_lido, 2) + "\t" +
    String(erro_angulo, 2)  + "\t" +
    String(res_proporcional, 2)  + "\t" +
    String(res_integral, 2)  + "\t" +
    String(res_derivativo, 2)  + "\t" +
    String(res_pid, 2)
  );
  ++counter_bt %= 5;
  if (counter_bt == 0) {
    Serial1.println(
      String(set_point, 2) + "\t" +
      String(angulo_lido, 2) + "\t" +
      String(erro_angulo, 2)  + "\t" +
      String(res_proporcional, 2)  + "\t" +
      String(res_integral, 2)  + "\t" +
      String(res_derivativo, 2)  + "\t" +
      String(res_pid, 2)
    );
  }
}

void setDirection(bool is_clockwise) {
  digitalWrite(PINO_IN1, is_clockwise);
  digitalWrite(PINO_IN2, !is_clockwise);
  digitalWrite(PINO_IN3, is_clockwise);
  digitalWrite(PINO_IN4, !is_clockwise);
}

void controle_pid() {

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
  setDirection(res_pid < 0);
  analogWrite(PINO_MOTOR_B, (uint8_t) abs(res_pid));

  //----------------------------------------------------
  //Protecao do sistema..
  if (angulo_lido > 245 && res_pid < 0) {
    analogWrite(PINO_MOTOR_B, 0);
  }
  
  if (angulo_lido < 130 && res_pid > 0) {
    analogWrite(PINO_MOTOR_B, 0);
  }
}

