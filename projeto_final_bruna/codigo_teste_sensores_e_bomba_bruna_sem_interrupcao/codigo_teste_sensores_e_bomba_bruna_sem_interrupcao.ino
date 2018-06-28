#define PIN_SENSOR_AGUA 3
#define PIN_SENSOR_PROD 5
#define PIN_BOMBA_AGUA 11
#define PIN_BOMBA_PROD 13
#define PIN_ONOFF_BOMBA_AGUA 7
#define PIN_ONOFF_BOMBA_PROD 8

unsigned long count_pulsos_agua;
unsigned long count_pulsos_prod;

unsigned long last_millis;
unsigned long current_millis;

bool pin_sensor_agua_last_reading;
bool pin_sensor_prod_last_reading;
bool pin_sensor_agua_current_reading;
bool pin_sensor_prod_current_reading;

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
  pinMode(PIN_SENSOR_AGUA, INPUT);
  pinMode(PIN_SENSOR_PROD, INPUT);
  status_agua = HIGH; status_prod = LOW;
}

void loop() {
  // Lendo sensr agua sem usar interrupt:
  pin_sensor_agua_current_reading = digitalRead(PIN_SENSOR_AGUA);
  if ( !pin_sensor_agua_last_reading && pin_sensor_agua_current_reading) { // Detectada borda de subida
    interrupt_sensor_agua();
  }
  pin_sensor_agua_last_reading = pin_sensor_agua_current_reading;

  // Lendo sensor prod sem usar interrupt
  pin_sensor_prod_current_reading = digitalRead(PIN_SENSOR_PROD);
  if ( !pin_sensor_prod_last_reading && pin_sensor_prod_current_reading) { // Detectada borda de subida
    interrupt_sensor_prod();
  }
  pin_sensor_prod_last_reading = pin_sensor_prod_current_reading;

  // Verificando se deve ligar as bombas
  status_agua = !digitalRead(PIN_ONOFF_BOMBA_AGUA);
  status_prod = !digitalRead(PIN_ONOFF_BOMBA_PROD);
  digitalWrite(PIN_BOMBA_AGUA, status_agua); digitalWrite(PIN_BOMBA_PROD, status_prod);

  current_millis = millis();
  if (current_millis - last_millis > 100) {
    last_millis = current_millis;
    //Mostrando
    Serial.print("Água: "); Serial.print(1000 * count_pulsos_agua / 330.00); Serial.print(" ml.\t");
    Serial.print("Prod: "); Serial.print(1000 * count_pulsos_prod / 330.00); Serial.print(" ml.\t");
    Serial.print("Bomba Água: "); Serial.print(int(status_agua)); Serial.print("\t");
    Serial.print("Bomba Prod: "); Serial.print(int(status_prod)); Serial.print("\n");
  }
}
