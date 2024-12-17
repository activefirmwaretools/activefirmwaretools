#include <QTcpSocket>
#include <qthread.h>
#include <unistd.h>

#define ACTIVE_TCP_PORT 37800

void SendCommand( QTcpSocket *socket, const char *str)
{
    QString command(str);
    qDebug() << "Command:  " << command;                // Print the command we are sending to the console window

    socket->write(str);           // Send a Command (must be \n terminated)
    socket->waitForBytesWritten();  // Wait for the command to be sent
}

QString ReadResponse( QTcpSocket *socket )
{
    QString response;

    if (!socket->canReadLine())             // If we have not received the entire response yet,
        socket->waitForReadyRead(1000);     // Wait to receive the response

    response = socket->readLine();          // Read the entire response including the '\n'
    qDebug() << "Response: " << response;   // Print the response we received to the console window

    return (response);                      // Return the response

}

int main(int argc, char const* argv[])
{

    QTcpSocket socket;

    socket.connectToHost("localhost", ACTIVE_TCP_PORT); // Connect to the Active-Pro Application

    if (socket.waitForConnected()) {                    // Wait for the connection to be initialized

        // Determining if we are connected to the Active-Pro Application and the Active-Pro Hardware Pod

        SendCommand( &socket, "Hello\n");               // See if we are talking OK to the Active-Pro Application
        ReadResponse( &socket );
        SendCommand( &socket, "isConnected\n");         // See if the Active-Pro Application is connected to the Active-Pro Pod
        ReadResponse( &socket );

        // Opening and Closing the Tabbed Windows on the right side of the application

        SendCommand( &socket, "ShowInputs\n");          // Show the Inputs Window in the Active-Pro App
        ReadResponse( &socket );
        SendCommand( &socket, "ShowList\n");            // Show the List Window in the Active-Pro App
        ReadResponse( &socket );
        SendCommand( &socket, "ShowSettings\n");        // Show the Settings Window in the Active-Pro App
        ReadResponse( &socket );
        SendCommand( &socket, "ShowNotes\n");           // Show the Notes Window in the Active-Pro App
        ReadResponse( &socket );
        SendCommand( &socket, "ShowOutputs\n");         // Show the Outputs Window in the Active-Pro App
        ReadResponse( &socket );
        SendCommand( &socket, "CloseTabs\n");           // Close any open tabs in the Active-Pro App
        ReadResponse( &socket );

        // Setting Outputs A0, A1, D0 and D1

        SendCommand( &socket, "SetD0Mode 0\n");         // Set the D0 Output level to Tristate
        ReadResponse( &socket );
        SendCommand( &socket, "SetD0Mode 1\n");         // Set the D0 Output level to 0V
        ReadResponse( &socket );
        SendCommand( &socket, "SetD0Mode 2\n");         // Set the D0 Output level to 3.3V
        ReadResponse( &socket );
        SendCommand( &socket, "SetD0Mode 3\n");         // Set the D0 Output level to PWM Mode
        ReadResponse( &socket );
        SendCommand( &socket, "SetD0PWM 25\n");         // Set the D0 Output PWM Percentage to 25% high
        ReadResponse( &socket );
        SendCommand( &socket, "SetD0PWM 75\n");         // Set the D0 Output PWM Percentage to 75% high
        ReadResponse( &socket );

        SendCommand( &socket, "SetD1Mode 0\n");         // Set the D1 Output level to Tristate
        ReadResponse( &socket );
        SendCommand( &socket, "SetD1Mode 1\n");         // Set the D1 Output level to 0V
        ReadResponse( &socket );
        SendCommand( &socket, "SetD1Mode 2\n");         // Set the D1 Output level to 3.3V
        ReadResponse( &socket );
        SendCommand( &socket, "SetD1Mode 3\n");         // Set the D1 Output level to PWM Mode
        ReadResponse( &socket );
        SendCommand( &socket, "SetD1PWM 25\n");         // Set the D1 Output level PWM Percentage to 25% high
        ReadResponse( &socket );
        SendCommand( &socket, "SetD1PWM 75\n");         // Set the D1 Output level PWM Percentage to 75% high
        ReadResponse( &socket );

        SendCommand( &socket, "SetA0Mode 0\n");         // Set the A0 Output level to Tristate
        ReadResponse( &socket );
        SendCommand( &socket, "SetA0Mode 1\n");         // Set the A0 Output level to 0V
        ReadResponse( &socket );
        SendCommand( &socket, "SetA0Mode 2\n");         // Set the A0 Output level to 1V
        ReadResponse( &socket );
        SendCommand( &socket, "SetA0Mode 3\n");         // Set the A0 Output level to 2V
        ReadResponse( &socket );
        SendCommand( &socket, "SetA0Mode 4\n");         // Set the A0 Output level to 3V
        ReadResponse( &socket );
        SendCommand( &socket, "SetA0Mode 5\n");         // Set the A0 Output level to 3.3V
        ReadResponse( &socket );
        SendCommand( &socket, "SetA0Mode 6\n");         // Set the A0 Output level to DC
        ReadResponse( &socket );
        SendCommand( &socket, "SetA0DCLEVEL 0.5\n");    // Set the A0 Output DC level to 0.5V
        ReadResponse( &socket );
        SendCommand( &socket, "SetA0DCLEVEL 1.5\n");    // Set the A0 Output DC level to 1.5V
        ReadResponse( &socket );
        SendCommand( &socket, "SetA0DCLEVEL 2.5\n");    // Set the A0 Output DC level to 2.5V
        ReadResponse( &socket );


        SendCommand( &socket, "SetA1Mode 1\n");         // Set the A1 Output level to 0V
        ReadResponse( &socket );
        SendCommand( &socket, "SetA1Mode 2\n");         // Set the A1 Output level to 1V
        ReadResponse( &socket );
        SendCommand( &socket, "SetA1Mode 3\n");         // Set the A1 Output level to 2V
        ReadResponse( &socket );
        SendCommand( &socket, "SetA1Mode 4\n");         // Set the A1 Output level to 3V
        ReadResponse( &socket );
        SendCommand( &socket, "SetA1Mode 6\n");         // Set the A1 Output level to DC
        ReadResponse( &socket );
        SendCommand( &socket, "SetA1DCLEVEL 0.5\n");    // Set the A1 Output DC level to 0.5V
        ReadResponse( &socket );
        SendCommand( &socket, "SetA1DCLEVEL 1.5\n");    // Set the A1 Output DC level to 1.5V
        ReadResponse( &socket );
        SendCommand( &socket, "SetA1Mode 7\n");         // Set the A1 Output to a Ramp waveform
        ReadResponse( &socket );
        SendCommand( &socket, "SetA1MINIMUM 0.5\n");    // Set the A1 waveform minimum to 0.5V
        ReadResponse( &socket );
        SendCommand( &socket, "SetA1MAXIMUM 2.5\n");    // Set the A1 waveform maximum to 2.5V
        ReadResponse( &socket );
        SendCommand( &socket, "SetA1Mode 8\n");         // Set the A1 Output to a Sine waveform
        ReadResponse( &socket );
        SendCommand( &socket, "SetA1Mode 9\n");         // Set the A1 Output to a Square waveform
        ReadResponse( &socket );
        SendCommand( &socket, "SetA1Mode 10\n");        // Set the A1 Output to a Triangle waveform
        ReadResponse( &socket );
        SendCommand( &socket, "SetA1Steps 4000\n");     // Set the A1 Output waveform to be 4000 steps per cycle (16.17msecs).  Each step is 4usecs.
        ReadResponse( &socket );
        SendCommand( &socket, "SetA1Steps 500\n");      // Set the A1 Output waveform to be 500 steps per cycle (2msecs). Each step is 4usecs.
        ReadResponse( &socket );

        // Capture Control

        SendCommand( &socket, "isCapturing\n");         // Check if the Active-Pro is capturing data
        ReadResponse( &socket );
        SendCommand( &socket, "STARTCAPTURE\n");        // Start a capture
        ReadResponse( &socket );
        SendCommand( &socket, "GetLogic\n");            // Get the current Logic Inputs (0-255 each bit is a channel ie. bit 0 = channel 0 digital level)
        ReadResponse( &socket );
        SendCommand( &socket, "GetCH1\n");              // Get the current voltage on analog CH1 in volts
        ReadResponse( &socket );
        SendCommand( &socket, "GetCH2\n");              // Get the current voltage on analog CH2 in volts
        ReadResponse( &socket );
        SendCommand( &socket, "GetCH3\n");              // Get the current voltage on analog CH3 in volts
        ReadResponse( &socket );
        SendCommand( &socket, "STOPCAPTURE\n");         // Stop the capture
        ReadResponse( &socket );
        SendCommand( &socket, "GetCaptureSize\n");      // How big is the capture in bytes
        ReadResponse( &socket );
        SendCommand( &socket, "GetCaptureTime\n");      // How long is the capture in seconds
        ReadResponse( &socket );

        // Clearing and Entering Notes into the Notes Window

        SendCommand( &socket, "ShowNotes\n");           // Show the Notes Window in the Active-Pro App
        ReadResponse( &socket );
        SendCommand( &socket, "ClearNote\n");           // Clear out the notes in the Notes Window
        ReadResponse( &socket );
        SendCommand( &socket, "AppendNote Sent to the Active-Pro Application.\n");     // Add text to the Notes Window
        ReadResponse( &socket );
        SendCommand( &socket, "AppendNote \n");         // Add a blank line to the notes window
        ReadResponse( &socket );
        SendCommand( &socket, "AppendNote And here is more data.\n");   // Add more text to the notes window
        ReadResponse( &socket );

        // Zooming the trace window

        SendCommand( &socket, "ZOOMALL\n");             // Zoom the waveforms to see all of the capture
        ReadResponse( &socket );
        SendCommand( &socket, "ZoomFrom 1.0 2.0\n");    // Zoom the waveforms from 1s to 2s
        ReadResponse( &socket );

        // Searching for data

        SendCommand( &socket, "SetCursorCurrent 1\n");  // Place the Current Cursor at time 1 seconds.  This cursor is where the searching starts from.
        ReadResponse( &socket );
        SendCommand( &socket, "Search mon\n");          // Find "mon" in the trace starting from the Current Cursor.  Returns the time that it was found.
        ReadResponse( &socket );
        SendCommand( &socket, "Search booga\n");        // Find "booga" in the trace.  If not found it returns NOTFOUND
        ReadResponse( &socket );

        // Setting Cursor Positions

        SendCommand( &socket, "SetCursorCurrent 3\n");  // Place the Current cursor at time 3.0 seconds
        ReadResponse( &socket );
        SendCommand( &socket, "SetCursorX1 0\n");       // Place the X1 cursor at time 0.0 seconds
        ReadResponse( &socket );
        SendCommand( &socket, "SetCursorX2 5.0\n");     // Place the X2 cursor at time 5.0 seconds
        ReadResponse( &socket );

        // File Operations

        SendCommand( &socket, "ExportBetweenCursors C:\\APITesting\\test.csv\n");   // Export all decoded traffic between the X1 and X2 cursors to a file (csv format)
        ReadResponse( &socket );
        SendCommand( &socket, "SaveCapture C:\\APITesting\\testsave.active\n");   // Export all decoded traffic between the X1 and X2 cursors to a file (csv format)
        ReadResponse( &socket );
        SendCommand( &socket, "SaveBetweenCursors C:\\APITesting\\testsavebetweencursors.active\n");   // Export all decoded traffic between the X1 and X2 cursors to a file (csv format)
        ReadResponse( &socket );
        SendCommand( &socket, "SaveConfiguration C:\\APITesting\\testsaveconfig.active\n");   // Export all decoded traffic between the X1 and X2 cursors to a file (csv format)
        ReadResponse( &socket );
        SendCommand( &socket, "OpenConfiguration C:\\APITesting\\testsaveconfig.active\n");   // Export all decoded traffic between the X1 and X2 cursors to a file (csv format)
        ReadResponse( &socket );
        SendCommand( &socket, "SaveScreenshot C:\\APITesting\\testclosed.png\n");   // Take a screenshot of the Active-Pro App window and save it to a file
        ReadResponse( &socket );
        SendCommand( &socket, "NewCapture\n");          // Reset all settings and clear any existing capture data
        ReadResponse( &socket );
        SendCommand( &socket, "OpenCapture C:\\APITesting\\testsave.active\n");     // Open a capture file that was previously saved
        ReadResponse( &socket );
        SendCommand( &socket, "Exit\n");          // Closes the Active-Pro Application
        ReadResponse( &socket );


        socket.close();                                     // Close the connection
    }
}
