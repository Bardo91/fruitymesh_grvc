#! /usr/bin/env python3

from sys import argv
import matplotlib.pyplot as plt
from statistics import mean, mode, median
import array

if len(argv) != 2:
    print("Error, necesita argumento de entrada")
    exit()
else:
    inputFileName = str(argv[1])
    logFile = open(inputFileName,"r")

    rssis = []
    line = logFile.readline()
    while line != "":
        try:
            number = int(line)
            rssis.append(number)
        except:
            number = 0
        line = logFile.readline()

    # medium filter
    rssisMean = []
    rssisMedian = []
    rssisMode = []
    T = 10;
    for i in range(len(rssis)):
        if(i >= T):
            rssisMean.append(mean(rssis[i-T:i]))
            rssisMedian.append(median(rssis[i - T:i]))
            #rssisMode.append(mode(rssis[i - T:i]))

    plt.figure(1)
    plt.plot(rssisMean, 'b*')
    plt.plot(rssisMedian, 'r*')
    plt.legend(['mean', 'median'])
    plt.show()

    #plt.plot(rssisMode, '*')
    #plt.ylabel('mode')
    #plt.show()