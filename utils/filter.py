import shutil
import string
import random
import sys

def randS(n):
	return ''.join(random.choice(string.ascii_uppercase + string.digits+string.ascii_lowercase) for _ in range(n))
	
a = sys.stdin.read()
print(a)