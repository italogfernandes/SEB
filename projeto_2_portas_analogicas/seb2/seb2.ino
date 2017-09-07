//Pinos
#define POT_PIN             A0
#define LED1                2
#define LED2                3
#define LED3                4
#define STATUS_LED          13
//Comunicacao Serial
#define UART_BAUDRATE       115200
#define UART_PACKET_START   '$'
#define UART_PACKET_END     '\n'
#define USB_CMD_START       'I'
#define USB_CMD_STOP        'P'
#define USB_CMD_READ        'R'
#define USB_CMD_ERROR       'E'
//Conversor AD, Application functions
#define AD_CONVERSION_LSB   4096
#define AD_SCALE_BACK       3.3
#define LIMIAR1             1024
#define LIMIAR2             1024
#define LIMIAR3             1024
#define SAMPLE_FREQ         50
#define SAMPLE_PERIOD       1000000/SAMPLE_FREQ //Sample period in microseconds

//Variaveis
unsigned long previousMicros;
bool aquisition_running = false;
uint16_t valor_lido;
char comando_usb = '0';

void setup() {
  pinMode(LED1, OUTPUT); pinMode(LED2, OUTPUT); pinMode(LED3, OUTPUT); //Pinos
  pinMode(STATUS_LED, OUTPUT);
  Serial.begin(UART_BAUDRATE);  //Serial para debug
  //SerialUSB.begin(UART_BAUDRATE); //Serial USB para pacote final
  analogReadResolution(12);//12 Bits
  aquisition_running = true;
}
void loop() {
  if(aquisition_running){
    if(micros() - previousMicros >= SAMPLE_PERIOD){
      previousMicros = micros();
      timerDataAcq();
    }
    
  }
  if (Serial.available()) {
    comando_usb = Serial.read();
    avaliar_comando();
  }
  if (SerialUSB.available()) {
    comando_usb = SerialUSB.read();
    avaliar_comando();
  }
}

void avaliar_comando() {

  switch (comando_usb) {
    case USB_CMD_START:
      aquisition_running = true;
      break;
    case USB_CMD_STOP:
      aquisition_running = false;
      break;
    case USB_CMD_READ:
      timerDataAcq();
      break;
    default:
      Serial.write(USB_CMD_ERROR);
      SerialUSB.write(USB_CMD_ERROR);
      break;
  }
}
void timerDataAcq() {
  //Reading
  valor_lido = analogRead(POT_PIN);
  //Sending
  Serial.println((float) valor_lido * AD_SCALE_BACK / (AD_CONVERSION_LSB - 1), 2); //Covnertendo e plotando
  SerialUSB.write(UART_PACKET_START);
  SerialUSB.write((uint8_t) (valor_lido >> 8));
  SerialUSB.write((uint8_t) (valor_lido & 0xFF));
  SerialUSB.write(UART_PACKET_END);
  //Update LEDS
  analogWrite(LED1, map(valor_lido,       0, LIMIAR1, 0, 255));
  analogWrite(LED1, map(valor_lido, LIMIAR1, LIMIAR2, 0, 255));
  analogWrite(LED1, map(valor_lido, LIMIAR2, LIMIAR3, 0, 255));
}

