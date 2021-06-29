#include <SPIFFS.h>
#include <WiFi.h>
// // #include "webapp-end.h"
// // #include "webapp-start.h"
#include "master-index.h"
#include "Arduino.h"

const char *ssid = "ESP32-Access-Point";
const char *password = NULL;


WiFiClient client;

// bool download = false;

// String downloadContent;

WiFiServer server(80);

String header;

void setupWiFi()
{
    Serial.print("Setting AP (Access Point)…");
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.begin();
}

void setup()
{
    Serial.begin(7200);
    setupWiFi();
    Serial.println("setup dome");
}

void loop()
{

    client = server.available();

    if (client)
    {
        String currentLine = "";
        while (client.connected())
        {
            if (client.available())
            {
                char c = client.read();
                Serial.write(c);
                header += c;
                if (c == '\n')
                {
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-type:text/html");
                    client.println("Connection: close");
                    client.println();
                    // htmlstring = webappStart;
                    client.println(masterIndex);

                    if(header.indexOf("GET /save") > 0){
                        // do some stuff
                    }

                    //   client.println(webappEnd);
                    break;
                }
                else
                {
                    currentLine = "";
                }
            }
        }

        header = "";
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
    }
}