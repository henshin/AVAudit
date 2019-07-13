import shutil
import string
import random
import sys

seen = open("seen.txt").readlines()
seen = [i.strip() for i in seen]

def randS(n):
	return ''.join(random.choice(string.ascii_uppercase + string.digits+string.ascii_lowercase) for _ in range(n))

file_name = sys.argv[1]
a = sys.stdin.read()


for i in a.split("\n"):
	if("detected" in i):
		detect = i.split("detected")[1].strip()
		if detect not in seen:
			shutil.copyfile(file_name, "C:\\Users\\caive\\AV_AUDIT\\antiviruses\\kaspersky\\samples\\sample_"+randS(5)+".vir")
			a = open("seen.txt", "a").write(detect+"\n")
		else:
			print("ALREADY IN DIRECTORY")