
from antiviruses.defender import defender
from antiviruses.eset import eset
from antiviruses.kaspersky import kaspersky
import logging

d = defender.Defender("./antiviruses/defender/samples/", logging.INFO)



#e = eset.ESET("./antiviruses/eset/samples/", logging.INFO)
#k = kaspersky.Kaspersky("./antiviruses/kaspersky/samples/", logging.INFO)

#d.leak("fingerprints\\GetComputerNameA.c")
#e.leak("fingerprints\\GetComputerNameA.c")
#environmental_variables = ["ALLUSERSPROFILE", "PROGRAMFILES", "WINDIR", "TEMP", "HOMEDRIVE"]
#for i in environmental_variables:
#	k.leak("fingerprints\\dump_file_system.c", template=i)



#e = eset.ESET("./antiviruses/eset/samples/", logging.INFO)
#k = kaspersky.Kaspersky("./antiviruses/kaspersky/samples/", logging.INFO)




#d.leak("fingerprints\\dump_network.c", ["-lWininet"])

#d.leak("fingerprints\\dump_registers_after_nop.c")
#d.leak("fingerprints\\dump_windows.c")
#k.leak("fingerprints\\dump_windows.c")

#d.leak("fingerprints\\dump_mac.c", ["-liphlpapi"])
#e.leak("fingerprints\\dump_mac.c", ["-liphlpapi"])
#k.leak("fingerprints\\GetComputerNameA.c", repeat=3)
#d.leak("fingerprints\\GetComputerNameA.c")
#e.leak("fingerprints\\GetComputerNameA.c")

# d.leak("fingerprints\\test.c", leakType="b")
#d.leak("fingerprints\\dump_ascii.c", leakType="b")
#d.leak("fingerprints\\dump_ascii.c", leakLive=True)
#for av in [e, d]:
#	av.leak("fingerprints\\GetUsernameA.c", ["-ladvapi32"], shouldTime=True)
#e.leak("fingerprints\\GetCommandLine.c", ["-ladvapi32"], repeat=3)
d.leak("fingerprints\\GetUsernameA.c", ["-ladvapi32"], repeat=3, shouldTime = True)
#k.leak("fingerprints\\GetCommandLine.c", ["-ladvapi32"], repeat=3)

#
#d.leak("fingerprints\\fldpi.c", bytesToRead=1)

dlls = ["kernel32.dll", "user32.dll", "advapi32.dll"]
for i in dlls:
	d.leak("fingerprints\\dump_dll_exports.c", template=i)

environmental_variables = ["ALLUSERSPROFILE", "PROGRAMFILES", "WINDIR", "TEMP", "HOMEDRIVE"]
for i in environmental_variables:
	d.leak("fingerprints\\dump_file_system.c", template=i)




#e.leak("fingerprints\\dump_file_system.c", template="WINDIR", leakLive=True)
#d.leak("fingerprints\\dump_file.c", leakType="b", template=r"C:\\Windows\\System32\\kernel32.dll")

#e.leak("fingerprints\\dump_environ.c", leakLive = True)


#d.leak("fingerprints\\shell_execute.c", ["-lshell32"], template="echo hi", leakLive=True)
#d.leak("fingerprints\\test.c", leakLive = True);
#e.leak("fingerprints\\dump_processes.c", leakLive=True);