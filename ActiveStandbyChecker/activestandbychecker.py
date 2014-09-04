#!/usr/bin/env python
import string
import subprocess

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

community = 'ptf'
file = open('ips.txt', 'r')
for line in file:
	constructor = line.split(":")[0]
	ip = line.split(":")[1]
	ip = ip.split("\n")[0]
	if constructor == "F5":
		bashCommand = "snmpget -O q -v2c -c "+community+" "+ip+" 1.3.6.1.4.1.3375.2.1.14.3.1.0"
		process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
		stat = process.communicate()[0]
		if stat != "":
			stat = stat.split(" ")[1]
			stat = stat.split("\n")[0]
			if stat is '4':
				print constructor + " : " + ip + " => " + bcolors.OKGREEN + "Active" + bcolors.ENDC
			elif stat is '3':
				print constructor+" : " + ip + " => " + bcolors.WARNING + "Standby" + bcolors.ENDC
			else:
				print constructor+" : " + ip + " => " + bcolors.HEADER + "Unknown" + bcolors.ENDC
	elif constructor == "Radware":
                bashCommand = "snmpwalk -O q -v2c -c "+community+" "+ip+" .1.3.6.1.4.1.1872.2.6.1.2.1.3.1.2"
                process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
                if stat != "":
			result = process.communicate()[0]
                	result = result.split("\n")
			result.pop()
                	bashCommand = "snmpwalk -O q -v2c -c "+community+" "+ip+" .1.3.6.1.4.1.1872.2.6.1.2.1.3.1.4"
                	process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
                	result2 = process.communicate()[0]
                	result2 = result2.split("\n")
			result2.pop()
			i=0
			for vADC in result:
				stat = result2[i].split(" ")[1]
				vADC = vADC.split('"')[1]
                		if stat is '5':
					print constructor+" : "+ vADC + " => " + bcolors.FAIL + "Offline" + bcolors.ENDC
                		elif stat is '6':
                                	print constructor+" : "+ vADC + " => " + bcolors.OKGREEN + "Active" + bcolors.ENDC
				elif stat is '7':
                                	print constructor+" : "+ vADC+" => " + bcolors.WARNING + "Standby" + bcolors.ENDC
				else:
                                	print constructor+" : "+ vADC+" => " + bcolors.HEADER + "Unknown" + bcolors.ENDC
				i+=1
	elif constructor == "Juniper":
                bashCommand = "snmpgetnext -O q -v2c -c "+community+" "+ip+" 1.3.6.1.4.1.2636.3.1.14.1"
                process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
                stat = process.communicate()[0]
                if stat != "":                        
			if "3.6.1.4.1.2636.3.1.14.1" in stat.split(" ")[0]:
                                print constructor + " : " + ip + " => " + bcolors.OKGREEN + "Active" + bcolors.ENDC
                        else:
                                print constructor+" : " + ip + " => " + bcolors.WARNING + "Standby" + bcolors.ENDC
        elif constructor == "Fortinet":
		bashCommand = "snmpwalk -O q -v2c -c "+community+" "+ip+" 1.3.6.1.2.1.31.1.1.1.1"
                process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
                result = process.communicate()[0]
		result = result.split("\n")
		result2.pop()
		if result != "":
			interface = "notfound"
                	for line in result:
				if '"port1"' in line:
					line=line.split(" ")[0]
					interface=line.split(".")[-1]
					break
		if interface is not "notfound":
			bashCommand = "snmpget -O q -v2c -c "+community+" "+ip+" 1.3.6.1.2.1.31.1.1.1.16."+interface
                	process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
                	stat = process.communicate()[0]
                	if stat != "" and "3.6.1.2.1.31.1.1.1.16" in stat.split(" ")[0]:
                        	stat = stat.split(" ")[1]
                        	stat = stat.split("\n")[0]
				if stat is '1':
                        	        print constructor + " : " + ip + " => " + bcolors.OKGREEN + "Active" + bcolors.ENDC
                        	elif stat is '2':
                        	        print constructor+" : " + ip + " => " + bcolors.WARNING + "Standby" + bcolors.ENDC
				else:
					print constructor+" : " + ip + " => " + bcolors.HEADER + "Unknown" + bcolors.ENDC
		else:
			print constructor + " : " + ip + " => " + bcolors.OKGREEN + "Interface port1 Not found" + bcolors.ENDC

