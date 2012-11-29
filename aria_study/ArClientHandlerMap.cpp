#include <assert.h>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <iostream>

#include "ArClientHandlerMap.h"

ArClientHandlerMap::ArClientHandlerMap(ArClientBase *client, ArMap *map)
{
    assert(client != NULL);
    assert(map != NULL);

    _client = client;
    _map = map;

    _requestMapHandlerCB = new ArFunctor1C<ArClientHandlerMap, ArNetPacket *>(this,
                &ArClientHandlerMap::_requestMapHandler);
    client->addHandler("getMap", _requestMapHandlerCB);
    _allMapDataReceived = false;
}

void ArClientHandlerMap::_requestMapHandler(ArNetPacket *packet)
{
    assert(_mapFile != NULL);

    // Get packet data length
    int packetDataLength = packet->getDataLength(); 

    // Read data
    char *data = new char[packetDataLength];
    packet->bufToData(data, packetDataLength);

    // Write data to file
    fprintf(_mapFile, "%s\n", data);

    // If all map data has been received, read data into the ArMap object
    if (data[0] == '\0' && _allMapDataReceived == false)
    {
        _allMapDataReceived = true;

        // Close the file
        fclose(_mapFile);

        // Store map data in appropriate data structure
        _map->readFile(_mapFileName);

        // Delete temporary map file
        if (remove(_mapFileName) == -1)
        {
            ArLog::log(ArLog::Normal, "Could not delete temporary map file");
            return;
        }
    }
}

void ArClientHandlerMap::requestMap()
{
    _allMapDataReceived = false;

    // Create temporary file to store map data
    snprintf(_mapFileName, MAXLEN, "tmp_%d.map", rand());
    _mapFile = fopen(_mapFileName, "w");

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


