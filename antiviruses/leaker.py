import os
import sys
import json
import subprocess
import time
import struct


class Leaker:
	def __init__(self, samplesFolder, logger, utils, avExe, avArgs, bytesToRead):
		self.samples = samplesFolder
		self.logger = logger
		self.compiler = "mingw32-gcc"
		self.utils = utils
		self.defaultCompilerArgs = ["-nostdlib", "-mconsole", 
		"-o{}stub.exe".format(self.utils), 
		"-luser32", "-lkernel32",
		"-fno-stack-check","-fno-stack-protector",
		"-mno-stack-arg-probe"]

		self.bytesToRead = bytesToRead
		

		self.avExe = avExe
		self.avArgs = avArgs


	# compile the c file
	# add viruses
	# scan the exe
	# parse the output -> write to stdout and log
	# call add_rsrc.exe and change the indexes
	# if less than 4 detections, we have reached the end of the
	def leak(self, FILTER_FUNC, cFilePath, compilelines=[], template="", 
		shouldTime=True, repeat=1, leakType="", leakLive=True, max_bytes=0):
		self.logger.info("Compiling the dropper...")
		if(template):
			cFile = open(cFilePath).read()
			cFile = cFile.replace("<template>", template)
			outFile = open(self.utils+"templated.c", "w")
			outFile.write(cFile)
			outFile.close()
			cFilePath = self.utils+"templated.c"

		result = subprocess.run([self.compiler,cFilePath]+self.defaultCompilerArgs+compilelines, stdout=subprocess.PIPE)
		if "error" in result.stdout.decode("utf-8"):
			self.logger.info("Detected error in compilation, exiting.")
			return

		self.logger.debug(result.stdout.decode('utf-8'))
		self.logger.info("Encrypting samples and adding to dropper resource section...")

		result = subprocess.run([self.utils+"add_rsrc.exe", self.utils+"stub.exe", self.samples], stdout=subprocess.PIPE)
		self.logger.debug(result.stdout.decode('utf-8'))
		total_bytes_read = 0


		for i in range(0, repeat):
			start_read_index = 0
			leaked_string = ""
			self.logger.info(("[{}] ".format(template) if template else "") + "Beginning dump of {}!".format(cFilePath))
			start_time = time.time()
			while True:
				self.generateConfig(start_read_index, self.bytesToRead)
				result = subprocess.run([self.utils+"add_rsrc.exe", self.utils+"stub.exe", self.utils+"config.bin", "9999"], stdout=subprocess.PIPE).stdout.decode('utf-8')
				self.logger.debug(result)
				self.logger.debug("Scanline: "+self.avExe+" "+" ".join(self.avArgs))
				result = subprocess.run([self.avExe, self.avArgs], stdout=subprocess.PIPE, stderr=subprocess.PIPE).stdout.decode('utf-8')
				self.logger.debug(result)
				bytes_read, filtered = FILTER_FUNC(result, leakType)
				if not filtered or (max_bytes and len(leaked_string)> max_bytes):
					end_time = time.time()
					print()
					self.logger.info("Finished dump of {}!".format(cFilePath))

					if not leakLive:
						self.logger.info("Dumped string: '{}'".format(leaked_string))

					if(shouldTime):
						leaked_len = len(leaked_string)
						time_taken = end_time - start_time
						self.logger.info("Took {}s to dump {} bytes. Approximately {} bytes/second".format(round(time_taken, 2), total_bytes_read, round(total_bytes_read/time_taken, 2)))
					return leaked_string
					break
				else:
					if(bytes_read < self.bytesToRead): # there were multiple characters in a row
						start_read_index -= self.bytesToRead-1 # go back to last index + 1
						total_bytes_read -= self.bytesToRead-1
						filtered = filtered[0] if not leakType else filtered[0:2] # niiiice
					if leakLive:
						print(filtered,end="", flush=True)
					self.logger.debug(filtered)
					leaked_string += filtered
					total_bytes_read += self.bytesToRead
				start_read_index += self.bytesToRead



	def loadTable(self, tablePath):
		with open(tablePath) as json_table:
			self.logger.info("Loaded json file into table")
			return json.load(json_table)
			#self.logger.debug(self.table)


	def generateConfig(self, index, numberofbytes):
		config = open(self.utils+"config.bin", "wb")
		config.write(struct.pack("<I", index))
		config.write(struct.pack("<I", index+numberofbytes))
		config.close()
		self.logger.debug("Config written from index {} to {}".format(index, index+numberofbytes))
