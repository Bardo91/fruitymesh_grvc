#! /usr/bin/env python3

from sys import argv
import matplotlib.pyplot as plt
from statistics import mean, mode, median
import array

def cmode(array):
    values = []
    repetitions = []
    for i in range(len(array)):
        if(array[i] in values):
            repetitions[values.index(array[i])] += 1
        else:
            values.append(array[i])
            repetitions.append(0)
    maxRepetitions = max(repetitions)
    return values[repetitions.index(maxRepetitions)]

if len(argv) != 2:
    print("Error, necesita argumento de entrada")
    exit()
else:
    inputFileName = str(argv[1])
    logFile = open(inputFileName,"r")

    rssis = []
    tags  = []
    line = logFile.readline()
    while line != "":
        try:
            lineTokens = line.split(",")
            if len(lineTokens) == 1:
                number = int(lineTokens[0])
                rssis.append(number)
            elif len(lineTokens) == 2:
                tag = int(lineTokens[0])
                number = int(lineTokens[1])
                tags.append(tag)
                rssis.append(number)
            else:
                print("Unexpected number of tokens")
                exit()
        except:
            number = 0
        line = logFile.readline()

    # medium filter
    rssisMean = []
    rssisMedian = []
    rssisMode = []
    T = 20;
    for i in range(len(rssis)):
        if(i >= T):
            rssisMean.append(mean(rssis[i-T:i]))
            rssisMedian.append(median(rssis[i - T:i]))
            rssisMode.append(cmode(rssis[i - T:i]))

    dotIdx = inputFileName.rfind(".")
    outputFileName = inputFileName[:dotIdx] + "_processed.txt"
    processedLog = open(outputFileName, "w")
    for i in range(len(rssis)):
        if(i >= T):
            processedLog.write(str(rssis[i]) + ", "+str(rssisMean[i-T]) + ", "+str(rssisMedian[i-T]) + ", "+str(rssisMode[i-T]) + "\n")


    plt.figure(1)
    plt.plot(rssis[T:], 'k')
    plt.plot(rssisMean, 'b')
    plt.plot(rssisMedian, 'r')
    plt.plot(rssisMode, 'g')
    plt.legend(['raw','mean', 'median','mode'])
    plt.figure(2)
    plt.plot(tags[T:], 'k')
    plt.show()