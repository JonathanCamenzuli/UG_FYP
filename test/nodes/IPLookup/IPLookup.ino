/**
 * @file IPLookup.ino
 *
 * @author Jonathan Camenzuli
 *
 * @brief IP Lookup Test
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 06/03/2023
 *
 * @section libraries Libraries
 * - MKRNB by Arduino Libraries (https://github.com/arduino-libraries/MKRNB)
 *
 * @section hardware Hardware
 * -  Arduino Nano
 */

#include <MKRNB.h>
#include "arduino_secrets.h"

// PIN Number
char pinnumber[] = SECRET_PINNUMBER;
char apn[] = SECRET_GPRS_APN;

// URL, endpoint and port
char server[] = SECRET_HOSTNAME;
char endpoint[] = "/";
uint32_t httpPort = SECRET_HTTP_PORT;

// initialize the library instance
NBClient client;
GPRS gprs;
NB nbAccess;

void setup()
{
    // initialize serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    Serial.println("Starting Arduino web client.");
    // connection state
    boolean connected = false;

    // After starting the modem with NB.begin()
    // attach to the GPRS network with the APN, login and password
    while (!connected)
    {
        if ((nbAccess.begin(pinnumber, apn) == NB_READY) && (gprs.attachGPRS() == GPRS_READY))
        {
            connected = true;
            Serial.println("Connected to ISP");
        }
        else
        {
            Serial.println("Not connected");
            delay(1000);
        }
    }

    Serial.println("connecting...");

    // if you get a connection, report back via serial:
    if (client.connect(server, httpPort))
    {
        Serial.println("connected");
        // Make a HTTP request:
        client.print("GET ");
        client.print(endpoint);
        client.println(" HTTP/1.1");
        client.print("Host: ");
        client.println(server);
        client.println("Connection: close");
        client.println();
    }
    else
    {
        // if you didn't get a connection to the server:
        Serial.println("connection failed");
    }
}

void loop()
{
    // if there are incoming bytes available
    // from the server, read them and print them:
    if (client.available())
    {
        String content;
        while (client.available())
        {
            content += (char)client.read();
        }

        // Find the index of the last newline character
        int lastNewlineIndex = -1;
        for (int i = content.length() - 1; i >= 0; i--)
        {
            if (content[i] == '\n')
            {
                lastNewlineIndex = i;
                break;
            }
        }

        if (lastNewlineIndex != -1)
        {
            // Extract the substring from the last newline character to the end of the string
            String lastLine = content.substring(lastNewlineIndex + 1);
            Serial.println(lastLine);
        }
    }

    // if the server's disconnected, stop the client:
    if (!client.available() && !client.connected())
    {
        Serial.println();
        Serial.println("disconnecting.");
        client.stop();

        // do nothing forevermore:
        for (;;)
            ;
    }
}