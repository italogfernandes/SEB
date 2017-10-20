#define USING_ARDUINO_DUE
//#define USING_ARUDINO_UNO

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
      _actual_time = millis();
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
      _actual_time = millis();
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
      _control_timer.setInterval(20);
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
      ++_passo_atual = _passo_atual % 8; //Incremento circular
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
Stepper motor(8, 9, 10, 11); ///A+, A-, B+ , B-
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
  Serial.println("s: Single-Step");
  Serial.println("d: Double-Step");
  Serial.println("h: Half-Step");
  //Biiirl
  motor.rotateClockwise();
}

void loop() {
  motor.update();
  if (Serial.available()) {
    serialOp = Serial.read();
    switch (serialOp) {
      case '+':
        motor.setStepInterval(motor.getStepInterval() - 1);
        Serial.println("Step Interval: " + String(motor.getStepInterval()));
        break;
      case '-':
        motor.setStepInterval(motor.getStepInterval() + 1);
        Serial.println("Step Interval: " + String(motor.getStepInterval()));
        break;
      case 'p':
        motor.stop();
        Serial.println("Stopped");
        break;
      case 'r':
        motor.rotateClockwise();
        Serial.println("Started");
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
