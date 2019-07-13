a = open("raw.txt").read()
import json
import os

res = {}
index = 0
for i in a.split("\n"):
	detect = ""
	loc = ""
	if ("detected") in i:
		detect = i.split('detected')[1].strip()
		loc = i.split('detected')[0].split(" ")[2].split("\\")[-1].strip()
	if loc and detect and index < 256:
		res[detect] = {"value":index, "file":loc}
		index += 1
print(res)