#include <cstdio>
#include <fstream>
#include <iostream>
#include "arclient.h"

using namespace std;

ArClient::ArClient() :
    ArClientBase(),
    _getMapCB(this, &ArClient::_handleGetMap)
{
    _mapReceived = false;
}

bool ArClient::connect(char *host, int port, char *username, char *password)
{
    // Connect to server
    bool result = blockingConnect(host, port, username, password);
    if (!result)
    {
        cout << "Connection to " << host << " failed." << endl;
        return result;
    }

    // Add robot config handler
    _configHandler = new ArClientHandlerConfig(this);

    // Add file transfer handler
    _clientFileFromClient = new ArClientFileFromClient(this);

    // Add map handler
    addHandler("getMap", &_getMapCB);

    // Run client
    runAsync();

    return true;
}

void ArClient::getRobotInfo()
{
    request("update", -1);
}

ArMap * ArClient::getMap()
{
    if (!request("getMap", -1))
    {
        cerr << "Error: Map request failed." << endl;
    }

    // Do some housekeeping
    _mapReceived = false;
    _strbuf.str("");

    while (!_mapReceived)
    {
        ArUtil::sleep(100);
    }

    // Save map strings to a file
    char filename[] = "tmp.map";
    ofstream outFile(filename, ios::out);
    outFile << _strbuf.rdbuf();
    outFile.close();

    // Load map
    ArMap *map = new ArMap();
    if (!map->readFile(filename))
    {
        cerr << "Error: Unable to load map file." << endl;
        delete map;
        return NULL;
    }

    // Remove temporary map file
    if (remove(filename) != 0)
    {
        cerr << "Error: Unable to remove temporary map file." << endl;
    }

    return map;
}

void ArClient::_handleGetMap(ArNetPacket *packet)
{
    if (_mapReceived)
    {
        return;
    }

    char data[1024];
    int length = packet->getDataLength();
    packet->bufToData(data, length);

    cout << "RCVD: " << data << endl;

    _strbuf << data << '\n';

    // All map data received
    if (data[0] == '\0')
    {
        _mapReceived = true;
    }
}

