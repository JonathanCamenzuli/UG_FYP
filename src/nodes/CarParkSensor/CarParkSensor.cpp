#include "CarParkSensor.h"
#include <ArduinoJson.h>
#include "Comms.h"
#include "arduino_secrets.h"
#include <MKRNB.h>
#include <ArduinoHttpClient.h>
#include <coap-simple.h>

int getUltrasonicReading()
{
    long duration; // Duration of Ultrasonic wave travel
    int distance;  // Distance calculated

    // HC-SR04 shenanigans
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Distance Calculation
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration * 0.034 / 2;
    return distance;
}

float averageArray(int *array, int elems)
{
    long sum = 0L;
    for (int i = 0; i < elems - 1; i++)
        sum += array[i];
    return ((float)sum) / elems;
}

void changeSendParkingState(bool &isVehicleParked, NB &nbAccess, GPRS &gprsAccess, IPAddress &ipAddress, HttpClient &httpClient, Coap &coap)
{
    // Create a string for storing the serialized JSON document
    char *jsonDocBuf = (char *)malloc(JSON_BUF_SIZE);

    serialiseJson(isVehicleParked, jsonDocBuf);

    // Turn on radio and transmit change in parking state
    Serial.println(jsonDocBuf);

    setupModem();
    // Check if connected and if not, reconnect
    if (nbAccess.status() != NB_READY || gprsAccess.status() != GPRS_READY)
    {
        connectNB(nbAccess, gprsAccess);
    }
    getIPAddress(ipAddress, httpClient);

    Serial.print("Setting Up CoAP Functionality...");
    coap.start(SECRET_COAP_PORT);
    Serial.println("done.");

    Serial.print("Sending packet to CoAP server on ");
    Serial.print(ipAddress);
    Serial.print("...");
    sendPacket(ipAddress, coap, jsonDocBuf);
    Serial.println("done.");

    Serial.print("Disconnecting from ISP and turning off Modem...");
    nbAccess.shutdown();
    Serial.println("done.");
    free(jsonDocBuf);
}

void serialiseJson(bool &isCarParked, char *buffer)
{
    // Size calculated on https://arduinojson.org/v6/assistant/
    StaticJsonDocument<96> jsonDoc;

    // Set the values of the JSON packet
    jsonDoc["nodetype"] = "CPS";
    jsonDoc["id"] = "cps0001";

    // Creating and setting the value for the data nested object
    JsonObject data = jsonDoc.createNestedObject("data");
    data["isCarParked"] = isCarParked;

    // Serialize the JSON document
    serializeJson(jsonDoc, buffer, JSON_BUF_SIZE);

    // Free the memory occupied by the JSON document
    jsonDoc.clear();
}