// This program accesses the TCP Socket to the Active-Pro API so people dom't need to write code.
//
// This works with Active-Pro App version 4.0 and above
//
// Each call simply:
//      - Open the connection to the app
//      - Sends the Command
//      - Receives the response
//      - Writes the response to the commandline output (stdio)
//      - Closes the connection
//      - Exits
//

#include <QTcpSocket>
#include <QtCore>

#define ACTIVE_TCP_PORT_DEVICE_1 37800
#define ACTIVE_TCP_PORT_DEVICE_2 37801

QTcpSocket socket;

void SendCommand( QTcpSocket *socket, QString QStr)
{
    const char *str = QStr.toLocal8Bit().constData();

    socket->write(str);           // Send a Command (must be \n terminated)
    socket->waitForBytesWritten();  // Wait for the command to be sent
}

QString ReadResponse( QTcpSocket *socket)
{
    QString response;

    socket->waitForReadyRead(10);
    response = socket->readAll();

    while (!response.contains("\n"))             // If we have not received the entire response yet,
    {
        socket->waitForReadyRead(10);
        response +=socket->readAll();
    }

    return (response);                      // Return the response
}

int main(int argc, char * argv[])
{
    QCoreApplication a(argc, argv);

    QTextStream out(stdout);        // Send the output to stdout

    QString Command;
    int device = 1;     // Default to device 1
    int port = ACTIVE_TCP_PORT_DEVICE_1;

    bool throwhelpscreen = 0;

    // Validate the command line arguments
    if ((argc < 2) || (argc > 3) || ((argc == 2) && (strcmpi(argv[1], "-?")) == 0))
    {
        throwhelpscreen = 1;
    }
    else if (argc == 2)
    {
        // No port number so just get the command
        device = 1;     // Default to device 1
        port = ACTIVE_TCP_PORT_DEVICE_1;
        Command = QString::fromLatin1(argv[1]);
    }
    else if (argc == 3)
    {
        // The port is specified
        if (strcmpi(argv[1], "1") == 0)
        {
            port = ACTIVE_TCP_PORT_DEVICE_1;
            device = 1;
            Command = QString::fromLatin1(argv[2]);
        }
        else if (strcmpi(argv[1], "2") == 0)
        {
            port = ACTIVE_TCP_PORT_DEVICE_2;
            device = 2;
            Command = QString::fromLatin1(argv[2]);
        }
        else
        {
            throwhelpscreen = 1;
        }
    }

    if (throwhelpscreen)
    {
        out << "ActiveAPICommandLine version 1.1\n";
        out << "Usage: activeapicommandline [optional device number] \"command with parameters\"\n";
        out << "       [optional device number] is 1 or 2.  Device number 1 is default if not provided.\n";
        out << "       command is the command to send and must be encloded in quotes.\n";
        out << "       Responses will be written to the console output (stdio).\n";
        out << "       Commands and Responses are available at http://www.activefirmwaretools.com\n";

        return (0);
    }

    socket.connectToHost("localhost", port);

    if (socket.waitForConnected(5000)) {                // Wait for the connection to be initialized

        SendCommand( &socket, Command + "\n");          // Append a new line

        QString response = ReadResponse( &socket );

        out << response;      // The responses already have a new line

        socket.disconnectFromHost();
        if (socket.state() != QAbstractSocket::UnconnectedState)
            socket.waitForDisconnected(1000);

        socket.close();                               // Close the connection
    }
    else
    {
       out << "Can not connect to device " << device << " on port " << port << "\n";
    }

    return (0);
}
