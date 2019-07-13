import shutil
import string
import random
import sys

def randS(n):
	return ''.join(random.choice(string.ascii_uppercase + string.digits+string.ascii_lowercase) for _ in range(n))

file_name = sys.argv[1]
a = sys.stdin.read()
if("VFS:test.exe" in a):
	shutil.copyfile(file_name, "C:\\Users\\caive\\samples\\unique_vir\\actually_unique\\"+file_name)