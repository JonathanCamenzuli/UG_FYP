#include "Comms.h"
#include <ArduinoJson.h>

// ALREADY DECLARED IN HEADER FILE
#include "arduino_secrets.h"
#include <MKRNB.h>
#include <coap-simple.h>
#include <ArduinoHttpClient.h>

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

void sendPacket(IPAddress &coapServer_ip, Coap &coap, char *packet)
{
    coap.put(coapServer_ip, SECRET_COAP_PORT, SECRET_COAP_ENDPOINT, packet);
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