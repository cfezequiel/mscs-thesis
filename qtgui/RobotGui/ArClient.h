#ifndef ARCLIENT_H
#define ARCLIENT_H

#include <string>
#include <sstream>

#include "Aria.h"
#include "ArNetworking.h"

using namespace std;

class ArClient : public ArClientBase
{
public:
    ArClient();
    ~ArClient();
    bool connect(char *host, int port, char *username=NULL, char *password=NULL);
    void getRobotInfo();
    bool mapReceived() { return _mapReceived; }
    ArMap * getMap();
    void getDrawings();
    list<string> * listCommands();
    ArConfig * getConfig();
    void sendMap(ArMap *map);
    void getUpdates(int frequency);

private:
    ArClientHandlerConfig *_configHandler;
    ArClientFileFromClient *_clientFileFromClient;

    // Map handler
    void _handleGetMap(ArNetPacket *packet);
    ArFunctor1C<ArClient, ArNetPacket *> *_getMapCB;
    bool _mapReceived;
    stringstream _strbuf;

    // List commands handler
    void _handleListCommands(ArNetPacket *packet);
    ArFunctor1C<ArClient, ArNetPacket *> *_listCommandsCB;
    unsigned int _nCommands; 
    list<string> _strListBuf;
    bool _commandsReceived;

    // Update handler
    void _handleUpdate(ArNetPacket *packet);
    ArFunctor1C<ArClient, ArNetPacket *> *_updateCB;
};

#endif // ARCLIENT_H
