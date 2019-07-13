a = open("raw.txt").read()
import json
import os

res = {}
index = 0
for i in a.split("\n"):
	detect = ""
	loc = ""
	if ("result") in i:
		b = i.split('result="')[1]
		detect = b.split('", ')[0]
	if("name") in i:
		loc = i.split('", result')[0].split('name=".\\')[1]
	if loc and detect:
		res[detect] = {"value":index, "file":loc}
		index += 1
print(res)