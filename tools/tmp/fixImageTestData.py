#! /usr/bin/python

'''
This script corrects all the (0,0) pose values in image-test .csv data files.
The (0,0) pose values are replaced with the pose corresponding to where the
images used to the test were taken.

Supported test types:
test1a
test1b
test2

'''

import os
import sys
import csv

def processTest1a(filename):
    out = []
    with open(filename, 'rb') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        # Skip 1st 2 rows
        out.append(reader.next())
        out.append(reader.next())
        # Process 'home' row
        row = reader.next()
        row[2] = 'NA'
        row[3] = 'NA'
        row[4] = -1242
        row[5] = 62208
        out.append(row)
        # Process remaining rows
        i = 0 
        for row in reader:
            row[2] = 'NA'
            row[3] = 'NA'
            row[4] = -936
            row[5] = 59157 - i * (305 *10)
            out.append(row)
            i = i + 1
        csvfile.close()

    with open(filename, 'wb') as csvfile:
        writer = csv.writer(csvfile, delimiter=',')
        for row in out:
            writer.writerow(row)

def processTest2(filename):
    out = []
    with open(filename, 'rb') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        # Skip 1st 2 rows
        out.append(reader.next())
        out.append(reader.next())
        # Process rows
        i = 0 
        for row in reader:
            row[2] = 'NA'
            row[3] = 'NA'
            row[4] = 9433 - i * (305 *10)
            row[5] = 63122 
            out.append(row)
            i = i + 1
        csvfile.close()

    with open(filename, 'wb') as csvfile:
        writer = csv.writer(csvfile, delimiter=',')
        for row in out:
            writer.writerow(row)

def getTestInfo(filename):
    filename = os.path.basename(filename)
    tokens = filename.split('_')
    name = tokens[0]
    testType = tokens[1]
    (timeStamp, ext) = '_'.join(tokens[2:]).split('.')
    return (name, testType, timeStamp)


def fixImageTestFile(filename):

    # Parse the input filename
    (name, testType, timeStamp) = getTestInfo(filename)

    # Process depending on testType
    if testType == 'test1a':
        processTest1a(filename)
    elif testType == 'test1b':
        processTest1a(filename)
    elif testType == 'test2':
        processTest2(filename)
    else:
        print 'Undefined testType (%s)' % testType
        exit(-1)

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print 'Usage: %s <image test directory>'
        exit(-1)

    topDir = sys.argv[1]
    for root, dirs, files in os.walk(topDir):
        if root.find('image_test') < 0:
            continue

        if files == []:
            continue
        
        for file in files:
            (name, ext) = os.path.splitext(file)
            if name.find('registration') >= 0 or ext != ".csv":
                continue
            filePath = root + '/' + file
            print 'Processing file: %s' % filePath
            try:
                fixImageTestFile(filePath)
            except:
                print 'Warning: could not process %s' % filePath
                


