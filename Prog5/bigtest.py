import sys

errors = 0
output = ""

def main():
	numEastOn = 0
	numWestOn = 0

	eastWaiting = 0
	westWaiting = 0

	dir = ""

	global output
	output = ""

	output = output + "\n\n\n\n\n\n\n\n\n"
	for line in sys.stdin:

		if "Eastward" in line:
			if "arrives at" in line:
				eastWaiting += 1
			elif "is on the rope" in line:
				if dir == "east" and westWaiting > 0:
					error("Eastward baboon got on while a Westward was waiting!")

				dir = "east"
				numEastOn += 1
				eastWaiting -= 1
			elif "completes crossing the canyon" in line:
				numEastOn -= 1

		elif "Westward" in line:
			if "arrives at" in line:
				westWaiting += 1
			elif "is on the rope" in line:
				if dir == "west" and eastWaiting > 0:
					error("Westward baboon got on while a Eastward was waiting!")

				dir = "west"
				numWestOn += 1
				westWaiting -= 1
			elif "completes crossing the canyon" in line:
				numWestOn -= 1		

		output = output + "\033[7m%s\033[0m\n" % line.replace('\n', '')

		output = output + 'On the rope: East: \033[22;32m%d\033[0m West: \033[22;33m%d\033[0m\n' % (numEastOn, numWestOn)
		output = output + 'Waiting:     East: \033[22;32m%d\033[0m West: \033[22;33m%d\033[0m\n' % (eastWaiting, westWaiting)
		output = output + "\n"
		if numEastOn > 0 and numWestOn > 0:
			error("Baboons moving in both directions!")

	global errors
	output += "\n\nNumber of Total Errors: \033[22;31m%d\033[0m\n" % (errors)
	if errors != 0:
		print "error"
		print output
	return errors

def error(str):
	global errors
	errors += 1
	global output	
	output = output + '\033[22;31mERROR! %s\033[0m\n' % str

main()
