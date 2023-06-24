//#include <ArduinoJson.h>
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

bool connectNB(NB &nbAccess, GPRS &gprsAccess, char *pin, char *apn)
{
  Serial.print("Connecting to ISP...");
  // Failed, retry
  while (nbAccess.begin(pin, apn) != NB_READY || gprsAccess.attachGPRS() != GPRS_READY)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("done.");

  return true;
}

void sendPacket(IPAddress &coapServer_ip, Coap &coap, char *packet, uint32_t port, char *endpoint)
{
  uint16_t msgid;
  Serial.print("Sending packet to CoAP server on ");
  Serial.print(coapServer_ip);
  Serial.print("...");

  msgid = coap.put(coapServer_ip, port, endpoint, packet);

  Serial.print("done - msgid: ");
  Serial.println(msgid);
}

void getIPAddress(IPAddress &ipAddress, HttpClient &httpClient)
{
  int statusCode = 0;
  String response;

  do
  {
    if (statusCode != 200 && statusCode != 0)
    {
      Serial.print("\nStatus Code: ");
      Serial.println(statusCode);
      Serial.println("Server Not Found - Trying again");
    }

    httpClient.get("/");
    statusCode = httpClient.responseStatusCode();
    response = httpClient.responseBody();

  } while (statusCode != 200);

  ipAddress.fromString(response);
}
