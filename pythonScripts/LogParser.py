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

    templateStr = "b'[CustomModule.cpp@175 CUSTOMMOD]: BLE event handled with (distance,rssi): "

    rssis = []
    line = logFile.readline()
    while line != "":
        idx = line.find(templateStr)
        if(idx != -1):
            try:
                tagNumber = line[idx+len(templateStr):]
                tokens = tagNumber.split(",")
                tokens[1] = tokens[1].replace("\\r\\n'\n", "")
                tag = tokens[0];
                rssi = int(tokens[1]);
                rssis.append(rssi)
                parsedLogFile.write(str(tag)+", "+str(rssi)+"\n")
            except:
                number = 0
        line = logFile.readline()

    logFile.close()
    parsedLogFile.close()