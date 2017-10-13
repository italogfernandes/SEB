////////////////
//Bibliotecas //
////////////////
#include "Motor.h" // Import a library Motor.h, que deve estar na mesma pasta deste arquivo .ino

////////////
//Defines //
////////////
#define DANCE_BUTTON 8    // Ignorar. Apenas funcao extra
#define LD_BUTTON 7       // Botão liga-desliga do sistema
#define FR_BUTTON 6       // Botão frei magnético do sistema
#define POT_V A0          // Pino de leitura da tensão no potenciometro

////////////////////////////////
//Variaveis e objetos globais //
////////////////////////////////

// Cria um objeto 'motor' da classe 'Motor' e inicializacao dos pinos Motor(IN1, IN2, ENABLE);
Motor motor(13, 12, 10); // Por que era um ponteiro?
//Variaveis para debouncing do LD_BUTTON
bool chButtonState, lastChButtonState = 0; //Ae sim
unsigned long lastDeboucingTime = 0, debounceDelay = 50;
bool estadoSistema = 1;

////////////////
//Main Sketch //
////////////////
/**
 * Iniciliza os pinos como saidas e entradas e tambem a comunicação serial
 */
void setup() {
  /* Informações da classe motor*/
  pinMode(motor.getIN1(), OUTPUT);     // Configuracao do pino IN1 como OUTPUT
  pinMode(motor.getIN2(), OUTPUT);     // Configuracao do pino IN2 como OUTPUT
  pinMode(motor.getEnable(), OUTPUT);  // Configuracao do pino ENABLE como OUTPUT

  //Variáveis internas do programa (botoes)
  pinMode(DANCE_BUTTON, INPUT_PULLUP);  // Configuracao do pino DANCE_BUTTON como INPUT_PULLUP
  pinMode(LD_BUTTON, INPUT_PULLUP);     // Configuracao do pino LD_BUTTON como INPUT_PULLUP
  pinMode(FR_BUTTON, INPUT_PULLUP);     // Configuracao do pino FR_BUTTON como INPUT_PULLUP

  Serial.begin(9600);                   // Inicializacao da porta serial
  Serial.println(" -------- PROGRAMA EM FUNCIONAMENTO -------- ");
}

/**
 * Se repete a cada ciclio, cuidando de executar a tarefa desejada
 * - Leitura da tensao em A0
 * - Controle do motor de acordo com a Leitura em A0
 * - Controle do motor de acordo com os botões
 */
void loop() {
  float f_tensao = calculaTensao();       // Chamada da funcao para calculo da tensao no potenc.
  //Ativa o freio mag. se o motor estiver funcionando e a chave FR_BUTTON pressionada
  if (!digitalRead(FR_BUTTON) && ligaDesliga()) {
    motor.stopMotor(f_tensao);          // Metodo do objeto 'motor' para freio magnetico
  } else      // Operador ternario
  // Se o sistema estiver ligado, faz o motor funcionar. Caso contrario, desliga motor.
  ligaDesliga() ? motor.rodaMotor(f_tensao) : motor.desligaMotor();
}

//////////////////////
//Metodos Auxilires //
/////////////////////

/**
 * [calculaTensao description]
 * @return [description]
 */
float calculaTensao() {  // funcao para calculo da tensao no potenc.
  return map(analogRead(POT_V), 0, 1023, 0, 330) / 100.0;
}
/**
 * [ligaDesliga description]
 * @return [description]
 */
bool ligaDesliga() { // Debounce do botao liga desliga. Retorna 1 para ligado, 0 para desligado.
  // Ver referencia em https://www.arduino.cc/en/Tutorial/Debounce para entendimento da funcao
  int chReading = !digitalRead(LD_BUTTON);
  if (chReading != lastChButtonState) {
    lastDeboucingTime = millis();
  }

  if ((millis() - lastDeboucingTime) > debounceDelay) {
    if (chReading != chButtonState) {
      chButtonState = chReading;
      if (chButtonState) {
        estadoSistema = !estadoSistema;
      }
    }
  }
  lastChButtonState = chReading;
  return estadoSistema;
}
