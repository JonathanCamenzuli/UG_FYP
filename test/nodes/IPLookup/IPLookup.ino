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
 * @section hardware Hardware
 * -  Arduino MKR NB 1500
 */

#include <MKRNB.h>
#include <ArduinoHttpClient.h>
#include "IPLookup.h"
#include "arduino_secrets.h"

char response;

// PIN Number
char pinnumber[] = SECRET_PINNUMBER;
char apn[] = SECRET_GPRS_APN;

// URL, endpoint and port
char server[] = SECRET_HOSTNAME;
char endpoint[] = "/";
uint32_t httpPort = SECRET_HTTP_PORT;

// Initialise any Library Instances
NBClient nbClient;
HttpClient httpClient = HttpClient(nbClient, server, httpPort);
GPRS gprs;
NB nbAccess;

IPAddress coapServer_ip;

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
}

void loop()
{
  getIPAddress(coapServer_ip, httpClient);
  Serial.println(coapServer_ip);

  for (;;)
    ;
}