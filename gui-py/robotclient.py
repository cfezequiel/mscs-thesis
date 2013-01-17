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
            
            d = self.deferreds['map']
            d.callback()

    def _handleGetGoals(self, packet):
        '''Handler for receiving list of goals from robot server.'''

        goals = self._readPacketToStrList(packet)

        # Get rid of last element which is ''
        goals = goals[:-1]

        d = self.deferreds['goals']
        d.callback(goals)

    def _handleUpdate(self, packet):
        '''Handler for receiving robot status updates.'''

        self.serverStatus = self._readPacketToStr(packet)
        self.serverMode = self._readPacketToStr(packet)
        self.batVoltage = packet.bufToUByte2() 
        self.xpos = float(packet.bufToByte4())
        self.ypos = float(packet.bufToByte4())
        self.theta = float(packet.bufToByte2())

#print '%s, %s @ (%d, %d, %d) bat = %d' % \
#            (self.serverStatus, self.serverMode, self.xpos, self.ypos,
#             self.theta, self.batVoltage)

    def _handleListDrawings(self, packet):
        '''Handler for receiving drawable figures.'''

        numFigures = packet.bufToUByte4()
        print 'Number of figures: %d' % numFigures
        figures = []
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
            figures.append(fig)

        d = self.deferreds['figures']
        d.callback(figures)
        return d

    def connect(self, host, port, username='', password=''):
        '''Connects to the robot server.'''

        d = defer.Deferred()
        server = (host, str(port))

        # Connect to the robot server
        if not self.blockingConnect(host, port, username, password):
            s = ":".join(server)
            print s
            d.errback(Exception("Could not connect to server (%s)" % s))
            return d

        # Run the client as a background thread
        self.runAsync()

        # Initialize request handlers
#self.initHandlers()
        
        # Set robot name as server host name
        self.setRobotName(self.getHost())

        # Request robot status updates periodically
        self.request('update', 100)

        d.callback(self)
        return d

    def initHandlers(self):
        self.addHandler('getMap', self._handleGetMap)
        self.addHandler('getGoals', self._handleGetGoals)
        self.addHandler('update', self._handleUpdate)
        self.addHandler('listDrawings', self._handleListDrawings)

    def getRobotMap(self):
        '''Retrieve the robot's map data.'''

        d = defer.Deferred()
        self.deferreds['map'] = d

        if self.dataExists("getMap"):
            print 'Requesting map data...' 
            if not self.request("getMap", -1):
                print "Request for map failed."
        else:
            print "Robot server does not support 'getMap'"

        ArUtil_sleep(1000)

        self.gotMap = False

        return d

    def getRobotGoals(self):
        '''Retrieve robot's list of goals.'''

        d = defer.Deferred()
        self.deferreds['goals'] = d

        if self.dataExists('getGoals'):
            if not self.requestOnce('getGoals'):
                print 'Request for goals list failed'
                return d.errback('Error: request')
        else:
            print 'Robot server does not support \'getGoals\''
            return d.errback('Error: does not exist')

        ArUtil_sleep(1000)

        return d

    def getFigures(self):
        '''Get list of drawable figures from the robot server.'''

        d = defer.Deferred()
        self.deferreds['figures'] = d
        self.requestOnce('listDrawings')
        ArUtil_sleep(1000)
        return d

    def gotoGoal(self, goalName):
        '''Go to the goal specified by \'goalName\'.'''

        self.requestOnceWithString('gotoGoal', goalName)
        ArUtil_sleep(1000)

    def stopRobot(self):
        '''Stops the robot.'''

        self.requestOnce('stop')

def initialize():
    '''Initialize ARIA data structures.'''
    Aria_init()

# -- For testing purposes only --
def myPrint(s):
    print s

def main():
    client = AriaRobotClient()
    d = client.connect("localhost", 7272, "guest", ".")
    d.add_callback(lambda _: client.getRobotMap())
    d.add_callback(lambda _: client.getFigures())
    d.add_callback(lambda _: client.getRobotGoals())
    d.add_callback(lambda goals: client.gotoGoal(random.choice(goals)))
    d.add_callback(lambda _: ArUtil_sleep(2000))
    d.add_callback(lambda _: client.stopRobot())
    d.add_callback(lambda _: client.disconnect())
    d.add_callback(lambda _: Aria_exit(0))
    return d

if __name__ == '__main__':
    main()

