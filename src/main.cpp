#include <SPIFFS.h>
#include <WiFi.h>
#include "webapp-end.h"
#include "webapp-start.h"

const char *ssid = "ESP32-Access-Point";
const char *password = NULL;

String htmlstring = webappStart;

WiFiClient client;

bool download = false;

String downloadContent;

WiFiServer server(80);

String header;

String getInput(int begin, int end)
{
    String input;

    for (int i = begin + 4; i < end; i++)
    {
        input += header.charAt(i);
    }

    Serial.println("input " + input);
    return input;
}

void setupWiFi()
{
    Serial.print("Setting AP (Access Point)…");
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.begin();
}

void setupSPIFFS()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        ESP.restart();
    }
}

void setup()
{
    Serial.begin(9600);
    setupSPIFFS();
    setupWiFi();
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
                    htmlstring = webappStart;
                    client.println(htmlstring);

                    if(header.indexOf("GET /save") > 0){
                        // do some stuff
                    }

                      client.println(webappEnd);
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