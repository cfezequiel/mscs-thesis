#ifndef ARCLIENT_H
#define ARCLIENT_H

#include <string>
#include <sstream>

#include "Aria.h"
#include "ArNetworking.h"

using namespace std;

static const int STRLEN = 256;
struct ArRobotInfo
{
    ArTypes::Byte2 batVoltage;
    ArTypes::Byte4 xpos;
    ArTypes::Byte4 ypos;
    ArTypes::Byte2 theta;
    ArTypes::Byte2 forwardVel;
    ArTypes::Byte2 rotationVel;
    char status[STRLEN];
    char mode[STRLEN];
};

class ArClient : public ArClientBase
{
public:
    ArClient();
    ~ArClient();
    bool connect(char *host, int port, char *username=NULL, char *password=NULL);
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
    void _handleUpdateNumbers(ArNetPacket *packet);
    ArFunctor1C<ArClient, ArNetPacket *> *_updateNumbersCB;
    void _handleUpdateStrings(ArNetPacket *packet);
    ArFunctor1C<ArClient, ArNetPacket *> *_updateStringsCB;
    ArRobotInfo _robotInfo;
};

#endif // ARCLIENT_H
