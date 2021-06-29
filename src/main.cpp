#include <SPIFFS.h>
#include <WiFi.h>
#include "player.h"
#include "Arduino.h"

const char *ssid = "ESP32-Access-Point";
const char *password = NULL;

WiFiClient client;

WiFiServer server(80);

String header;

void prepareHTML()
{
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
    // htmlstring = webappStart;
    // client.println(webstring);
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
                    // client.println("HTTP/1.1 200 OK");
                    // client.println("Content-type:text/html");
                    // client.println("Connection: close");
                    // client.println();
                    // // htmlstring = webappStart;
                    // client.println(playerIndexHTML);
                    prepareHTML();
                    client.println(playerIndexHTML);

                    Serial.println("Herader " + header);
                    if (header.indexOf("GET /enterGame") >= 0)
                    {
                        Serial.println("enter Game");
                    }

                    client.println("</body>");
                    client.println(playerIndexJS);
                    client.println("</html>");

                    break;
                }

                else
                {
                    currentLine = "";
                }
                Serial.println("wifi client available");
            }
            Serial.println("wifi client connected");
        }

        header = "";
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
    }

    if (Serial.available())
    {
        Serial.println("available");
        String inputSerial = Serial.readString();
        if (inputSerial.equals("enter Game"))
            Serial.println("IN GAME");
    }
}