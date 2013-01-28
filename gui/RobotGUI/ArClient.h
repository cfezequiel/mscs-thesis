#ifndef ARCLIENT_H
#define ARCLIENT_H

#include <sstream>

#include "Aria.h"
#include "ArNetworking.h"

using namespace std;

class ArClient : public ArClientBase
{
public:
    ArClient();
    bool connect(char *host, int port, char *username, char *password);
    void getRobotInfo();
    bool mapReceived() { return _mapReceived; }
    ArMap * getMap();
    void getDrawings();
    void getUpdates(int frequency);

private:
    ArClientHandlerConfig *_configHandler;
    ArClientFileFromClient *_clientFileFromClient;
    void _handleGetMap(ArNetPacket *packet);
    ArFunctor1C<ArClient, ArNetPacket *> _getMapCB;
    bool _mapReceived;
    stringstream _strbuf;
};

#endif // ARCLIENT_H
