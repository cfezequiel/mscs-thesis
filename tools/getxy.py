#! /usr/bin/python

import sys

tileWidth = 305
tileHeight = 305

if __name__ == '__main__':
    # Get inputs
    tx = int(sys.argv[1])
    ty = int(sys.argv[2])

    if tx > 0:
        x = -174 - tx * tileWidth + 0.5 * tileWidth
    elif tx < 0:
        x = (tileWidth - 174) - tx * tileWidth - 0.5 * tileWidth
    else:
        x = 0

    y = 140 + ty * tileHeight - 0.5 * tileHeight

    print (x,y)

