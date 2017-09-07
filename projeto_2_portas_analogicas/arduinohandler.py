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
from struct import unpack
from ctypes import c_short
from Queue import Queue
from serial import Serial
from threading import Timer
from threadhandler import ThreadHandler
import numpy as np
import scipy as sp
import os, sys
#------------------------------------------------------------------------------
#Communicationconstants
#Constants for handling serial communication
class CommConsts():
	UART_ST = 0x24  			#Start transmission
	UART_ET = 0x0A				#End transmission
	CMD_START = 'I'				#Command that starts acquisition
	CMD_STOP = 'P'  			#Command that stops acquisition
	CMD_TIMEOUT = 0x03			#Board response timed out

#------------------------------------------------------------------------------
class SerialHandler():
	def __init__(self,_port='ttyUSB0',_baud=115200,_timeout=0.5):
		self.port =  str(_port)
		self.baud = _baud
		self.timeout = _timeout
		self.waiting = False
		self.serialPort = None

	def open(self):
		try:
			self.serialPort = Serial(self.port,self.baud,timeout=self.timeout)
			if self.serialPort.is_open:
				self.serialPort.flushInput()
				self.serialPort.flushOutput()
				return True
			else:
				return False
		except:
			return False

	def close(self):
		try:
			self.serialPort.flushInput()
			self.serialPort.flushOutput()
			self.serialPort.close()
			if self.serialPort.is_open:
				return False
			else:
				return True
		except:
			return False

	def waitBytes(self,_numBytes):
		try:
			self.waiting = True
			t = Timer(self.timeout,self.getTimeout)
			t.start()
			numWaiting = 0
			while True:
				if(self.serialPort.is_open):
					numWaiting = self.serialPort.in_waiting
					if numWaiting < _numBytes and self.waiting is True:
						pass
					else:
						break
				else:
					break
			if numWaiting >= _numBytes:
				t.cancel()
				return True
			else:
				return False
		except:
			return False

	def getTimeout(self):
		self.waiting = False

	def waitSTByte(self,_startByte):
		receivedByte = 0
		while True:
			ret = self.waitBytes(1)
			if ret:
				receivedByte = ord(self.serialPort.read())
				if receivedByte == _startByte:
					return True
				else:
					return False
			else:
				return False

	def to_int16(self,_MSB,_LSB):
		return c_short((_MSB<<8) + _LSB).value

	def to_float(self,_byteVector):
		binF = ''.join(chr(i) for i in _byteVector)
		return unpack('f',binF)[0]
#------------------------------------------------------------------------------
#Definition of the Arduino class
#Inherits from SerialHandler for using serial communication
#Serial data is stored in a queue that can be accessed for
#later processing
class Arduino(SerialHandler):
	#Constructor
	def __init__(self,_port='/dev/ttyUSB0',_baud=115200):
		self.acqThread = ThreadHandler(self.readPackage)
		self.isConnected = False
		self.dataQueue = Queue()
		self.flagAcq = False
		SerialHandler.__init__(self,_port,_baud)

	#Stars data acquisition
	#Needs return?
	def start(self):
		self.serialPort.write(CommConsts.CMD_START)
		self.serialPort.flushInput()
		self.serialPort.flushOutput()
		self.dataQueue = Queue()
		self.flagAcq = True

	#Stops data acquisition
	#Needs return?
	def stop(self):
		self.serialPort.write(CommConsts.CMD_STOP)

	'''
	Summary: Method for reading packages
	The defined package contains:
	1st byte: Start transmission
	2nd byte: Potentiometer MSB value
	3rd byte: Potentiometer LSB value
	4th byte: End flag 1 if Switch is ON or End flag 2 if Switch is OFF
	'''
	def readPackage(self):
		#print 'readPackage called'
		dataVector = []
		cont = 0
		try:
			#print 'Tentando leitura'
			ret = self.waitSTByte(CommConsts.UART_ST)
			if ret:
				#print 'Start recebido'
				ret = self.waitBytes(2)
				if ret:
					data = self.serialPort.read(2)
					data = map(ord,data)
					#print 'Recebido  :\t%d\t%d\t=\t%d' % (data[0], data[1], (data[0] << 8 | data[1]))
					dataVector.append(3.3*(data[0] << 8 | data[1])/4095.00)
					#print 'Convertido:\t%.2f' % (dataVector[0])
				ret = self.waitBytes(1)
				if ret:
					endByte = ord(self.serialPort.read())
					if endByte == CommConsts.UART_ET:
						if self.flagAcq is True:
							#print 'Lido:\t%.2f\t%.2f' % (dataVector[0], dataVector[1])
							self.acqThread.lock.acquire()
							self.dataQueue.put(dataVector)
							self.acqThread.lock.release()
					else:
						print dataVector
						print ' package error!'
		except:
			print data
			print ' read error!'
#------------------------------------------------------------------------------
#Tests acquisition
if __name__ == "__main__":
	avr = Arduino('/dev/ttyACM0')
	avr.open()
	while(True):
		print '-------------------------------'
		print 'Arduino Data Acquisition'
		print '-------------------------------'
		print 'Menu'
		print '1 - New acquisition'
		print '2 - Stop acquisition'
		print '3 - Exit'
		print '-------------------------------'
		strkey = raw_input()
		if strkey == '1':
			print("Start command sent and thread started.")
			avr.start()
			avr.acqThread.start()
		elif strkey == '2':
			print("Stop command sent and thread killed.")
			avr.stop()
			avr.acqThread.kill()
		elif strkey == '3':
			print("Connection Closed.")
			avr.close()
			break
#------------------------------------------------------------------------------
