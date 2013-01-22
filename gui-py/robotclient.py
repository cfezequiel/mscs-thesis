#! /usr/bin/python

import random
import defer

from AriaPy import *
from ArNetworkingPy import *

class Figure:
    
    def __init__(self):
        self.figureName = ''
        self.shapeID = ''
        self.colorPrimary = 0
        self.shapeSize = 0
        self.layerNum = 0
        self.refreshTime = 0
        self.colorSecond = 0

    def __str__(self):
        s = 'Figure name: %s\n' % self.figureName
        s += 'Shape ID: %s\n' % self.shapeID
        s += 'Primary color: 0x%x\n' % self.colorPrimary
        s += 'Shape size: %d\n' % self.shapeSize
        s += 'Layer number: %d\n' % self.layerNum
        s += 'Refresh time: %d\n' % self.refreshTime
        s += 'Secondary color: 0x%x\n' % self.colorSecond

        return s

class AriaRobotClient(ArClientBase):

    def __init__(self):
        ArClientBase.__init__(self)
        self.deferreds = {}
        self.gotMapData = False
        self.mapDataStr = ''
        self.serverStatus = ''
        self.serverMode = ''
        self.batVoltage = 0 
        self.xpos = 0
        self.ypos = 0
        self.theta = 0
        self.goals = []
        self.figures = []

    def _readPacketToStr(self, packet):
        '''Read packet data and return a string'''

        data = []
        while (packet.getReadLength() < packet.getLength()):
            ch = chr(packet.bufToUByte())
            data.append(ch)
            if ch == '\x00':
                break

        return "".join(data)

    def _readPacketToStrList(self, packet):
        '''Read packet data and return list of strings.'''

        data = []
        for i in range(packet.getDataLength()):
            buf = chr(packet.bufToUByte())
            if buf == '\x00':
                buf = '\n'
            data.append(buf)
        data = "".join(data).split('\n')

        return data

    def _handleGetMap(self, packet):
        '''Handler for receiving map data.'''

        s = self._readPacketToStr(packet)
        self.mapDataStr += s
        print s
        if s.find('DATA') >= 0:
            print 'Got map.'
            self.gotMapData = True
            return

            # Save map
            mapFilename = 'tmp_'
            fp = open(mapFilename, 'w')
            fp.write(self.mapDataStr)
            fp.close()
            
    def _handleGetGoals(self, packet):
        '''Handler for receiving list of goals from robot server.'''

        self.goals = self._readPacketToStrList(packet)

        # Get rid of last element which is ''
        self.goals = self.goals[:-1]

    def _handleUpdate(self, packet):
        '''Handler for receiving robot status updates.'''

        self.serverStatus = self._readPacketToStr(packet)
        self.serverMode = self._readPacketToStr(packet)
        self.batVoltage = packet.bufToUByte2() 
        self.xpos = float(packet.bufToByte4())
        self.ypos = float(packet.bufToByte4())
        self.theta = float(packet.bufToByte2())

        print '%s, %s @ (%d, %d, %d) bat = %d' % \
            (self.serverStatus, self.serverMode, self.xpos, self.ypos, 
            self.theta, self.batVoltage) 

    def _handleListDrawings(self, packet):
        '''Handler for receiving drawable figures.'''

        numFigures = packet.bufToUByte4()
        print 'Number of figures: %d' % numFigures
        for i in range(numFigures):
            fig = Figure()
            fig.figureName = self._readPacketToStr(packet)
            fig.shapeID = self._readPacketToStr(packet)
            fig.colorPrimary = packet.bufToUByte4()
            fig.shapeSize = packet.bufToUByte4()
            fig.layerNum = packet.bufToUByte4()
            fig.refreshTime = packet.bufToUByte4() 
            fig.colorSecond = packet.bufToUByte4() 
            print str(fig)
            self.figures.append(fig)

    def connect(self, host, port, username='', password=''):
        '''Connects to the robot server.'''

        # Connect to the robot server
        if not self.blockingConnect(host, port, username, password):
            return False

        # Run the client as a background thread
        self.runAsync()

        # Initialize request handlers
        self.initHandlers()
        
        # Set robot name as server host name
        self.setRobotName(self.getHost())

        return True

    def initHandlers(self):
        self.addHandler('getMap', self._handleGetMap)
        self.addHandler('getGoals', self._handleGetGoals)
        self.addHandler('update', self._handleUpdate)
        self.addHandler('listDrawings', self._handleListDrawings)

    def getMap(self):
        '''Retrieve the robot's map data.'''

        result = self.request("getMap", -1)
        ArUtil_sleep(1000)
        return result

    def getGoals(self):
        '''Retrieve robot's list of goals.'''

        result = self.requestOnce('getGoals')
        ArUtil_sleep(1000)
        return result

    def getUpdates(self):
        '''Get status updates from the robot periodically.'''

        result = self.request('update', 100)
        ArUtil_sleep(1000)
        return result

    def getFigures(self):
        '''Get list of drawable figures from the robot server.'''

        result = self.requestOnce('listDrawings')
        ArUtil_sleep(1000)
        return result

    def gotoGoal(self, goalName):
        '''Go to the goal specified by \'goalName\'.'''

        result = self.requestOnceWithString('gotoGoal', goalName)
        ArUtil_sleep(1000)
        return result

    def stopRobot(self):
        '''Stops the robot.'''

        result = self.requestOnce('stop')
        return result

def initialize():
    '''Initialize ARIA data structures.'''
    Aria_init()

# -- For testing purposes only --
def myPrint(s):
    print s

def main():
    client = AriaRobotClient()
    client.connect("localhost", 7272, "guest", ".")
    client.getUpdates()

    ArUtil_sleep(5000)

if __name__ == '__main__':
    main()

