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
 * @section libraries Libraries
 * - ArduinoJson by Benoît Blanchon (https://github.com/bblanchon/ArduinoJson)
 * - MKRNB by Arduino Libraries (https://github.com/arduino-libraries/MKRNB)
 * - CoAP-simple-library by Hirotaka Niisato (https://github.com/hirotakaster/CoAP-simple-library)
 * - CoAP Test Header File (./CoapTest.h)
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

// IP Lookup URL, endpoint and port
char server[] = SECRET_HOSTNAME;
char endpoint[] = "/";
uint32_t httpPort = SECRET_HTTP_PORT;

// TEMPORARY!!
char coapIP[] = SECRET_IP_ADDRESS;

// CoAP server endpoint and port
char coapEndpoint[] = SECRET_COAP_ENDPOINT;
uint32_t coapPort = SECRET_COAP_PORT;

// Radio Access Technology (7 is for LTE-M and 8 is for NB-IoT)
uint32_t rat = SECRET_RAT;

bool debug = false;

// initialize the library instance
NB nbAccess(debug);
NBClient client;
NBUDP udp;
Coap coap(udp);
GPRS gprsAccess;

// TEMPORARY !!
// IPAddress coapServer_ip(coapIP);
IPAddress coapServer_ip;

void setup()
{
  // Temporary
  coapServer_ip.fromString((coapIP));

  // Open serial communication and wait for port to open
  Serial.begin(115200);
  while (!Serial)
    ;

  // Wait for it to get ready
  Serial.print("Waiting for modem to get ready...");
  MODEM.begin();
  while (!MODEM.noop())
    ;
  Serial.println("done.");

  // Disconnect from any connected networks
  Serial.print("Disconnecting from network...");
  MODEM.send("AT+COPS=2");
  MODEM.waitForResponse(2000);
  Serial.println("done.");

  // Set RAT (NB-IoT or LTE-M)
  Serial.print("Setting Radio Access Technology to NB-IoT or LTE-M (7 is for LTE-M and 8 is for NB-IoT)...");
  MODEM.sendf("AT+URAT=%d", rat);
  MODEM.waitForResponse(2000);
  Serial.println("done.");

  // Apply changes and save configuration
  Serial.print("Applying changes and saving configuration...");
  MODEM.send("AT+CFUN=15");
  do
  {
    delay(1000);
    MODEM.noop();
  } while (MODEM.waitForResponse(1000) != 1);
  Serial.println("done.");

  // Turn radio on
  Serial.print("Modem ready, turn radio on in order to configure it...");
  MODEM.send("AT+CFUN=1");
  MODEM.waitForResponse(2000);
  Serial.println("done.");

  // Check attachment
  Serial.print("Check attachment until CSQ RSSI indicator is less than 99...");
  String response;
  int status = 99;
  while (status > 98 && status > 0)
  {
    MODEM.send("AT+CSQ");
    MODEM.waitForResponse(2000);
    String sub = response.substring(6, 8);
    status = sub.toInt(); // Will return 0 if no valid number is found
    delay(1000);
  }
  Serial.println("done.");

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
  // Seed random number generator with noise from pin 0
  randomSeed(analogRead(0));

  Serial.print("Sending packet to CoAP Server");

  // Generate random number
  float rand = 20 + random(0, 9);

  sendPacket(coapServer_ip, coap, rand);

  Serial.println("done.");

  Serial.println("Wait 30s before sending again...");
  delay(30000);
}