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
 * - ArduinoLowPower by Arduino Libraries (https://github.com/arduino-libraries/ArduinoLowPower)
 * - MKRNB by Arduino Libraries (https://github.com/arduino-libraries/MKRNB)
 * - CoAP Test Header File (./CoapTest.h)
 *
 * @section hardware Hardware
 * -  Arduino MKR NB 1500
 */

#include <ArduinoLowPower.h>
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

  coapServer_ip.fromString((coapIP));

  // Open serial communication and wait for port to open
  Serial.begin(115200);
  while (!Serial)
    ;

  /**
   * Issue a series of AT commands to the modem.
   *
   *    - Wait for it to get ready
   *    - Disconnect from any connected networks
   *    - Set RAT (NB-IoT or LTE-M)
   *    - Apply changes and save configuration
   *    - Turn radio on
   *    - Check attachment
   *    - Set operator
   */

  Serial.print("Waiting for modem to get ready...");
  MODEM.begin();
  while (!MODEM.noop())
    ;
  Serial.println("done.");

  Serial.print("Disconnecting from network...");
  MODEM.send("AT+COPS=2");
  MODEM.waitForResponse(2000);
  Serial.println("done.");

  Serial.print("Setting Radio Access Technology to NB-IoT or LTE-M (7 is for LTE-M and 8 is for NB-IoT)...");
  MODEM.sendf("AT+URAT=%d", rat);
  MODEM.waitForResponse(2000);
  Serial.println("done.");

  Serial.print("Applying changes and saving configuration...");
  MODEM.send("AT+CFUN=15");
  do
  {
    delay(1000);
    MODEM.noop();
  } while (MODEM.waitForResponse(1000) != 1);
  Serial.println("done.");

  Serial.print("Modem ready, turn radio on in order to configure it...");
  MODEM.send("AT+CFUN=1");
  MODEM.waitForResponse(2000);
  Serial.println("done.");

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

  // Serial.print("Set operator to Telenor...");
  // MODEM.sendf("AT+COPS=1,2,\"%d\"", cops);
  // MODEM.waitForResponse(4000);
  // Serial.println("done.");

  // Check if connected and if not, reconnect
  if (nbAccess.status() != NB_READY || gprsAccess.status() != GPRS_READY)
  {
    connectNB(nbAccess, gprsAccess);
  }

  // Initialize CoAP client
  coap.start();

  // Seed random number generator with noise from pin 0
  randomSeed(analogRead(0));
}

void loop()
{
  // Check if connected and if not, reconnect
  // if (nbAccess.status() != NB_READY || gprsAccess.status() != GPRS_READY)
  // {
  //   connectNB(nbAccess, gprsAccess);
  // }

  // TEMPORARY !!

  Serial.print("Sending packet to CoAP Server");

  // Generate random number
  float rand = 20 + random(0, 9);

  // Package JSON Document
  // char *jsonDoc = serialiseJson(rand);

  //////////////////////////////////////// serialise
  // Size calculated on https://arduinojson.org/v6/assistant/
  StaticJsonDocument<96> jsonDoc;

  // Create a string for storing the serialized JSON document
  char buffer[BUF_SIZE];

  // Set the values of the JSON packet
  jsonDoc["nodetype"] = "TEST";
  jsonDoc["id"] = "test0001";

  // Creating and setting the value for the data nested object
  JsonObject data = jsonDoc.createNestedObject("data");
  data["testValue"] = rand;

  // Serialize the JSON document
  serializeJson(jsonDoc, buffer);

  // Free the memory occupied by the JSON document
  jsonDoc.clear();

  Serial.println(buffer);
  // Serial.println(jsonDoc);

  // uint16_t msgid = coap.put(coapServer_ip, SECRET_COAP_PORT, SECRET_COAP_ENDPOINT1, jsonDoc);
  uint16_t msgid = coap.put(coapServer_ip, SECRET_COAP_PORT, SECRET_COAP_ENDPOINT, buffer);

  // sendPacket(coapServer_ip, coap);
  Serial.println("done.");

  // Continue CoAP operations (handle ACK, send response)
  coap.loop();

  Serial.println("Wait 10s before sending again...");
  delay(10000);
}