# -*- coding: utf-8 -*-
#------------------------------------------------------------------------------
# FEDERAL UNIVERSITY OF UBERLANDIA
# Faculty of Electrical Engineering
# Biomedical Engineering Lab
#------------------------------------------------------------------------------
# Author: Italo Gustavo Sampaio Fernandes
# Contact: italogsfernandes@gmail.com
# Git: www.github.com/italogfernandes
#------------------------------------------------------------------------------
# Decription:
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
#Libraries
try:
    import RPi.GPIO as GPIO
except RuntimeError:
    print("Error importing RPi.GPIO!  This is probably because you need superuser privileges.  You can achieve this by using 'sudo' to run your script")

#------------------------------------------------------------------------------
#Controla um motor dc
class MotorHandler():
	def __init__(self, _pinEnable=13, _pinPositive=5, _pinNegative=6,_velocidade=50):
        self.pinEnable = _pinEnable
        self.pinos = (_pinPositive,_pinNegative)
        self.pwmVelocidade = None
        self.initialize_pins()
        self.velocidade = _velocidade # Velocidade em porcentagem de 0 a 100

	def initialize_pins(self):
        GPIO.setup(self.pinos, GPIO.OUT, initial=0) #Inicia os pinos da ponte H
        GPIO.setup(self.pinEnable, GPIO.OUT) #Inicia o pino de velocidade e em seguido seu pwm
        self.pwmVelocidade = GPIO.PWM(self.pinEnable, 500)  # channel=self.enable frequency=500Hz
        self.pwmVelocidade.start(0) #Inicialmente = 0

    def girarHorario(self,_velocidade=self.velocidade):
        GPIO.output(self.pinos , (1,0))  # first LOW, second HIGH
        self.velocidade = _velocidade
        self.pwmVelocidade.ChangeDutyCycle(self.velocidade)

    def girarAntiHorario(self,_velocidade=self.velocidade):
        GPIO.output(self.pinos , (0,1))  # first HIGH, second LOW
        self.velocidade = _velocidade
        self.pwmVelocidade.ChangeDutyCycle(self.velocidade)

    def setVelocidade(self,_velocidade):
        self.velocidade = _velocidade
        self.pwmVelocidade.ChangeDutyCycle(self.velocidade)

    def girar(self, _direcao=1, _velocidade=self.velocidade):
        if _direcao > 0:
            self.girarHorario(_velocidade)
        elif _direcao < 0:
            self.girarAntiHorario(self,_velocidade):
        else:
            self.stop()

    def stop(self,_magnetico=True):
        self.pwmVelocidadep.ChangeDutyCycle(0)
        if _magnetico:
            GPIO.output(self.pinos , (0,0))  # first LOW, second LOW

    def clear_io(self):
        self.pwmVelocidade.stop()
        GPIO.cleanup(self.pinos)
        GPIO.cleanup(self.pinEnable)

#------------------------------------------------------------------------------
#Controla um Motor de Passo
#class StepperHandler():

#------------------------------------------------------------------------------
#Controla um Carrinho com 2 motores em cada lado
class CarHandler():
    def __init__(self, _pinsEnable=[13,12], _pinsInputs=[5,6,23,24],_velocidade=50):
        self.velocidade = _velocidade # Velocidade em porcentagem de 0 a 100
        self.motorA = MotorHandler(_pinsEnable[0], _pinsInputs[0], _pinsInputs[1],self.velocidade)
        self.motorB = MotorHandler(_pinsEnable[1], _pinsInputs[2], _pinsInputs[3],self.velocidade)
        self.motores = [self.motorA,self.motorB]

    def setVelocidade(self,_velocidade):
        self.velocidade = _velocidade
        for motor in motores:
            motor.setVelocidade(self.velocidade)

    def frente(self):
        self.motorA.girar(1)
        self.motorB.girar(-1)

    def re(self):
        self.motorA.girar(-1)
        self.motorB.girar(1)

    def direita(self,_brusca=False):
        if _brusca:
            self.motorA.girar(1)
            self.motorB.girar(1)
        else:
            self.motorA.girar(1)
            self.motorB.girar(0)

    def esquerda(self,_brusca=False):
        if _brusca:
            self.motorA.girar(-1)
            self.motorB.girar(-1)
        else:
            self.motorA.girar(0)
            self.motorB.girar(-1)

    def stop(self):
        for motor in motores:
            motor.stop()

    def clear_io(self):
        for motor in motores:
            motor.clear_io()

#------------------------------------------------------------------------------
#Tests
if __name__ == "__main__":
    GPIO.setmode(self.boardMode) #seta a board para BCM ou BOARD
	meu_motor = MotorHandler(13,5,6,100)
	while(True):
		print('-------------------------------')
		print('Controlling a DC Motor')
		print('-------------------------------')
		print('Menu')
		print('1 - Girar Horario')
		print('2 - Girar Anti-Horario')
		print('3 - Parar')
        print('4 - Mais Velocidade')
        print('5 - Menos Velocidade')
        print('q - Sair')
		print('-------------------------------')
		strkey = raw_input()
		if strkey == 'q':
			print("Desativando.")
			meu_motor.stop()
            meu_motor.clear_io()
            break
		elif strkey == '1':
			print("Horario.")
			meu_motor.girarHorario()
		elif strkey == '2':
		    meu_motor.girarAntiHorario()
    	elif strkey == '3':
            meu_motor.stop()
    	elif strkey == '4':
            nova_velocidade = meu_motor.velocidade + 5
            if nova_velocidade > 100:
                nova_velocidade = 100
		    meu_motor.setVelocidade(nova_velocidade)
        elif strkey == '5':
            nova_velocidade = meu_motor.velocidade - 5
            if nova_velocidade < 0:
                nova_velocidade = 0
		    meu_motor.setVelocidade(nova_velocidade)

#------------------------------------------------------------------------------
