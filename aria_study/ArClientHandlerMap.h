#ifndef ARCLIENTHANDLERMAP_H
#define ARCLIENTHANDLERMAP_H

#include <string>

#include "Aria.h"
#include "ArNetworking.h"
using namespace std;

static const int MAXLEN = 256;

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
    ArFunctor1C<ArClientHandlerMap, ArNetPacket *> *_requestMapHandlerCB;
    FILE *_mapFile;
    char _mapFileName[MAXLEN];
};


#endif /* ARCLIENTHANDLERMAP_H */ 
