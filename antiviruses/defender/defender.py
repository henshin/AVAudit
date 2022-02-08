import os
import logging
import sys
import struct
import time
import json
import subprocess

from antiviruses import leaker as leak_funcs

class Defender:
	# defenderCmdLine has [FILE] to be replaced with the file to be scanned
	def __init__(self, samplesFolderPath, logLevel):

		self.logger = logging.getLogger("Defender")
		self.logger.setLevel(logLevel)
		file_handler = logging.FileHandler("logs\\Defender.log")
		stderr_handler = logging.StreamHandler()
		file_handler.setFormatter(logging.Formatter("[%(name)s] %(levelname)s - %(message)s"))
		stderr_handler.setFormatter(logging.Formatter("[%(name)s] %(levelname)s - %(message)s"))
		self.logger.addHandler(file_handler)
		self.logger.addHandler(stderr_handler)

		self.samplesFolderPath = samplesFolderPath
		self.currentDir = os.path.dirname(os.path.abspath(__file__))
		self.utils = "{}\\utils\\".format(self.currentDir)

		self.avExe = r"C:\PROGRA~1\WINDOW~1\MpCmdRun.exe"
		self.avArgs = ["-Scan", "-DisableRemediation", "-ScanType","3","-File",self.utils+'stub.exe']

		self.logger.info("Initialised the Windows Defender tester")
		self.logger.info("Defender: "+self.avExe)

		self.tablePath = self.currentDir+"\\defender_table.json"
		self.logger.info("Loading virus dictionary from {}...".format(self.tablePath))

		self.leaker = leak_funcs.Leaker(samplesFolderPath, self.logger, self.utils, self.avExe, self.avArgs, 4)
		self.table = self.leaker.loadTable(self.tablePath)


	# From the results of an antivirus scan, filter queries the virus dictionary to return a string
	def leak(self, cFilePath, compilelines=[], template="", shouldTime=True, repeat=1, leakType="", leakLive=True, max_bytes=0):
		for i in range(0, repeat):
			self.leaker.leak( self.filter, cFilePath, compilelines, template, shouldTime, repeat, leakType, leakLive, max_bytes)


	def filter(self, s, leakType):
		if(not self.table):
			self.logger.error("The virus table does not exist!")
			return
		ret_string = {} # Dictionary containing "index":"virus string" to be replaced by "index":chr(byte)
		threats = s.split("-------------------------------------------------------------------------------")
		bytes_read = 0
		for threat in threats:
			t_name = ""
			index = 0
			for line in threat.split("\n"):
				if "Threat   " in line:
					bytes_read += 1
					t_name = line.split(": ")[1].strip()
				elif "VFS" in line:
					index = 0 if "#" not in line else int(line.split("#")[1].strip(")\r\n"))
			if (t_name):
				ret_string[index] = t_name
		chr_on_roids = lambda x : chr(x) if (x <=127 and not leakType) else '%02x' % x
		if self.logger.isEnabledFor(logging.DEBUG):
			self.logger.debug(' '.join(str(self.table[ret_string[x]]["value"]) for x in sorted(ret_string.keys())))
		return bytes_read, ''.join(chr_on_roids(self.table[ret_string[x]]["value"]) for x in sorted(ret_string.keys()))
