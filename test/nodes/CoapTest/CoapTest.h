

/**
 * @file CarParkSensor.h
 *
 * @author Jonathan Camenzuli
 *
 * @brief CoAP Test Header File
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 15/03/2023
 *
 * @section libraries Libraries
 * - ArduinoJson by Benolt Blanchon (https://github.com/bblanchon/ArduinoJson)
 */

#ifndef __COAPTEST_H
#define __COAPTEST_H

#include <MKRNB.h>
#include <ArduinoJson.h>
#include <coap-simple.h>
#include "arduino_secrets.h"

#define SLEEP_TIME_MS 40000 // Sleep time, in milliseconds
#define BUF_SIZE 200        // Buffer Size

/**
 * @brief Function returns serialised JSON document for CoAP Test
 *
 * @return String Serialised JSON Document
 */
char *serialiseJson(float testVal)
{
    // Size calculated on https://arduinojson.org/v6/assistant/
    StaticJsonDocument<96> jsonDoc;

    // Create a string for storing the serialized JSON document
    char buffer[BUF_SIZE];

    // Set the values of the JSON packet
    jsonDoc["nodetype"] = "TEST";
    jsonDoc["id"] = "test0001";

    // Creating and setting the value for the data nested object
    JsonObject data = jsonDoc.createNestedObject("data");
    data["testValue"] = testVal;

    // Serialize the JSON document
    serializeJson(jsonDoc, buffer);

    // Free the memory occupied by the JSON document
    jsonDoc.clear();

    // Return the serialized JSON document as a string
    return buffer;
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

uint16_t sendPacket(IPAddress coapServer_ip, Coap coap)
{
    // Generate random number
    float rand = 20 + random(0, 9);

    // Package JSON Document
    char *jsonDoc = serialiseJson(rand);
    char buffer[BUF_SIZE];

    uint32_t buf_size = snprintf(buffer, BUF_SIZE, jsonDoc);

    // Send a CoAP POST message to CoAP server
    uint16_t msgid = coap.send(
        coapServer_ip,        // IP address
        SECRET_COAP_PORT,     // Port
        SECRET_COAP_ENDPOINT, // CoAP endpoint
        COAP_CON,             // CoAP packet type (con, noncon, ack, reset)
        COAP_POST,            // CoAP method
        NULL,                 // CoAP token
        0,                    // CoAP token length
        (uint8_t *)buffer,    // Message payload
        buf_size              // Message payload length
    );

    return msgid;
}

#endif