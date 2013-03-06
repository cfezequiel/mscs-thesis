#ifndef ARCLIENT_H
#define ARCLIENT_H

#include <string>
#include <sstream>
#include <list>

#include "Aria.h"
#include "ArNetworking.h"
#include "mapdata.h"

using namespace std;

class ArClient : public ArClientBase
{

public:
    ArClient();
    ~ArClient();
    bool connect(const char *host, int port, const char *username=NULL, const char *password=NULL);
    bool mapReceived() { return _mapReceived; }
    ArMap * getMapFromServer();
    void getDrawings();
    list<string> * listCommands();
    ArConfig * getConfig();
    void sendMap(ArMap *map);
    void getUpdates(int frequency);
    void setMapFileConfigOnServer(const char *filename);
    void stop();
    void goToGoal(const char *goalName);
    void goHome();
    void resume();

protected:
    virtual void updateNumbersReceived(ArRobotInfo robotInfo) {}
    virtual void updateStringsReceived(ArRobotInfo robotInfo) {}
    virtual void getPathReceived(Points *path) {}
    virtual void serverShutdown() {}
    virtual void connectionError() {}

private:
    // Config handler
    ArClientHandlerConfig *_configHandler;

    // File handlers
    ArClientFileFromClient *_clientFileFromClient;
    ArClientFileLister *_clientFileLister;

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
    ArFunctor1C<ArClient, ArNetPacket *> *_updateNumbersCB;
    ArFunctor1C<ArClient, ArNetPacket *> *_updateStringsCB;
    void _handleUpdateNumbers(ArNetPacket *packet);
    void _handleUpdateStrings(ArNetPacket *packet);
    ArRobotInfo _robotInfo;
    string _lastMode; // for resuming prevous action after stop()

    // Robot path handler
    ArFunctor1C<ArClient, ArNetPacket *> *_getPathCB;
    void _handleGetPath(ArNetPacket *packet);
    Points _path;

    // Goal
    string _currentGoal;

    // Disconnect handler
    ArFunctorC<ArClient> *_disconnectOnErrorCB;
    ArFunctorC<ArClient> *_serverShutdownCB;
};

#endif // ARCLIENT_H
