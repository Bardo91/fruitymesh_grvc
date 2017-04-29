#! /usr/bin/env python3

from sys import argv
import array

if len(argv) != 2:
    print("Error, necesita argumento de entrada")
    exit()
else:
    inputFileName = str(argv[1])
    logFile = open(inputFileName,"r")
    dotIdx = inputFileName.rfind(".")
    outputFileName = inputFileName[:dotIdx] + "parsed.txt"
    parsedLogFile = open(outputFileName, "w")

    templateStr = "[CustomModule.cpp@169 CUSTOMMOD]: BLE event handled with rssi: "

    rssis = []
    line = logFile.readline()
    while line != "":
        idx = line.find(templateStr)
        if(idx != -1):
            try:
                number = int(line[idx+len(templateStr):idx+len(templateStr)+3])
                rssis.append(number)
                parsedLogFile.write(str(number)+"\n")
            except:
                number = 0
        line = logFile.readline()

    logFile.close()
    parsedLogFile.close()