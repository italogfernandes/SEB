#define LUZ         6
#define SENSOR      A0
#define REFERENCIA  A1

int referencia;
int valor_atual;
int erro;

float dt;

float kp = 5;
float ki = 1;

float P, I;
float SA;

unsigned long time_ultima_vez;

void setup() {
  Serial.begin(115200);
  pinMode(LUZ, OUTPUT);
}

void loop() {

  referencia = 0;
  valor_atual = 0;
  //Leitura dos sensores com media movel
  for (int i = 0; i < 50; i++) {
    referencia += analogRead(REFERENCIA) / 50.0f;
    valor_atual += analogRead(SENSOR) / 50.0f;
  }

  //Calculo do erro
  erro = referencia - valor_atual;

  //Calculo do dt para a integral
  dt = millis() - time_ultima_vez;
  dt = dt / 1000.0;//Converte para segundos


  //Calculo do PI
  P = erro * kp;
  I = ki * erro * dt + I;
  time_ultima_vez = millis();

  //Calcula o Sinal Atuante
  SA = P + I;
  if (SA > 255) {
    SA = 255;
  }
  if (SA < 0) {
    SA = 0;
  }

  //Joga na saida
  analogWrite(LUZ,SA);

  //Mostra as coisas na serial
  Serial.print("\tLeitura: ");
  Serial.print(valor_atual);

  Serial.print("\tReferencia: ");
  Serial.print(referencia);

  Serial.print("\tSA: ");
  Serial.print(SA);
  
  Serial.println();
  delay(50);
}
