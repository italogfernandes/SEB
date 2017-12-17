//---------------------------------------------------------------------------
#include "I2Cdev.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include "Timer.h"
#include "SoftwareSerial.h"
#include "madgwick.h"
//---------------------------------------------------------------------------
// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif
//---------------------------------------------------------------------------
MPU6050 mpu(0x68);
HMC5883L mag;
//---------------------------------------------------------------------------
//madgwick parameters
//TODO: Beta should be different for each sensor
float GyroMeasError = PI * (40.0f / 180.0f);
float beta = sqrt(3.0f / 4.0f) * GyroMeasError;   // compute beta
float* quat;
float quat0[4] = {1.0f, 0.0f, 0.0f, 0.0f};
float quat1[4] = {1.0f, 0.0f, 0.0f, 0.0f};
float quat2[4] = {1.0f, 0.0f, 0.0f, 0.0f};
float quat3[4] = {1.0f, 0.0f, 0.0f, 0.0f};
float quat4[4] = {1.0f, 0.0f, 0.0f, 0.0f};
float eul[3] = {0, 0, 0};
float deg[3] = {0, 0, 0};
//---------------------------------------------------------------------------
uint8_t fifoBuffer[42];// FIFO storage fifoBuffer of mpu
int16_t ax, ay, az; //accel
int16_t accel[3] = {0, 0, 0};
int16_t gx, gy, gz; //gyro
int16_t mx, my, mz; //mag
//---------------------------------------------------------------------------
uint16_t fifoCount;     // count of all bytes currently in FIFO
int numbPackets;
bool is_alive = false;
bool running_coleta = false;
bool led_state = LOW;


//---------------------------------------------------------------------------
Timer t;
void takereading();
//---------------------------------------------------------------------------
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(saidaC, OUTPUT);  pinMode(saidaB, OUTPUT);  pinMode(saidaA, OUTPUT);
  Serial.begin(115200);

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  Wire.setClock(400000);
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  offsets = (int*)malloc(sizeof(int) * numSensors * 6);
  memcpy(offsets, offsets0, sizeof(int) * 6);
  memcpy(offsets + 6, offsets1, sizeof(int) * 6);
  memcpy(offsets + 12, offsets2, sizeof(int) * 6);
  memcpy(offsets + 18, offsets3, sizeof(int) * 6);
  memcpy(offsets + 24, offsets4, sizeof(int) * 6);
  for (int i = 0; i < numSensors; i++)
  {
    initializeSensor(i);
  }
  for (int i = 0; i < numSensors; i++)
  {
    verificaSensor(i);
  }
  Serial.println("Pronto para iniciar, aguardando comando serial.");
  digitalWrite(saidaA, LOW);
  digitalWrite(saidaB, LOW);
  digitalWrite(saidaC, LOW);
  //while (!Serial.available());

  t.every(10, takereading); //chama a cada 10ms = 1000/20

}
//---------------------------------------------------------------------------
void loop() {
  takereading();
}
//---------------------------------------------------------------------------
void takereading() {
  quat = readSensor(i);
  send_serial_packet(quat);

  Serial.println();
  digitalWrite(LED_PIN, led_state);
  led_state = !led_state;
}

void send_serial_packet(float* sensorQuaternion) {
  for (int i = 0; i < 4; i++)
  {
    Serial.print(sensorQuaternion[i]);
    Serial.print("\t");
  }
}

//---------------------------------------------------------------------------
float* readSensor(int sensorId)
{
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  //select_sensor(2);
  mag.getHeading(&mx, &my, &mz);

  //Compensate accelerometer readings with the calibration matrix
  //accel[0] = ax; accel[1] = ay; accel[2] = az;
  //compAcc(accel, cAcc);

  float fax = (float)(cAcc[0]) / 16384.0f;
  float fay = (float)(cAcc[1]) / 16384.0f;
  float faz = (float)(cAcc[2]) / 16384.0f;
  float fgx = (float)(gx) * (250.0f / 32768);
  float fgy = (float)(gy) * (250.0f / 32768);
  float fgz = (float)(gz) * (250.0f / 32768);


  mag.getHeadingWithOffset(&mx, &my, &mz, magOffsets);
  for (int i = 0; i < 5; i++){
    QuaternionUpdate(quat0, ax, ay, az, fgx * (PI / 180.0f), fgy * (PI / 180.0f), fgz * (PI / 180.0f), mx, my, mz, beta, 50.0);
  }
  
  return quat0;
}


//---------------------------------------------------------------------------
void initializeSensor()
{
  if (mpu.testConnection())
  {
    Serial.println("conn ok - Sensor: " + String(sensorId));
    //Serial.println("Birl - " + String(sensorId));
    mpu.initialize();
    mag.initialize();//will i use the mag???
    delay(50);
    mpu.setXAccelOffset(offsets[]);
    mpu.setYAccelOffset(offsets[1]);
    mpu.setZAccelOffset(offsets[2]);
    mpu.setXGyroOffset(offsets[3]);
    mpu.setYGyroOffset(offsets[4]);
    mpu.setZGyroOffset(offsets[5]);
    //necessary if using the calibrationa matrix
//    mpu.setXAccelOffset(0);
//    mpu.setYAccelOffset(0);
//    mpu.setZAccelOffset(0);
  }
}

//---------------------------------------------------------------------------
void compAcc(int16_t* acc, float* a)
{
  a[0] = acc[0]*calibMatrix[0] + acc[1]*calibMatrix[3] + acc[2]*calibMatrix[6] + calibMatrix[9];
  a[1] = acc[0]*calibMatrix[1] + acc[1]*calibMatrix[4] + acc[2]*calibMatrix[7] + calibMatrix[10];
  a[2] = acc[0]*calibMatrix[2] + acc[1]*calibMatrix[5] + acc[2]*calibMatrix[8] + calibMatrix[11];
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

