#! /usr/bin/python

'''
This script changes the Y-values of the obstacle field of the
test data from negative to positive.
'''

import os
import sys
import csv

if len(sys.argv) < 2:
    print "Usage: %s <test data dir>"
    exit(-1)

topDir = sys.argv[1]
for root, dirs, files in os.walk(topDir):
    if files == []:
        continue

    for file in files:
        (name, ext) = os.path.splitext(file)
        if name.find('registration') >= 0 or ext != ".csv":
            continue
        filePath = root + '/' + file
        print 'Processing file: %s' % filePath
        csvData = []
        with open(filePath, 'rb') as csvFile:
            reader = csv.reader(csvFile, delimiter=',')
            csvData.append(reader.next())
            csvData.append(reader.next())
            for row in reader:
                y = float(row[-1])
                y = abs(y)
                newRow = row
                newRow[-1] = str(y)
                csvData.append(newRow)
        with open(filePath, 'wb') as csvFile:
            writer = csv.writer(csvFile, delimiter=',')
            for row in csvData:
                writer.writerow(row)


