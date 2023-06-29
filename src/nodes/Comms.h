/**
 * @file Comms.h
 *
 * @author Jonathan Camenzuli
 *
 * @brief Communication-related Functions Header File
 *
 * Source code is part of my Final Year Project in Computer Engineering (2022/23) entitled
 * "Miniature implementation of an IoT-based Smart City"
 *
 * @date 02/03/2023
 */

#ifndef __COMMS_H
#define __COMMS_H

#include <MKRNB.h>
#include <coap-simple.h>
#include <ArduinoHttpClient.h>

/**
 * @brief Function for Setting Up Modem and Chipset
 *
 */
void setupModem();

/**
 * @brief Connects to NB-IoT Network
 *
 * @param nbAccess      NB Object
 * @param gprsAccess    GPRS Object
 * @param pin           SIM PIN
 * @param apn           SIM APN
 * @return true         Connection Successful
 * @return false        Connection Failed
 */
bool connectNB(NB &nbAccess, GPRS &gprsAccess, char *pin, char *apn);

/**
 * @brief Sends Packet to CoAP server
 *
 * @param coapServer_ip IP Address to Server (Object)
 * @param coap          Coap Object
 * @param packet        Packet to send to CoAP server
 * @param port          CoAP Server Port
 * @param endpoint      CoAP Server Endpoint
 */
void sendPacket(IPAddress &coapServer_ip, Coap &coap, char *packet, uint32_t port, char *endpoint);

/**
 * @brief Provides IP Address from IP Lookup Server
 *
 * @param ipAddress     IPAddress Object where IP Address will be stored
 * @param httpClient    HTTP Client Object
 */
void getIPAddress(IPAddress &ipAddress, NBClient &nbClient, char *server, uint32_t port);

#endif