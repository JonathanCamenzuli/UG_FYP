#include "IPLookup.h"

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