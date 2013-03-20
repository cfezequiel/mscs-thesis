#! /usr/bin/python

'''
This script recursively merges all the test data (.csv) into one huge .csv
file. 
'''

import os
import sys
import csv

counter = 0
headerRow = []

if len(sys.argv) < 2:
    print 'Usage: %s <top dir>' % sys.argv[0]
    exit(-1)

csvData = []
topDir = sys.argv[1]
for root, dirs, files in os.walk(topDir):
    print 'Processing ', root, ' ...'
    if root.find('image_test') >= 0:
        test = 'image'
    elif root.find('video_test') >= 0:
        test = 'video'
    else:
        continue

    if files != []:
        for file in files:
            # Check if csv file
            (name, ext) = os.path.splitext(file)
            if (ext != '.csv'):
                continue
            
            # Get test info from file name
            fileInfo = file.split('_')
            map = fileInfo[1]

            fileFullPath = "%s/%s" % (root, file)
            with open(fileFullPath, 'rb') as csvFile:
                reader = csv.reader(csvFile, delimiter=',')
                # Get data from first row
                firstRow = reader.next()
                (dontcare, obsX, obsY) = firstRow

                # Check header row
                if headerRow == []:
                    headerRow  = reader.next()
                    headerRow = ['Test', 'Map'] + headerRow + ['RefObsX', 'RefObsY']
                    csvData.append(headerRow)
                else:
                    # Skip that row
                    reader.next()

                # Process remaining rows
                for row in reader:
                    row = [test, map] + row + [obsX, obsY]
                    csvData.append(row)


outFile = 'test_data.csv'
with open(outFile, 'wb') as csvFile:
    writer = csv.writer(csvFile, delimiter=',')
    for row in csvData:
        writer.writerow(row)

