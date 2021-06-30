#include <SPIFFS.h>
#include <WiFi.h>
#include "player.h"
#include "Arduino.h"
#include "style.h"

#define LED_CORRECT 19
#define LED_WRONG 18
#define LED_SCORE_1 17
#define LED_SCORE_2 33
#define LED_SCORE_3 14
#define RXD 16
#define TXD 17

const char *ssid = "ESP32-Access-Point";
const char *password = NULL;

WiFiClient client;

WiFiServer server(80);

String header;

String page;
String serial;
int scorePlayer = 3;
int scoreMaster = 2;
String selectedAnswer;
String selectedQuestion;
boolean firstQuestion = true;

void prepareHTML()
{
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
}

void updateScoreLEDs()
{
    if (scorePlayer == 0)
    {
        digitalWrite(LED_SCORE_1, LOW);
        digitalWrite(LED_SCORE_2, LOW);
        digitalWrite(LED_SCORE_3, LOW);
    }
    else if (scorePlayer == 1)
    {
        digitalWrite(LED_SCORE_1, HIGH);
        digitalWrite(LED_SCORE_2, LOW);
        digitalWrite(LED_SCORE_3, LOW);
    }
    else if (scorePlayer == 2)
    {
        digitalWrite(LED_SCORE_1, HIGH);
        digitalWrite(LED_SCORE_2, HIGH);
        digitalWrite(LED_SCORE_3, LOW);
    }
    else if (scorePlayer == 3)
    {
        digitalWrite(LED_SCORE_1, HIGH);
        digitalWrite(LED_SCORE_2, HIGH);
        digitalWrite(LED_SCORE_3, HIGH);
    }
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
    Serial2.begin(9600, SERIAL_8N1, RXD, TXD);
    setupWiFi();
    Serial.println("setup dome");
}

void loop()
{
    if (Serial2.available())
    {
        serial += Serial2.readString();
        Serial.println(serial);
    }

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
                    //HTML preperation
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-type:text/html");
                    client.println("Connection: close");
                    client.println();

                    //********************* HEADER *******************
                    if (header.indexOf("GET /?sendKey=") >= 0)
                    {
                        //Send Key to GM
                        Serial.println("?key=" + header.substring(14, 18));
                        Serial2.println("?key=" + header.substring(14, 18));
                    }
                    else if (header.indexOf("GET /style.css") >= 0) {
                        client.println(playerStyle);
                        break;
                    }
                    else if (header.indexOf("GET /checkKey") >= 0)
                    {
                        Serial.println("Check Key");
                        page = "checkKey";

                        Serial.println("?key=" + header.substring(14, 18));
                        Serial2.println("?key=" + header.substring(14, 18));
                    }
                    else if (header.indexOf("GET /?selectedAnswer=") >= 0)
                    {
                        Serial.println("Go to Aolution");
                        page = "solution";

                        selectedAnswer = header.substring(21, 22);

                        //Send selected answer to GM
                        Serial.println("?answer="+header.substring(21, 22));
                        Serial2.println("?answer="+header.substring(21, 22));
                    }
                    else if (header.indexOf("GET /newQuestion") >= 0)
                    {
                        selectedQuestion = serial.substring(10, 15);
                        Serial.println("New Question " + selectedQuestion);
                        page = "question";
                    }
                    else if (header.indexOf("GET /?result=") >= 0)
                    {
                        if (header.indexOf("right") >= 0)
                        {
                            digitalWrite(LED_CORRECT, HIGH);
                            scorePlayer++;
                            Serial.println("RICHTIG");
                        }
                        else if (header.indexOf("wrong") >= 0)
                        {
                            digitalWrite(LED_WRONG, HIGH);
                            scoreMaster++;
                            Serial.println("FALSCH");
                        }
                    }
                    else if (header.indexOf("command") >= 0)
                    {
                        // Serial.println(serial);
                        if (serial.indexOf("?valid=") >= 0)
                        {
                            Serial.println("VALID");
                            client.println("RELOAD");
                            if (serial.indexOf("true") >= 0)
                            {
                                Serial.println("TRUE");
                                // page = "question";
                                page = "wait";
                                serial = "";
                                break;
                            }
                        }
                        else if (serial.indexOf("?question=") >= 0)
                        {

                            Serial.println("Get question from Game Master");
                            client.println("RELOAD");
                            selectedQuestion = serial.substring(10, 15);
                            Serial.println("selected Question " + selectedQuestion);

                            page = "question";
                            serial = "";
                            // break;
                        }
                        else if (serial.indexOf("?correct=") >= 0)
                        {
                            String test = serial.substring(9, 10);
                            Serial.println(selectedAnswer);
                            client.println("{\"correctAnswer\":\"" + test + "\", \"selectedAnswer\":\"" + selectedAnswer + "\"}");
                            page = "solution";
                            serial = "";
                        }
                        else
                        {
                            page = "index";
                        }
                        serial = "";
                        break;
                    }

                    //*********************** PAGES ********************
                    if (page.indexOf("checkKey") >= 0)
                    {
                        Serial.println("in ckeck key");
                        client.println(playerCheckKey);
                    }
                    else if (page.indexOf("wait") >= 0)
                    {
                        Serial.println("in Wait");
                        client.println(playerWait);

                        // Serial.println("?key="+);
                        // Serial2.println("?key="+);
                    }
                    else if (page.indexOf("question") >= 0)
                    {
                        Serial.println("in question " + selectedQuestion);
                        String htmlcode = String(playerQuestion);
                        htmlcode.replace("$questionID", selectedQuestion);
                        client.println(htmlcode);
                    }
                    else if (page.indexOf("solution") >= 0)
                    {
                        Serial.println("in solution");
                        String htmlcode = String(solutionHTML);
                        Serial.println("************ " + selectedQuestion);
                        htmlcode.replace("$QUESTION", selectedQuestion);
                        Serial.println(selectedQuestion);
                        client.println(htmlcode);
                    }
                    else if (page.indexOf("win") >= 0)
                    {
                        if (scoreMaster < scorePlayer)
                        {
                            Serial.println("WINNER");
                            client.println(playerWin);
                        }
                        else
                        {
                            Serial.println("LOOSER");
                            client.println(playerLoose);
                        }
                    }
                    else
                    {
                        client.println(playerIndex);
                    }

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
    }
    if (scoreMaster == 3 || scorePlayer == 3)
    {
        page = "win";
    }
    updateScoreLEDs();
}