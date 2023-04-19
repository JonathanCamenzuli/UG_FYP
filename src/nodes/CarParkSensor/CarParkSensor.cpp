#include "CarParkSensor.h"
#include <ArduinoJson.h>
#include "Comms.h"

// ALREADY DECLARED IN HEADER FILE
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

/**
 * @brief Function returns average of an array
 *
 * @param array     The array itself
 * @param elems     Number of elements in array
 * @return float    Sum of array elements divided by number of elements
 */
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
    coap.start();
    sendPacket(ipAddress, coap, jsonDocBuf);

    // Temporary
    free(jsonDocBuf);
}

/**
 * @brief Creates Serialised JSON document for Car Park Sensor
 *
 * @param isCarParked   Boolean which indicates whether a vehicle is parked
 * @param buffer        Buffer to store serialised JSON document
 */
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
    // serializeJson(jsonDoc, buffer, sizeof(buffer));
    serializeJson(jsonDoc, buffer, JSON_BUF_SIZE);

    // Free the memory occupied by the JSON document
    jsonDoc.clear();
}