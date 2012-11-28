// Test communicating with the robot server
#include <stdio.h>
#include <list>

#include "Aria.h"
#include "ArNetworking.h"
#include "ArClientHandlerMap.h"

#define N 256

int main(int argc, char **argv)
{
    Aria::init();
    ArLog::init(ArLog::StdOut, ArLog::Normal);
    ArClientBase client;

    ArArgumentParser parser(&argc, argv);

    ArClientSimpleConnector clientConnector(&parser);
    parser.loadDefaultArguments();

    if (!clientConnector.parseArgs() || !parser.checkHelpAndWarnUnparsed())
    {
        clientConnector.logOptions();
        exit(0);
    }

    if (!clientConnector.connectClient(&client))
    {
        if (client.wasRejected())
            printf("Server '%s' rejected connection, exiting\n", client.getHost());
        else
            printf("Could not connect to server '%s', exiting\n", client.getHost());
        exit(1);
    } 

    client.setRobotName(client.getHost());

    ArMap map;
    ArClientHandlerMap mapHandler(&client, &map);
    mapHandler.requestMap();
    client.runAsync();

    while (!mapHandler.mapReceived())
    {
        ArUtil::sleep(500);
    }

    printf("Map: Number of lines = %d\n", map.getNumLines());

    Aria::shutdown();

    return 0;
}

