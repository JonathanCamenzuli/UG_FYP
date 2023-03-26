#include "CoapTest.h"
#include "arduino_secrets.h"
#include <MKRNB.h>
#include <coap-simple.h>
#include <ArduinoJson.h>

void setupModem()
{
    // Wait for it to get ready
    Serial.print("Waiting for modem to get ready...");
    MODEM.begin();
    while (!MODEM.noop())
        ;
    Serial.println("done.");

    // Set RAT to NB-IoT
    Serial.print("Setting Radio Access Technology to NB-IoT...");
    MODEM.sendf("AT+URAT=%d", 8);
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
}

void serialiseJson(float &testVal, char *buffer)
{
    // Size calculated on https://arduinojson.org/v6/assistant/
    StaticJsonDocument<96> jsonDoc;

    // Set the values of the JSON packet
    jsonDoc["nodetype"] = "TEST";
    jsonDoc["id"] = "test0001";

    // Creating and setting the value for the data nested object
    JsonObject data = jsonDoc.createNestedObject("data");
    data["testValue"] = testVal;

    // Serialize the JSON document
    serializeJson(jsonDoc, buffer, BUF_SIZE);

    // Free the memory occupied by the JSON document
    jsonDoc.clear();
}

bool connectNB(NB &nbAccess, GPRS &gprsAccess)
{
    Serial.print("Connecting to ISP...");
    // Failed, retry
    while (nbAccess.begin(SECRET_PINNUMBER, SECRET_GPRS_APN) != NB_READY || gprsAccess.attachGPRS() != GPRS_READY)
    {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("done.");

    return true;
}

void sendPacket(IPAddress &coapServer_ip, Coap &coap, float &rand)
{
    // Create a string for storing the serialized JSON document
    char *jsonDocBuf = (char *)malloc(BUF_SIZE);
    // alternative: char jsonDocBuf[BUF_SIZE];

    // Package JSON Document
    serialiseJson(rand, jsonDocBuf);
    Serial.println(jsonDocBuf);

    coap.put(coapServer_ip, SECRET_COAP_PORT, SECRET_COAP_ENDPOINT, jsonDocBuf);
    free(jsonDocBuf);
}

void getIPAddress(IPAddress &ipAddress, HttpClient &httpClient)
{
    int statusCode;
    String response;

    httpClient.get("/");

    statusCode = httpClient.responseStatusCode();
    response = httpClient.responseBody();

    if (statusCode != 200)
    {
        Serial.print("Status Code: ");
        Serial.println(statusCode);
        Serial.print("Server Not Found");
        ipAddress.fromString("0.0.0.0");
    }

    ipAddress.fromString(response);
}