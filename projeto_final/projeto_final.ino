/* UNIVERSIDADE FEDERAL DE UBERLANDIA
   Biomedical Engineering
   Autors: Ítalo G S Fernandes
           Paulo Camargos Silva
   contact: italogsfernandes@gmail.com
   URLs: https://github.com/italogfernandes/SEB
  Este codigo faz parte da disciplina de sinais e sistemas
  para engenhara biomedica e visa emular um gerador de sinais
  utilizando as portas DAC do Arduino Due
  Tal sinal tambem sera enviado para a interface Serial
  podendo ser visualizado pelo serial plotter.
    Esquema de montagem:
    Arduino - Dispositivo
    DAC0    - Sinal de saida do canal 0
    DAC1    - Sinal de saida do canal 1
    GND     - Ground comum
*/
#include "Wire.h" //Biblioteca I2C do arduino
#include "I2Cdev.h" //Biblioteca para utilizacao de I2C melhorada, depende de Wire.h
#include "MPU6050_6Axis_MotionApps20.h" //Biblioteca com o codigo do digital motion processor (DMP)

#define ANALOG_SETPOINT A0
#define ANALOG_KP       A1
#define ANALOG_KI       A2
#define ANALOG_KD       A3
#define PINO_MOTOR      6

#define PSDMP 42 //Tamanho dos pacotes da DMP

int numbPackets; //Qnt de pacotes do processador de movimento
uint8_t fifoBuffer[64]; // FIFO storage buffer

MPU6050 mpu;
Quaternion q;           // [w, x, y, z]         quaternion container
float euler[3];         // [psi, theta, phi]    Euler angle container in rads
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
float euler_degrees[3]; // [psi, theta, phi]    Euler angle container in degress

float set_point; //Angulo desejado, de 0 a 360º
float angulo_lido; //Angulo lido do sensor
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

void setup() {
  Wire.begin(); //Inicia o I2C
  Serial.begin(115200);//Inicia a Serial
  pinMode(PINO_MOTOR, OUTPUT);
  configuraSensor();
}


void loop() {
  numbPackets = floor(mpu.getFIFOCount() / PSDMP); //Verifica qnt de leituras no sensor
  if (numbPackets >  0) { //Se tiver ao menos 1

    //----------------------------------------------------
    //Realiza leitura do sensor
    mpu.getFIFOBytes(fifoBuffer, PSDMP); //Le este pacote de dados
    mpu.dmpGetQuaternion(&q, fifoBuffer); //Obtem o valor do quaternion atraves do pacote obtido
    mpu.dmpGetEuler(euler, &q); //E então converte este quaternion para angulos de euler
    for (int i = 0; i < 3; i++) {
      euler_degrees[i] = euler[i] * 180 / M_PI; //Converte para graus
    }

    angulo_lido = euler_degrees[0]; //0 é o X do sensor, 1 é o Y do sensor e 2 e o Z do sensor

    //----------------------------------------------------
    //Realiza leitura do setPoint
    set_point = analogRead(ANALOG_SETPOINT) * 360.0 / 1024.0; //Le o SetPoint que pode ser de 0 a 360

    //----------------------------------------------------
    //Ajusta Valor das constanste Kp,Ki e Kd se necessario
    Kp = analogRead(ANALOG_KP) * 3.0 / 1024.0; //Kp,Ki e Kd podem valer de 0 a 3
    Ki = analogRead(ANALOG_KI) * 3.0 / 1024.0;
    Kd = analogRead(ANALOG_KD) * 3.0 / 1024.0;

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
    res_pid = res_pid < 0 ? 0 : res_pid; //Limite minimo de 0
    res_pid = res_pid > 255 ? 255 : res_pid; //Limite maximo de 255

    //----------------------------------------------------
    //Joga o sinal na saida do sistema
    analogWrite(PINO_MOTOR, (uint8_t) res_pid);

    //----------------------------------------------------
    //Mostra os valores na Serial para serem plotados
    Serial.println(
      String(set_point, 2) + "\t" +
      String(res_pid, 2) + "\t" +
      String(angulo_lido, 2) + "\t" +
      String(erro_angulo, 2)  + "\t" +
      String(Kp*100, 2)  + "\t" + //multipliquei por 100 para ficar mais facil de visualizar no serial Plotter
      String(Ki*100, 2)  + "\t" +
      String(Kd*100, 2)
    );
  }
}

void configuraSensor() {
  if (mpu.testConnection())
  {
    //Initializes the IMU
    mpu.initialize();

    //Initializes the DMP
    uint8_t ret = mpu.dmpInitialize();
    delay(50);

    if (ret == 0)
    {
      mpu.setDMPEnabled(true);
      //Configura os Offsets do Acelerometro e giroscopio
      //      mpu.setXAccelOffset(-566);
      //      mpu.setYAccelOffset(3075);
      //      mpu.setZAccelOffset(1248);
      //      mpu.setXGyroOffset(15);
      //      mpu.setYGyroOffset(108);
      //      mpu.setZGyroOffset(33);
    }
    else
    {
      Serial.println("Error!");
    }
  }
}


