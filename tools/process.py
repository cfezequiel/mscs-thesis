#! /usr/bin/python

import os

app='~/Projects/TH/tools/fixImageTestData.py'
listing = os.listdir('.')
for file in listing:
    if os.path.splitext(file)[1] != '.csv':
        continue
    print 'Processing %s' % file
    ret = os.system('%s %s' % (app, file))
    if (ret):
        print 'Error processing %s' % file


