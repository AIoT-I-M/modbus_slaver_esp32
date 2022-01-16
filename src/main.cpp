
#include <Arduino.h>
#include <WiFi.h>

// Modbus server TCP
#include "ModbusServerTCPasync.h"

#define MAX_MEMORY 64
char ssid[] = "deep-dark-in";
char pass[] = "665544332211";

ModbusServerTCPasync MBserver;
uint16_t memory[MAX_MEMORY];
//-------------------------------------------
// 	Code |  Register Type
//-------|------------------------------------
// 	1	 |	Read Coil
// 	2	 |	Read Discrete Input
// 	3	 |	Read Holding Registers
// 	4	 |	Read Input Registers
// 	5	 |	Write Single Coil
// 	6	 |	Write Single Holding Register
// 	15	 |	Write Multiple Coils
// 	16	 |	Write Multiple Holding Registers
//-------------------------------------------

uint8_t coils[10] = {1, 0, 1, 0, 0, 0, 0, 0, 0, 1};
ModbusMessage FC01(ModbusMessage request)
{
    ModbusMessage response;
    uint8_t serverID = request.getServerID();
    uint16_t count = request.end() - request.begin();
    response.setMessage(serverID, READ_COIL, count, coils);
    return response;
}

void setup()
{

    Serial.begin(115200);
    WiFi.begin(ssid, pass);
    delay(200);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(". ");
        delay(1000);
    }
    IPAddress wIP = WiFi.localIP();
    Serial.printf("WIFi IP address: %u.%u.%u.%u\n", wIP[0], wIP[1], wIP[2], wIP[3]);

    // Set up test memory
    for (uint16_t i = 0; i < 32; ++i)
    {
        memory[i] = (i * 2) << 8 | ((i * 2) + 1);
    }

    // Define and start RTU server
    MBserver.registerWorker(1, READ_COIL, &FC01);
    MBserver.start(1502, 1, 20000);
}

void loop()
{
    delay(1000);
    Serial.println("PING");
}