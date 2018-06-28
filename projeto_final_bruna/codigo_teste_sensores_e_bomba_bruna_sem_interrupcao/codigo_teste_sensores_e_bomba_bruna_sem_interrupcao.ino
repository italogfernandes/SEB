#define PIN_SENSOR_AGUA 3
#define PIN_SENSOR_PROD 5
#define PIN_BOMBA_AGUA 11
#define PIN_BOMBA_PROD 13
#define PIN_ONOFF_BOMBA_AGUA 7
#define PIN_ONOFF_BOMBA_PROD 8

volatile unsigned long count_pulsos_agua;
volatile unsigned long count_pulsos_prod;

bool status_agua;
bool status_prod;

void interrupt_sensor_agua() {
  count_pulsos_agua++;
}

void interrupt_sensor_prod() {
  count_pulsos_prod++;
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_BOMBA_AGUA, OUTPUT);
  pinMode(PIN_BOMBA_PROD, OUTPUT);
  pinMode(PIN_ONOFF_BOMBA_AGUA, INPUT_PULLUP);
  pinMode(PIN_ONOFF_BOMBA_PROD, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_AGUA), interrupt_sensor_agua, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_PROD), interrupt_sensor_prod, RISING);
  status_agua = HIGH; status_prod = LOW;
}

void loop() {
  status_agua = !digitalRead(PIN_ONOFF_BOMBA_AGUA);
  status_prod = !digitalRead(PIN_ONOFF_BOMBA_PROD);
  Serial.print("Água: "); Serial.print(1000 * count_pulsos_agua / 330.00); Serial.print(" ml.\t");
  Serial.print("Prod: "); Serial.print(1000 * count_pulsos_prod / 330.00); Serial.print(" ml.\t");
  Serial.print("Bomba Água: "); Serial.print(int(status_agua)); Serial.print("\t");
  Serial.print("Bomba Prod: "); Serial.print(int(status_prod)); Serial.print("\n");
  digitalWrite(PIN_BOMBA_AGUA, status_agua); digitalWrite(PIN_BOMBA_PROD, status_prod);
  delay(100);
}
