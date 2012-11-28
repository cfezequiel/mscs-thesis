#include <assert.h>
#include <cstdlib>
#include <sstream>
#include <iostream>

#include "ArClientHandlerMap.h"

ArClientHandlerMap::ArClientHandlerMap(ArClientBase *client, ArMap *map)
{
    assert(client != NULL);
    assert(map != NULL);

    _client = client;
    _map = map;

    ArFunctor1<ArNetPacket *> requestMapHandlerCB(&_requestMapHandler);
    client.addHandler("getMap", &requestMapHandlerCB);
    _allMapDataReceived = false;

    // Set temporary map file name
    stringstream ss;
    ss << "tmp_" << rand();
    _mapFileName.assign(ss.str());
}

void ArClientHandlerMap::requestMapHandler(ArNetPacket *packet)
{
    // Get packet data length
    int packetDataLength = packet->getDataLength(); 

    // Read data
    char *data = new char[packetDataLength];
    packet->bufToData(data, packetDataLength);

    // Write data to file
    mapFile.open(_mapFileName.c_str(), fstream::out, fstream::app);
    if (!_mapFile)
    {
        ArLog::log(ArLog::Normal, "Could not open temporary map file");
        return;
    }
    _mapFile << data;
    mapFile.close();

    // If all map data has been received, read data into the ArMap object
    if (data[0] == '\0' && _allMapDataReceived = false)
    {
        _allMapDataReceived = true;

        _map->readFile(_mapFileName.c_str());

        // Delete map file
        if (remove(_mapFileName) == -1)
        {
            ArLog::log(ArLog::Normal, "Could not delete temporary map file");
            return;
        }

        // Assign new temporary map file name
        stringstream ss;
        ss << "tmp_" << rand();
        _mapFileName.assign(ss.str());
    }
}

void ArClientHandlerMap::requestMap()
{
    _allMapDataReceived = false;

    if (!_client->dataExists("getMap"))
    {
        ArLog::log(ArLog::Normal, "Server does not support 'getMap'");
        return;
    }
        
    if (!_client->request("getMap", -1))
    {
        ArLog::log(ArLog::Normal, "Request for map failed.");
        return;
    }
}

bool ArClientHandlerMap::mapReceived()
{
    return _allMapDataReceived;
}


