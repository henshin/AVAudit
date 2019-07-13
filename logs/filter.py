a = open("ESET.log").read()
import json
import os

res = {}
index = 0
for i in a.split("\n"):
	detect = ""
	loc = ""
	if ("['") in i:
		b = i.split("['")[1]
		detect = b.split("']")[0]
	if detect:
		res[detect] = {"value":index}
		index += 1
print(res)