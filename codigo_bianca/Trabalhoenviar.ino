////////////
//Defines //
////////////
#define PINO_CHAVE      10 // Dê preferencia para defines todas em maiusculos
#define PINO_ANALOGICO  A0 // variaveis em minusculo, e funções a primeria letra em maiusculo
#define PINO_LED1       30 // Os pinos dos leds estavam como variaveis
#define PINO_LED2       35 // prefira colocar pinos como Defines
#define PINO_LED3       40 // Seu código tbm vai funcionar de outro jeito
#define PINO_LED4       45 // Mas é bom seguir padrões

//////////////////////
//Variaveis globais //
//////////////////////
int tempo_espera; // Essa variável tava declarada num lugar esquisito
//unsigned long por que ta no site: https://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
unsigned long tempo_atual, tempo_desejado; // Variaveis para o millis
int passo_atual; // indicará qual etapa dos leds o arduino esta executando

////////////
//Funções //
////////////
void setup(){
    passo_atual = 1; // O primeiro passo sera o passo I
    // Pinos de entrada e saida
    pinMode(PINO_CHAVE, INPUT_PULLUP);
    pinMode(PINO_LED1, OUTPUT);
    pinMode(PINO_LED2, OUTPUT);
    pinMode(PINO_LED3, OUTPUT);
    pinMode(PINO_LED4, OUTPUT);
}

void loop() {
    //lê a tensão e ja converte ela pro tempo (de 50ms ate 500ms)
    tempo_espera = map(analogRead(PINO_ANALOGICO),0,1023,50,500);

    tempo_atual = millis(); // Nesse código tem que usar millis, com delay fica mto ruim
    if(tempo_atual >= tempo_desejado){ // Se deu o tempo para fazer algo
        tempo_desejado = tempo_atual + tempo_espera; // atualiza qual sera o proximo tempo

        if(!digitalRead(PINO_CHAVE)){ // Verifica se a chave esta ativada
            // Quando seu processo tiver varios passos
            // divida eles em um switch dessa forma
            // Isso é o principio do conceito de programar uma maquina de estados
            // Se quiser dar um nome bonito pra essa coisa
            switch (passo_atual) {
                case 1: // Executando o passo I
                digitalWrite(PINO_LED1, LOW);
                digitalWrite(PINO_LED2, LOW);
                digitalWrite(PINO_LED3, LOW);
                digitalWrite(PINO_LED4, LOW);
                passo_atual = 2; // Vai pro passo II
                break;
                case 2: // Executando o passo II
                digitalWrite(PINO_LED1, HIGH);
                digitalWrite(PINO_LED2, LOW);
                digitalWrite(PINO_LED3, LOW);
                digitalWrite(PINO_LED4, LOW);
                passo_atual = 3; // Vai pro passo III
                break;
                case 3: // Executando o passo III
                digitalWrite(PINO_LED1, HIGH);
                digitalWrite(PINO_LED2, HIGH);
                digitalWrite(PINO_LED3, LOW);
                digitalWrite(PINO_LED4, LOW);
                passo_atual = 4; // Vai pro passo IV
                break;
                case 4: // Executando o passo IV
                digitalWrite(PINO_LED1, HIGH);
                digitalWrite(PINO_LED2, HIGH);
                digitalWrite(PINO_LED3, HIGH);
                digitalWrite(PINO_LED4, LOW);
                passo_atual = 5; // Vai pro passo V
                break;
                case 5: // Executando o passo V
                digitalWrite(PINO_LED1, HIGH);
                digitalWrite(PINO_LED2, HIGH);
                digitalWrite(PINO_LED3, HIGH);
                digitalWrite(PINO_LED4, HIGH);
                passo_atual = 1; // Volta ao inicio
                break;
            }
        }
    }
}
