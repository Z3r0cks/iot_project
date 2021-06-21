#include <SPIFFS.h>
#include <WiFi.h>
#include "webapp-end.h"
#include "webapp-start.h"

const char* ssid = "ESP32-Access-Point";
const char* password = NULL;

String htmlstring = webappStart;

WiFiClient client;

bool download = false;

String downloadContent;

WiFiServer server(80);

String header;

String getInput(int begin, int end) {
    String input;

    for (int i = begin + 4; i < end; i++) {
        input += header.charAt(i);
    }

    Serial.println("input " + input);
    return input;
}

void setupWiFi() {
    Serial.print("Setting AP (Access Point)…");
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.begin();
}

void setupSPIFFS() {
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        ESP.restart();
    }
}

void setup() {
    Serial.begin(9600);
    setupSPIFFS();
    setupWiFi();
}

void loop() {
    client = server.available();

    if (client) {
        String currentLine = "";
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);
                header += c;
                if (c == '\n') {
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-type:text/html");
                    client.println("Connection: close");
                    client.println();
                    htmlstring = webappStart;
                    client.println(htmlstring);

                    if (header.indexOf("GET /save") >= 0) {
                        download = false;
                        File questionFile =
                            SPIFFS.open("/newQuestions.txt", FILE_APPEND);

                        int indexQ = header.indexOf("__!q");
                        int indexA = header.indexOf("__!a");
                        int indexB = header.indexOf("__!b");
                        int indexC = header.indexOf("__!c");
                        int indexD = header.indexOf("__!d");

                        questionFile.print(
                            "__F: " + getInput(indexQ, indexA) +
                            "__A: " + getInput(indexA, indexB) +
                            "__B: " + getInput(indexB, indexC) +
                            "__C: " + getInput(indexC, indexD) +
                            "__D: " + getInput(indexD, header.length() - 11));

                        questionFile.close();
                    } else if (header.indexOf("GET /delete") >= 0) {
                        SPIFFS.remove("/newQuestions.txt");
                    } else if (header.indexOf("GET /iotsose21") >= 0) {
                        downloadContent = "";
                        File questionFile = SPIFFS.open("/newQuestions.txt");

                        Serial.println("File Content:");

                        while (questionFile.available()) {
                            downloadContent += char(questionFile.read());
                        }
                        questionFile.close();
                        Serial.println("downlpad Content " + downloadContent);
                        client.println(
                            "<button id=\"createLinkBtn\" "
                            "onclick=\"createLink('" +
                            downloadContent + "')\">Starte Download</button>");
                        client.println(
                            "<button onclick='deleteSPIFFS(); "
                            "window.location.reload();'>Erstellte Fragen "
                            "Löschen</button>");
                    }

                    client.println(webappEnd);
                    break;
                } else {
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