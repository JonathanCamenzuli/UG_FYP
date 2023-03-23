#include "CoapTest.h"
#include "arduino_secrets.h"
#include <MKRNB.h>
#include <coap-simple.h>
#include <ArduinoJson.h>

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

    // Serial.println(buffer);
}

bool connectNB(NB nbAccess, GPRS gprsAccess)
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

uint16_t sendPacket(IPAddress &coapServer_ip, Coap &coap, float &rand)
{
    // Create a string for storing the serialized JSON document
    char jsonDocBuf[BUF_SIZE];

    // Package JSON Document
    serialiseJson(rand, jsonDocBuf);
    Serial.println(jsonDocBuf);

    uint16_t msgid = coap.put(coapServer_ip, SECRET_COAP_PORT, SECRET_COAP_ENDPOINT, jsonDocBuf);
    free(jsonDocBuf);
    return msgid;
}