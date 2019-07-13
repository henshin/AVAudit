import shutil
import string
import random
import sys

def randS(n):
	return ''.join(random.choice(string.ascii_uppercase + string.digits+string.ascii_lowercase) for _ in range(n))
	
a = sys.stdin.read()
past_seened = open("C:\\Users\\caive\\samples\\seen.txt").read().splitlines()
cur_seened = []
b = a.split("-------------------------------------------------------------------------------")
seen = []
for i in b:
	#print(i)
	virus_id = ""
	vir_path = ""
	if(i.count("file") != 1): # zip files/upx
		continue
	for line in i.split("\n"):
		if "Threat  " in line:
			virus_id = line.split(": ")[1].strip()
		if "containerFile " in line:
			vir_path = line.split(": ")[1].strip()
			break
		elif "file " in line:
			vir_path = line.split(": ")[1].strip()
	if(virus_id in past_seened or virus_id in cur_seened or not virus_id or not vir_path):
		continue
	cur_seened.append(virus_id)
	print(virus_id+" ---> "+vir_path)
	shutil.copyfile(vir_path, "C:\\Users\\caive\\samples\\unique_vir\\first_byte\\sample_"+randS(5)+".vir")
c = open("C:\\Users\\caive\\samples\\seen.txt", "a+")
for i in cur_seened:
	c.write(i+"\n")