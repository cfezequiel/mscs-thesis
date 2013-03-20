#! /usr/bin/python

'''
This script processes the registraton data for the robot navigation test.
'''

import csv
import sys

oftenInC4Lab = ['5-6 days a week', 'Everyday', '3-4 days a week']
recentEyeExam = ['This month', '1-2 months ago', '3-12 months ago',\
                 'More than a year ago']

if len(sys.argv) < 2:
    print 'Usage: %s <registration csv file>'
    exit(-1)

csvData = []
file = sys.argv[1]
with open(file, 'rb') as csvFile:
    reader = csv.reader(csvFile, delimiter=',')
    # Skip header
    reader.next()
    for row in reader:
        # Unpack row
        (name, gender, age, lab, eye, rts, fps, email, work) = row

        # Evaluate data
        name = name.split(' ')[0].lower()
        isMale = 1 if gender == 'Male' else 0 
        isOftenInC4Lab = 1 if lab in oftenInC4Lab else 0
        hadRecentEyeExam = 1 if eye in recentEyeExam else 0
        playsVideoGames = 1 if rts == 'Yes' or fps == 'Yes' else 0

        # Store evaluated data
        row = [name, age, isMale, isOftenInC4Lab, hadRecentEyeExam,\
               playsVideoGames]
        csvData.append(row)

# Write output to file
with open('registration_out.csv', 'wb') as csvFile:
    writer = csv.writer(csvFile, delimiter=',')
    # Create header
    header = ['name', 'age', 'isMale', 'isOftenInC4Lab', 'hadRecentEyeExam',\
              'playsVideoGames']

    writer.writerow(header)
    for row in csvData:
        writer.writerow(row)


