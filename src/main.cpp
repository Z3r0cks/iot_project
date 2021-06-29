#include "Arduino.h"
#include <SPIFFS.h>
#include <WiFi.h>
#include <MasterHtml.h>

const char *ssid = "ESP32-Access-Point";
const char *password = NULL;

WiFiClient client;
WiFiServer server(80);

void setupWiFi()
{
    Serial.print("Setting up Acces Point");
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print(" at ");
    Serial.println(IP);

    server.begin();
}

void respond(String data, String contentType)
{
    client.println("HTTP/1.1 200 OK");
    client.print("Content-type: ");
    client.println(contentType);
    client.println("Connection: close");
    client.println();
    if (!data.isEmpty())
    {
        client.println(data);
    }
}

void respond(String html)
{
    respond(html, "text/html");
}

void setup()
{
    Serial.begin(7200);
    setupWiFi();
    Serial.println("Setup done.");
}

void loop()
{

    client = server.available();

    if (client)
    {
        while (client.connected())
        {
            if (client.available())
            {
                String header = client.readString();
                Serial.println(header);
                if (header.endsWith("\n"))
                {
                    if (header.indexOf("GET /")) {
                        client.println("HTTP/1.1 200 OK");
                        client.print("Content-type: ");
                        client.println("text/html");
                        client.println("Connection: close");
                        client.println();
                        client.println(masterIndex);
                    }
                    break;
                }
            }
        }

        client.stop();
        Serial.println("Client disconnected.");
        Serial.println();
    }
}