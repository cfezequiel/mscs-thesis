#! /usr/bin/python

'''
This script merges the test data and the processed registration data.
'''

import sys
import csv

# Mappings
maps = {'test1a':0,'test1b':1,'test2':2}
tests = {'image':1,'video':0}

if len(sys.argv) < 3:
    print 'Usage: %s <test csv file> <registration csv file>'
    exit(-1)

(regFile, testFile) = sys.argv[1:3]

# Read registration file
regHeader = []
regData = {}
with open(regFile, 'rb') as csvFile:
    reader = csv.reader(csvFile, delimiter=',')
    regHeader = reader.next()[1:]
    for row in reader:
        key = row[0]
        regData[key] = row[1:]

# Read and process test file
keyIndex = 2
skip = 4
regTestData = []
with open(testFile, 'rb') as csvFile:
    reader = csv.reader(csvFile, delimiter=',')
    # Generate header
    row = reader.next()
    row = row[:keyIndex] + ['userID'] + regHeader + row[keyIndex + skip:]
    row[0] = 'Test (%s)' % str(tests).replace(',',';')
    row[1] = 'Map (%s)' % str(maps).replace(',',';')
    regTestData.append(row)

    # Process rows
    for row in reader:
        row[0] = tests[row[0]]
        row[1] = maps[row[1]]
        key = row[keyIndex].lower()
        #if not regData.has_key(key):
        #    print 'No matching reference for %s' % key
        #    continue
        found = False
        userID = 0
        for regKey in regData.keys():
            if regKey.find(key) >= 0:
                found = True
                break
            userID += 1;
        if not found:
            print 'Warning: could not find matching entry for %s' % key
            continue

        regRow = regData[regKey]
        row = row[:keyIndex] + [userID] + regRow + row[keyIndex + skip:]
        regTestData.append(row)

# Write merged data to file
outFile = 'merged_data.csv'
with open(outFile, 'wb') as csvFile:
    writer = csv.writer(csvFile, delimiter=',')
    for row in regTestData:
        writer.writerow(row)


