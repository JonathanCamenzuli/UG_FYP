/**
 * @file CoapTest.ino
 *
 * @author Jonathan Camenzuli
 *
 * @brief CoAP Test
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 15/03/2023
 *
 * @section hardware Hardware
 * -  Arduino MKR NB 1500
 */

#include <MKRNB.h>
#include <ArduinoJson.h>
#include <coap-simple.h>
#include "arduino_secrets.h"
#include "CoapTest.h"

// PIN Number, APN
char pinNumber[] = SECRET_PINNUMBER;
char apn[] = SECRET_GPRS_APN;

// IP Lookup URL, Endpoint and Port
char server[] = SECRET_HOSTNAME;
char endpoint[] = "/";
uint32_t httpPort = SECRET_HTTP_PORT;

// CoAP Server Endpoint and Port
char coapEndpoint[] = SECRET_COAP_ENDPOINT;
uint32_t coapPort = SECRET_COAP_PORT;

// Initialise any Library Instances
NBClient nbClient;
NBUDP udp;
Coap coap(udp);
HttpClient httpClient = HttpClient(nbClient, server, httpPort);
GPRS gprsAccess;
NB nbAccess;
IPAddress coapServer_ip;

void setup()
{

  // Open serial communication and wait for port to open
  Serial.begin(9600);
  while (!Serial)
    ;

  setupModem();

  // Check if connected and if not, reconnect
  if (nbAccess.status() != NB_READY || gprsAccess.status() != GPRS_READY)
  {
    connectNB(nbAccess, gprsAccess);
  }

  // Initialize CoAP client
  coap.start();
}

void loop()
{
  getIPAddress(coapServer_ip, httpClient);

  randomSeed(analogRead(0));

  Serial.print("Sending packet to CoAP Server on ");
  Serial.println(coapServer_ip);

  // Generate random number
  float rand = 20 + random(0, 9);

  sendPacket(coapServer_ip, coap, rand);

  Serial.println("done.");

  Serial.println("Wait 30s before sending again...");
  delay(30000);
}