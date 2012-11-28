#ifndef ARCLIENTHANDLERMAP_H
#define ARCLIENTHANDLERMAP_H

#include <fstream>
#include <string>

#include "Aria.h"
#include "ArNetworking.h"
using namespace std;

class ArClientHandlerMap
{
public:
    ArClientHandlerMap(ArClientBase *client, ArMap *map);
    void requestMap(void);
    bool mapReceived();

private:
    ArClientBase *_client;
    ArMap *_map;

    bool _allMapDataReceived;
    void _requestMapHandler(ArNetPacket *packet);
    fstream _mapFile;
    string _mapFileName;
};


#endif /* ARCLIENTHANDLERMAP_H */ 
