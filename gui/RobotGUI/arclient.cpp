#include <iostream>
#include "arclient.h"

ArClient::ArClient() : ArClientBase()
{
    // Do nothing
}

bool ArClient::connect(char *host, int port, char *username, char *password)
{
    // Connect to server
    bool result = blockingConnect(host, port, username, password);
    if (!result)
    {
        cout << "Connection to " << host << " failed." << endl;
        return result
    }

    // Add robot config handler
    configHandler = new ArClientHandlerConfig(this);

    // Add file transfer handler
    clientFileFromClient = new ArClientFileFromClient(this);

    // Add map handler
    ArFunctor1<ArNetPacket *> getMapCB(this->handleGetMap);

    // Run client
    client.runAsync();
}

void getRobotInfo()
{
    request("update", -1);
}
