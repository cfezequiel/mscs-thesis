#include "Aria.h"
#include "ArNetworking.h"

int main(int argc, char **argv)
{
    // Must be called before other Aria functions
    // Initializes thread layer and signal handling method (Linux)
    Aria::init();

    // Base client class (ArNetworking)
    ArClientBase client;

    // Used to get options from command line
    // Default arguments can be found in /etc/Aria.args and ARIAARGS env
    // variable
    ArArgumentBuilder builder;
    builder.addPlain("-host");
    builder.addPlain("127.0.0.1");
    builder.addPlain("-user");
    builder.addPlain("guest");
    builder.addPlain("-password");
    builder.addPlain(".");
    char **args = builder.getArgv();
    for (unsigned int i = 0; i < builder.getArgc(); i++)
    {
        printf("arg[%d] = %s\n", i, args[i]);
    }

    ArArgumentParser parser(&builder);

    // Used to connect the client to a robot server given some options (parser)
    ArClientSimpleConnector clientConnector(&parser);

    // Load default options
    parser.loadDefaultArguments();
    if (!Aria::parseArgs() || !parser.checkHelpAndWarnUnparsed())
    {
        Aria::logOptions();
        exit(0);
    }

    // Connect client to remote server
    if (!clientConnector.connectClient(&client))
    {
        if (client.wasRejected())
        {
            printf("Server '%s' rejected connection, exiting\n",
                    client.getHost());
        }
        else
        {
            printf("Could not connect to server '%s', exiting\n",
                    client.getHost());
        }
        exit(1);
    }

    // Set the robot name as server host name
    client.setRobotName(client.getHost());

    // Run the client in a background thread
    client.runAsync();

    // Client will stop running when disconnected with server, or Aria shutdown
    client.disconnect();
    Aria::shutdown();

    printf("Robot client is down. Exiting program");

    return 0;
}
