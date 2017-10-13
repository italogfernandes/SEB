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
#Controla um servo-motor
class ServoHandler():
	def __init__(self, _pinSignal=13, _limits=[30,150]):
        self.pinSignal = _pinSignal
        self.pwmSignal = None
        self.initialize_pins()

	def initialize_pins(self):
        GPIO.setup(self.pinSignal, GPIO.OUT) #Inicia os pino para controle do angulo
        self.pwmSignal = GPIO.PWM(self.pinSignal, 500)  # channel=self.pinSignal, frequency=500Hz
        self.pwmSignal.start(0) #Inicialmente = 0

    def clear_io(self):
        self.pwmSignal.stop()
        GPIO.cleanup(self.pinSignal)

#------------------------------------------------------------------------------
#Tests
if __name__ == "__main__":
    GPIO.setmode(self.boardMode) #seta a board para BCM ou BOARD
	meu_servo = ServoHandler(13)
	while(True):
		print('-------------------------------')
		print('Controlling a Servo Motor')
		print('-------------------------------')
		print('Menu')
		print('1 - +5º')
		print('2 - -5º')
		print('3 - Get Angulo')
        print('q - Sair')
		print('-------------------------------')
		strkey = raw_input()
		if strkey == 'q':
			print("Desativando.")
            meu_servo.clear_io()
            break
		elif strkey == '1':
    		#TODO
		elif strkey == '2':
		    #TODO
    	elif strkey == '3':
            #TODO


#------------------------------------------------------------------------------
