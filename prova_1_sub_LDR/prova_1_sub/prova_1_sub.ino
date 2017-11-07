#define pinADC A4 //Pino do conversor AD
#define DARK_THRESHOLD 10 //Menor leitura obtida para o escuro
#define LIGHT_THRESHOLD 980 //Maior leitura obtida para o claro
uint16_t leitura_ad; //armazena a leitura do conversor AD de 0 a 1024
double brightness_percent; //amazenara o valor do brilho em porcentagem
//Sendo DARK_THRESHOLD como 0% e LIGHT_THRESHOULD como 100%
const uint8_t pinled[] = {2, 3, 4, 5}; //Pinos dos leds
void setup() {
  Serial.begin(115200); //Inicia a comunicacao serial a 115200 de baudrate
  for (int i = 0; i < 4; i++) {
    pinMode(pinled[i], OUTPUT); // inicia todos os pinos dos leds como output
  }
}

void loop() {
  leitura_ad = analogRead(pinADC); //Lendo o Conversor AD no pino definido
  //Regra de 3 para conversao de escala, Subtrai o offset e divide pelo tamanho total, Multiplica por 100 para dar em porcentagem
  brightness_percent = (double)(leitura_ad - DARK_THRESHOLD) * 100.0 / (double) (LIGHT_THRESHOLD - DARK_THRESHOLD); //Convertendo para Porcentagem de Iluminação
  for (int i = 0; i < 4; i++) { //Acendendo os leds proporcionalmente
    //Para cada led (de 0 a 4) Coloca ele de acordo com a expressao : porcentagem > 20 * (i+1)
    //ou seja 20 * 1 = 20... 20 * 2 = 40 .... ate 20 * 4 = 80
    // se a expressao retornar True (1, HIGH) acende, se a expressao retornar False (0, LOW) apaga
    digitalWrite(pinled[i], brightness_percent >= (20 * (i + 1)));
  }
  //Mostra o valor de acordo com o protocolo pedido
  //Funciona com o SerialPlotter
  Serial.println(); //Linha em branco
  Serial.println("-----------------------------");
  Serial.println("Brilho Atual = " + String(brightness_percent, 0) + " %");
  Serial.println("-----------------------------");
  Serial.println(); //Linha em Branco
}
