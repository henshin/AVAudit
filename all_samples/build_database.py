import json

a = open("defender_dump.txt").read()
dic = {}
threats = a.split("-------------------------------------------------------------------------------")
for threat in threats:
	t_name = ""
	f_name = ""
	for line in threat.split("\n"):
		if("Threat     ") in line:
			t_name = line.split(": ")[1].rstrip().lstrip()
		elif("file        ") in line:
			f_name = line.split("\\")[-1].rstrip().lstrip()
	if (not t_name or not f_name):
		continue
	dic[f_name] = t_name
jsonf = {}
for index, value in enumerate(sorted(dic.keys())):
	jsonf[dic[value]] = index

with open("defender_table.json", "w") as outfile:
	json.dump(jsonf, outfile)