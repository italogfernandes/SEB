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
import webbrowser
from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4.uic import loadUiType
from threading import Thread, Lock, Timer
from Queue import Queue
from datetime import datetime
from sets import Set
import numpy as np
from threadhandler import ThreadHandler
from arduinohandler import *
import math
import serial.tools.list_ports as serial_tools
from datetime import datetime
from mingus.midi import  fluidsynth
from mingus.containers import Note
from mingus.containers import MidiInstrument

#------------------------------------------------------------------------------
Ui_MainWindow, QMainWindow = loadUiType('mainwindow.ui')
#------------------------------------------------------------------------------
class Main(QMainWindow, Ui_MainWindow):
	def __init__(self):
		super(Main,self).__init__()
		self.setupUi(self)

		self.avr  = None
		self.threshold1 = 1.0
		self.threshold2 = 2.0
		self.threshold3 = 3.0

		self.threshold1reached = False
		self.threshold2reached = False
		self.threshold3reached = False
		self.oldthreshold1reached = self.threshold1reached
		self.oldthreshold2reached = self.threshold2reached
		self.oldthreshold3reached = self.threshold3reached


		self.stateChanged = False

		self.populateSerialPorts()

		self.btnConnectDisconnect.clicked.connect(self.doConnect)
		self.btnStartStop.clicked.connect(self.doStartStop)

		self.sliderLimiar1.valueChanged.connect(self.threshold1Changed)
		self.sliderLimiar2.valueChanged.connect(self.threshold2Changed)
		self.sliderLimiar3.valueChanged.connect(self.threshold3Changed)

		self.scene = QGraphicsScene(self)
		self.graphicsLeds.setScene(self.scene)
		self.normalColor = QBrush(QColor.fromRgb(162,178,245))
		self.hilightColor = QBrush(QColor.fromRgb(57,255,77))
		self.outlinePen = QPen(Qt.black)
		self.outlinePen.setWidth(1)
		h = self.scene.height()
		w = self.scene.width()

		self.circleLed1 = self.scene.addEllipse(w/2,(h/2)-150,100,100,self.outlinePen,self.normalColor)
		self.circleLed2 = self.scene.addEllipse(w/2,h/2,100,100,self.outlinePen,self.normalColor)
		self.circleLed3 = self.scene.addEllipse(w/2,(h/2)+150,100,100,self.outlinePen,self.normalColor)
		self.textLed1 = self.scene.addText("Led 1", QFont("Arial", 12))
		self.textLed2 = self.scene.addText("Led 2", QFont("Arial", 12))
		self.textLed3 = self.scene.addText("Led 3", QFont("Arial", 12))
		self.textLed1.setPos((w/2)+25,35+(h/2)-150)
		self.textLed2.setPos((w/2)+25,35+(h/2))
		self.textLed3.setPos((w/2)+25,35+(h/2)+150)


	def show_error_msg(self,msg_to_show):
		msg = QMessageBox()
		msg.setIcon(QMessageBox.Warning)
		msg.setText(msg_to_show)
		msg.setWindowTitle("Erro")
		retval = msg.exec_()

	def show_info_msg(self,msg_to_show):
		msg = QMessageBox()
		msg.setIcon(QMessageBox.Information)
		msg.setText(msg_to_show)
		msg.setWindowTitle("Mensagem Info")
		retval = msg.exec_()

	def populateSerialPorts(self):
		if len(serial_tools.comports()) == 0:
			self.show_error_msg("Nenhuma porta Serial Disponivel")
			self.cbSerialPorts.setEnabled(False)
			self.btnStartStop.setEnabled(False)
		else:
			for serial_port in serial_tools.comports():
				self.cbSerialPorts.addItem(serial_port.device)
				self.cbSerialPorts.setCurrentIndex(self.cbSerialPorts.count()-1)
				if("Arduino" in serial_port.description):
					self.doConnect()

	def doConnect(self):
		if self.avr == None:
			self.avr = Arduino(self.cbSerialPorts.itemText(self.cbSerialPorts.currentIndex()))

		if self.avr != None:
			if self.avr.serialPort == None:
				try:
					if self.avr.open():
						#self.show_info_msg("Porta serial %s aberta com sucesso!" % (self.avr.port))
						self.btnConnectDisconnect.setText("Desconectar")
						self.cbSerialPorts.setEnabled(False)
						self.avr.stop()
				except Exception as e:
					self.show_error_msg("Erro ao abrir a porta serial")
			else:
				try:
					if self.avr.close():
						self.show_info_msg("Porta serial %s fechada com sucesso!" % (self.avr.port))
						self.btnConnectDisconnect.setText("Conectar")
						self.cbSerialPorts.setEnabled(True)
						self.avr.serialPort = None
				except Exception as e:
					self.show_error_msg("Erro ao fechar a porta serial")

	def doStartStop(self):
		if not self.avr.acqThread.isAlive:
			try:
				self.avr.start()
				#Thread that handles the data acquisition
				self.dataProc = ThreadHandler(self.runAquisition)

				#Start the threads
				self.avr.acqThread.start()
				self.dataProc.start()

				self.btnStartStop.setText("Stop")
				self.btnConnectDisconnect.setEnabled(False)
			except Exception as e:
				self.show_error_msg("Erro ao iniciar aquisicao.\nError Log: " + str(e))
		else:
			try:
				self.doStop()
				self.btnStartStop.setText("Start")
				self.btnConnectDisconnect.setEnabled(True)
			except Exception as e:
				self.show_error_msg("Erro ao finalizar aquisicao.\nError Log: " + str(e))

	def doStop(self):
		self.avr.stop()
		time.sleep(1)
		#Kill the threads
		self.avr.acqThread.kill()
		self.dataProc.kill()

	def runAquisition(self):
		if self.avr.dataQueue.qsize() > 0:
			self.disassemblePacket()

	def disassemblePacket(self):
		n = self.avr.dataQueue.qsize()
		for i in range(n):
			data = self.avr.dataQueue.get()
			#print data
			#print "data:\t%.2f" % (data)
			self.showReadValue(data[0])

			self.threshold1reached = data[0] > self.threshold1
			self.threshold2reached = data[0] > self.threshold2
			self.threshold3reached = data[0] > self.threshold3

			if not self.oldthreshold1reached and self.threshold1reached:
				print 'Subiu Limiar 1'
				self.stateChanged = True
			elif self.oldthreshold1reached  and not self.threshold1reached:
				print 'Desceu Limiar 1'
				self.stateChanged = True

			if not self.oldthreshold2reached and self.threshold2reached:
				print 'Subiu Limiar 2'
				self.stateChanged = True
			elif self.oldthreshold2reached  and not self.threshold2reached:
				print 'Desceu Limiar 2'
				self.stateChanged = True

			if not self.oldthreshold3reached and self.threshold3reached:
				print 'Subiu Limiar 3'
				self.stateChanged = True
			elif self.oldthreshold3reached  and not self.threshold3reached:
				print 'Desceu Limiar 3'
				self.stateChanged = True

			self.runStateMachine()

			self.oldthreshold1reached = self.threshold1reached
			self.oldthreshold2reached = self.threshold2reached
			self.oldthreshold3reached = self.threshold3reached

	def runStateMachine(self):
		if self.stateChanged:
			self.stateChanged = False
			#State 1 0 0 -> primeiro led
			if not self.threshold1reached:
				print "Apagado"
				self.clearGraphView()
			elif not self.threshold2reached:
				print "Led 1"
				self.updadeGraphView(self.circleLed1)
			elif not self.threshold3reached:
				print "Led 2"
				self.updadeGraphView(self.circleLed2)
			else:
				print "Led 3"
				self.updadeGraphView(self.circleLed3)

	def threshold1Changed(self):
		self.labelLimiar1.setText('Limiar 1: %.2f V' % (self.sliderLimiar1.value()/100.00))
		self.threshold1 = (self.sliderLimiar1.value()/100.00)

	def threshold2Changed(self):
		self.labelLimiar2.setText('Limiar 2: %.2f V' % (self.sliderLimiar2.value()/100.00))
		self.threshold2 = (self.sliderLimiar2.value()/100.00)

	def threshold3Changed(self):
		self.labelLimiar3.setText('Limiar 3: %.2f V' % (self.sliderLimiar3.value()/100.00))
		self.threshold3 = (self.sliderLimiar3.value()/100.00)

	def showReadValue(self,valorLido):
		self.sliderValorLido.setValue(int(np.round(valorLido*100)))
		self.lbTitleValorLido.setText('Valor Lido: %.2f V' % (valorLido))
		self.dialValorLido.setValue(int(np.round(valorLido*100)))

	def clearGraphView(self):
		self.circleLed1.setBrush(self.normalColor)
		self.circleLed2.setBrush(self.normalColor)
		self.circleLed3.setBrush(self.normalColor)

	def updadeGraphView(self,shapetoupdate):
		self.clearGraphView()
		shapetoupdate.setBrush(self.hilightColor)

if __name__ == '__main__':
	import sys
	from PyQt4 import QtGui

	app = QtGui.QApplication(sys.argv)
	main = Main()
	#main.plot()
	main.show()
	sys.exit(app.exec_())
