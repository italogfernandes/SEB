#define pinADC A4
#define DARK_THRESHOLD 10
#define LIGHT_THRESHOLD  980

uint16_t leitura_ad;
double brightness_percent;
const uint8_t pinled[] = {2, 3, 4, 5};

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 4; i++) {
    pinMode(pinled[i], OUTPUT);
  }
}

void loop() {
  doShowMessage();
  leitura_ad = analogRead(pinADC);
  brightness_percent = (double)(leitura_ad - DARK_THRESHOLD) * 100.0 / (double) (LIGHT_THRESHOLD - DARK_THRESHOLD);
  for (int i = 0; i < 4; i++) {
    digitalWrite(pinled[i], brightness_percent >= (20 * (i + 1)));
  }
}

void doShowMessage() {
  Serial.println(); //Linha em branco
  Serial.println("-----------------------------");
  Serial.println("Brilho Atual = " + String(brightness_percent, 2) + " %");
  Serial.println("-----------------------------");
  Serial.println(); //Linha em Branco
}

