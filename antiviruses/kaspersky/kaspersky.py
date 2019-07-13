import os
import logging
import sys
import struct
import time
import json
import subprocess

from antiviruses import leaker as leak_funcs


class Kaspersky:
	# defenderCmdLine has [FILE] to be replaced with the file to be scanned
	def __init__(self, samplesFolderPath, logLevel):

		self.logger = logging.getLogger("Kaspersky")
		self.logger.setLevel(logLevel)
		file_handler = logging.FileHandler("logs\\Kaspersky.log")
		stderr_handler = logging.StreamHandler()
		file_handler.setFormatter(logging.Formatter("[%(name)s] %(levelname)s - %(message)s"))
		stderr_handler.setFormatter(logging.Formatter("[%(name)s] %(levelname)s - %(message)s"))
		#self.logger.addHandler(file_handler)
		self.logger.addHandler(stderr_handler)

		self.samplesFolderPath = samplesFolderPath
		self.currentDir = os.path.dirname(os.path.abspath(__file__))
		self.utils = "{}\\utils\\".format(self.currentDir)

		self.avExe = r"C:\Program Files (x86)\Kaspersky Lab\Kaspersky Anti-Virus 19.0.0\avp.com"
		self.avArgs = ["SCAN ", self.utils+'stub.exe ',"/i0 ", "/fi "]

		self.logger.info("Initialised the Kaspersky tester")
		self.logger.info("Kaspersky: "+self.avExe)

		self.tablePath = self.currentDir+"\\kaspersky_database.json"
		self.logger.info("Loading virus dictionary from {}...".format(self.tablePath))
		

		self.leaker = leak_funcs.Leaker(samplesFolderPath, self.logger, self.utils, self.avExe, self.avArgs, 1)
		self.table = self.leaker.loadTable(self.tablePath)


	# From the results of an antivirus scan, filter queries the virus dictionary to return a string
	def leak(self, cFilePath, compilelines=[], template="", shouldTime=True, repeat=1, leakType="", leakLive=True, max_bytes=0):
		for i in range(0, repeat):
			self.leaker.leak( self.filter, cFilePath, compilelines, template, shouldTime, repeat, leakType, leakLive, max_bytes)


	def filter(self, s, leakType):
		if(not self.table):
			self.logger.error("The virus table does not exist!")
			return
		ret_string = [] # Dictionary containing "index":"virus string" to be replaced by "index":chr(byte)
		threats = s.split("\n")
		bytes_read = 0
		for threat in threats:
			t_name = ""
			if "detected" in threat and "detected:" not in threat:
				bytes_read += 1
				t_name = threat.split('detected')[1].strip()
				ret_string.append(t_name)
		chr_on_roids = lambda x : chr(x) if (x <=127 and not leakType) else '%02x' % x
		return bytes_read, ''.join(chr_on_roids(self.table[x]["value"]) for x in ret_string)



