/* UNIVERSIDADE FEDERAL DE UBERLANDIA
   Biomedical Engineering
   Autors: Ítalo G S Fernandes
           Paulo Camargos
           Nathalia
   contact: italogsfernandes@gmail.com
   URLs: https://github.com/italogfernandes/SEB
  Este codigo faz parte da disciplina de sinais e sistemas
  para engenhara biomedica e visa realizar o controle de 
  um motor de passo.
  Nele existem 2 implementacoes, uma mais simples com delay
  e sem preocupacao com possiveis erros.
  E outra mais elaborada, onde as principais funcoes foram 
  encapsuladas como objetos e podem ser reutilizadas com 
  maior facilidade.
    Esquema de montagem:
    Arduino - Dispositivo
    8       -   Chave Liga Desliga
    9       -   Chave de Aumentar a Velocidade
    10      -   Chave de Diminuir a Velocidade
    11      -   Chave para Alterar Sentido de Rotacao
    7       -   Terminal A+ (Positivo da Bobina A) do Motor de Passo
    6       -   Terminal A- (Negativo da Bobina A) do Motor de Passo
    5       -   Terminal B+ (Positivo da Bobina B) do Motor de Passo
    3       -   Terminal B- (Negativo da Bobina B) do Motor de Passo
    Obs: As chaves nao sao usadas no codigo mais elaborado,
    este se comunica via comunicao Serial com Baudrate de 115200
    Menu:
       +: Mais Velocidade
       -: Menos Velocidade
       p: Parar
       r: Rodar Sentido Horário
       a: Rodar Sentido Anti-Horário
       s: Single-Step
       d: Double-Step
       h: Half-Step

   Para selecionar qual tipo de codigo voce deseja executar use:
   Para selecionar a versao com delay:
      #define VERSAO_COM_DELAY
      //#define VERSAO_SEM_DELAY
  Para selecionar a versao sem delay:
      //#define VERSAO_COM_DELAY
      #define VERSAO_SEM_DELAY
*/

//Descomente e comente qual versao você deseja executar:
//#define VERSAO_COM_DELAY
#define VERSAO_SEM_DELAY


/////////////////////////////////////
//IMPLEMENTACAO DA VERSAO COM DELAY//
/////////////////////////////////////
#ifdef VERSAO_COM_DELAY
// Definição dos pinos
#define pinLD 8
#define pinVelmais 9
#define pinVelmenos 10
#define pinSentido 11
#define pinAmais 7
#define pinAmenos 6
#define pinBmais 5
#define pinBmenos 3
int tempoespera = 20;

void setup() {
  pinMode(pinLD, INPUT_PULLUP);
  pinMode(pinVelmais, INPUT_PULLUP);
  pinMode(pinVelmenos, INPUT_PULLUP);
  pinMode(pinSentido, INPUT_PULLUP);
  pinMode(pinAmais, OUTPUT);
  pinMode(pinAmenos, OUTPUT);
  pinMode(pinBmais, OUTPUT);
  pinMode(pinBmenos, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(pinLD) == HIGH) {

    if (digitalRead(pinSentido)) {
      darpassodouble(tempoespera);
    }
    else {
      darpassodoubleanti(tempoespera);
    }

    if (digitalRead(pinVelmais) == LOW) {
      tempoespera = tempoespera - 1;
      if (tempoespera < 5) {
        tempoespera = 5; // velocidade maxima
      }
      Serial.print("Novo Tempo = ");
      Serial.print(tempoespera);
      Serial.println(" ms");
    }
    if (digitalRead(pinVelmenos) == LOW) {
      tempoespera = tempoespera + 1;
      Serial.print("Novo Tempo = ");
      Serial.print(tempoespera);
      Serial.println(" ms");
    }
  }
}
void darpassosingle(int tempo) {
  digitalWrite(pinAmais, 1);
  digitalWrite(pinAmenos, 0);
  digitalWrite(pinBmais, 0);
  digitalWrite(pinBmenos, 0);
  Serial.println(" Passo 1");
  delay(tempo);
  digitalWrite(pinAmais, 0);
  digitalWrite(pinAmenos, 0);
  digitalWrite(pinBmais, 0);
  digitalWrite(pinBmenos, 1);
  //Serial.println(" Passo 2");
  delay(tempo);
  digitalWrite(pinAmais, 0);
  digitalWrite(pinAmenos, 1);
  digitalWrite(pinBmais, 0);
  digitalWrite(pinBmenos, 0);
  Serial.println(" Passo 3");
  delay(tempo);
  digitalWrite(pinAmais, 0);
  digitalWrite(pinAmenos, 0);
  digitalWrite(pinBmais, 1);
  digitalWrite(pinBmenos, 0);
  Serial.println(" Passo 4");
  delay(tempo);
}

void darpassodouble(int tempo) {
  //Ta bugada essa
  digitalWrite(pinAmais, 1);
  digitalWrite(pinAmenos, 0);
  digitalWrite(pinBmais, 0);
  digitalWrite(pinBmenos, 1);
  // Serial.println(" Passo 1");
  delay(tempo);
  digitalWrite(pinAmais, 0);
  digitalWrite(pinAmenos, 1);
  digitalWrite(pinBmais, 0);
  digitalWrite(pinBmenos, 1);
  //  Serial.println(" Passo 2");
  delay(tempo);
  digitalWrite(pinAmais, 0);
  digitalWrite(pinAmenos, 1);
  digitalWrite(pinBmais, 1);
  digitalWrite(pinBmenos, 0);
  // Serial.println(" Passo 3");
  delay(tempo);
  digitalWrite(pinAmais, 1);
  digitalWrite(pinAmenos, 0);
  digitalWrite(pinBmais, 1);
  digitalWrite(pinBmenos, 0);
  //Serial.println(" Passo 4");
  delay(tempo);
}
void darpassodoubleanti(int tempo) {
  //Ta bugada essa
  digitalWrite(pinAmais, 1);
  digitalWrite(pinAmenos, 0);
  digitalWrite(pinBmais, 0);
  digitalWrite(pinBmenos, 1);
  Serial.println(" Passo 1");
  delay(tempo);
  digitalWrite(pinAmais, 1);
  digitalWrite(pinAmenos, 0);
  digitalWrite(pinBmais, 1);
  digitalWrite(pinBmenos, 0);
  Serial.println(" Passo 2");
  delay(tempo);
  digitalWrite(pinAmais, 0);
  digitalWrite(pinAmenos, 1);
  digitalWrite(pinBmais, 1);
  digitalWrite(pinBmenos, 0);
  Serial.println(" Passo 3");
  delay(tempo);
  digitalWrite(pinAmais, 0);
  digitalWrite(pinAmenos, 1);
  digitalWrite(pinBmais, 0);
  digitalWrite(pinBmenos, 1);
  Serial.println(" Passo 4");
  delay(tempo);
}
#endif /*VERSAO_COM_DELAY*/

/////////////////////////////////////
//IMPLEMENTACAO DA VERSAO SEM DELAY//
/////////////////////////////////////
#ifdef VERSAO_SEM_DELAY

////////////
//Defines //
////////////
#define UART_BAUDRATE 115200

///////////
//Clases //
///////////
/////////////////////////////////////////////////
// Implementacao de um timer atraves do millis //
/////////////////////////////////////////////////

class Timer {
  private:
    unsigned long _actual_time;
    unsigned long _waited_time;
    bool _running;
    uint32_t _interval;

  public:
    bool elapsed;

    Timer(uint32_t interval = 1000) {
      _interval = interval;
      elapsed = false;
      _actual_time = micros();
      _waited_time = _actual_time + interval;
    }

    void setInterval(uint32_t interval) {
      _interval = interval;
    }

    uint32_t getInterval() {
      return _interval;
    }

    void start() {
      _running = true;
      _actual_time = millis();
      _waited_time = _actual_time + _interval;
    }

    void stop() {
      _running = false;
    }

    void wait_next() {
      elapsed = false;
    }

    void update() {
      _actual_time = micros();
      if (_running) {
        if (_actual_time >= _waited_time) {
          _waited_time = _actual_time + _interval;
          elapsed = true;
        }
      }
    }

};



///////////////////////////////////////////
//Classe para controle do motor de passo //
///////////////////////////////////////////

//Necessarios para a Classe Stepper:
typedef enum {
  SINGLE_STEP,
  DOUBLE_STEP,
  HALF_STEP
} acionamento_t;
//Sequencias de passos
const uint8_t single_step_seq[4] = {8, 4, 2, 1};
const uint8_t double_step_seq[4] = {12, 6, 3, 9};
const uint8_t half_step_seq[8] = {8, 12, 4, 6, 2, 3, 1, 9};

class Stepper {
  private:
    Timer _control_timer;
    uint8_t _pin_phase_A[2]; //Pinos da Fase A [Positivo] [Negativo]
    uint8_t _pin_phase_B[2]; //Pinos da Fase B [Positivo] [Negativo]
    acionamento_t _tipo_de_passo;
    uint8_t _passo_atual;
    bool _clockwise;

    void alinharBobinas(uint8_t decimal) {
      digitalWrite(_pin_phase_A[0], (decimal >> 3) & 0x01); //Bit 3
      digitalWrite(_pin_phase_B[0], (decimal >> 2) & 0x01); //Bit 2
      digitalWrite(_pin_phase_A[1], (decimal >> 1) & 0x01); //Bit 1
      digitalWrite(_pin_phase_B[1], (decimal >> 0) & 0x01); //Bit 0
    }

    void doSingleStep() {
      alinharBobinas(single_step_seq[_passo_atual / 2]); //Utiliza apenas os indices e 0,2,4,6 para dar o passo
    }

    void doDoubleStep() {
      alinharBobinas(double_step_seq[_passo_atual / 2]);
    }

    void doHalfStep() {
      alinharBobinas(half_step_seq[_passo_atual]);
    }

  public:
    /**
       [Stepper description]
       @param pinAPositive  [description]
       @param pinANegative  [description]
       @param pinBPositive  [description]
       @param pinBNegative  [description]
       @param tipo_de_passo [description]
    */
    Stepper(uint8_t pinAPositive, uint8_t pinANegative,
            uint8_t pinBPositive, uint8_t pinBNegative,
            acionamento_t tipo_de_passo = SINGLE_STEP) {
      _pin_phase_A[0] = pinAPositive;
      _pin_phase_A[1] = pinANegative;
      _pin_phase_B[0] = pinBPositive;
      _pin_phase_B[1] = pinBNegative;
      _tipo_de_passo = tipo_de_passo;
      _passo_atual = 0;
      _control_timer.setInterval(20000);
      _clockwise = true;
    }

    void begin() {
      pinMode(_pin_phase_A[0], OUTPUT);
      pinMode(_pin_phase_B[0], OUTPUT);
      pinMode(_pin_phase_A[1], OUTPUT);
      pinMode(_pin_phase_B[1], OUTPUT);
    }

    void setAcionamento(acionamento_t tipo_de_passo) {
      _tipo_de_passo = tipo_de_passo;
    }

    void setStepInterval(uint32_t interval) {
      _control_timer.setInterval(interval);
    }

    uint32_t getStepInterval() {
      return _control_timer.getInterval();
    }

    void setFreq(uint32_t frequency) {
      setStepInterval(1000000 / frequency);
    }

    uint32_t getFreq() {
      return 1000000 / getStepInterval();
    }

    void doStep() {
      switch (_tipo_de_passo) {
        case SINGLE_STEP:
          doSingleStep();
          break;
        case DOUBLE_STEP:
          doDoubleStep();
          break;
        case HALF_STEP:
          doHalfStep();
          break;
      }
      if (_clockwise) {
        ++_passo_atual = _passo_atual % 8; //Incremento circular
      } else { //Decemento circular
        if (_passo_atual == 0) {
          _passo_atual = 7;
        } else {
          _passo_atual = _passo_atual - 1;
        }

      }
    }

    void rotateClockwise() {
      _clockwise = true;
      _control_timer.start();
    }

    void rotateAntiClockwise() {
      _clockwise = false;
      _control_timer.start();
    }

    void stop() {
      _control_timer.stop();
    }

    void update() {
      _control_timer.update();
      if (_control_timer.elapsed) {
        doStep();
        _control_timer.wait_next();
      }
    }
};


//////////////////////
//Variaveis globais //
//////////////////////
Stepper motor(7, 6, 5, 3); ///A+, A-, B+ , B-
bool status_led = false;
char serialOp;
//////////////////
//Main Function //
//////////////////
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  motor.begin();
  Serial.begin(UART_BAUDRATE);
  Serial.println("Hora do show porra!");
  Serial.println("Envie um comando:");
  Serial.println("+: Mais Velocidade");
  Serial.println("-: Menos Velocidade");
  Serial.println("p: Parar");
  Serial.println("r: Rodar Sentido Horário");
  Serial.println("a: Rodar Sentido Anti-Horário");
  Serial.println("s: Single-Step");
  Serial.println("d: Double-Step");
  Serial.println("h: Half-Step");
  motor.setFreq(100);
  //Biiirl
  motor.rotateClockwise();
}

void loop() {
  motor.update();
  if (Serial.available()) {
    serialOp = Serial.read();
    switch (serialOp) {
      case '+':
        motor.setFreq(motor.getFreq() + 1);
        Serial.println("Step Freq: " + String(motor.getFreq()) + " Hz");
        Serial.println("Step Interval: " + String(motor.getStepInterval()) + " us");
        break;
      case '1':
        motor.setFreq(motor.getFreq() + 10);
        Serial.println("Step Freq: " + String(motor.getFreq()) + " Hz");
        Serial.println("Step Interval: " + String(motor.getStepInterval()) + " us");
        break;
      case '2':
        motor.setFreq(motor.getFreq() - 10);
        Serial.println("Step Freq: " + String(motor.getFreq()) + " Hz");
        Serial.println("Step Interval: " + String(motor.getStepInterval()) + " us");
        break;
      case '3':
        motor.setFreq(motor.getFreq() + 100);
        Serial.println("Step Freq: " + String(motor.getFreq()) + " Hz");
        Serial.println("Step Interval: " + String(motor.getStepInterval()) + " us");
        break;
      case '4':
        motor.setFreq(motor.getFreq() - 100);
        Serial.println("Step Freq: " + String(motor.getFreq()) + " Hz");
        Serial.println("Step Interval: " + String(motor.getStepInterval()) + " us");
        break;
      case '-':
        motor.setFreq(motor.getFreq() - 1);
        Serial.println("Step Freq: " + String(motor.getFreq()) + " Hz");
        Serial.println("Step Interval: " + String(motor.getStepInterval()) + " us");
        break;

      case 'p':
        motor.stop();
        Serial.println("Stopped");
        break;
      case 'r':
        motor.rotateClockwise();
        Serial.println("Started Clockwise");
        break;
      case 'a':
        motor.rotateAntiClockwise();
        Serial.println("Started AntiClockwise");
        break;
      case 's':
        motor.setAcionamento(SINGLE_STEP);
        Serial.println("Mode: Single-Step");
        break;
      case 'd':
        motor.setAcionamento(DOUBLE_STEP);
        Serial.println("Mode: Double-Step");
        break;
      case 'h':
        motor.setAcionamento(HALF_STEP);
        Serial.println("Mode: Half-Step");
        break;
      default:
        break;
    }
  }
}
/////////////////////////
//End of Main Function //
/////////////////////////
#endif /*VERSAO_SEM_DELAY*/
