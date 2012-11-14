#include "ariarobot.h"

AriaRobot::AriaRobot()
{
    // do nothing
    // TODO: put guards on ARIA initialization so that it is only invoked
    // once
    Aria::init();
}

AriaRobot::~AriaRobot()
{
    // FIXME : this should be done only when last robot is being destroyed
    Aria::shutdown();
}

void AriaRobot::connect(string username, string password, string server)
{
    client = new ArClientBase;

    ArArgumentBuilder builder;
    builder.addPlain("-host");
    builder.addPlain(server.c_str());
    builder.addPlain("-user");
    builder.addPlain(username.c_str());
    builder.addPlain("-password");
    builder.addPlain(password.c_str());

    ArArgumentParser parser(&builder);

    ArClientSimpleConnector clientConnector(&parser);

    // Connect Aria client to remote server
    if (!clientConnector.connectClient(client))
    {
        if (client->wasRejected())
        {
            printf("Server '%s' rejected connection, exiting\n",
                   client->getHost());
        }
        else
        {
            printf("Could not connect to server '%s', exiting\n",
                   client->getHost());
        }
        return;
    }

    // Set the robot name as server host name
    client->setRobotName(client->getHost());

    // Run the client in a background thread
    client->runAsync();

    // TODO: find way to keep the client 'alive'

    return;
}

void AriaRobot::disconnect()
{
    client.disconnect();
}
