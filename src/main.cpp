#include "Arduino.h"
#include <SPIFFS.h>
#include <WiFi.h>
#include <Master.h>

#define LED_CORRECT 19
#define LED_WRONG 18
#define LED_SCORE_1 25
#define LED_SCORE_2 26
#define LED_SCORE_3 27

#define ROUTE_INDEX_JS "GET /index.js"
#define ROUTE_QUESTIONS_JS "GET /questions.js"
#define ROUTE_SOLUTION_JS "GET /solution.js"
#define ROUTE_ENDSCREEN_JS "GET /endscreen.js"
#define ROUTE_VALIDATE_KEY "GET /validate-key"
#define ROUTE_CHOOSE_QUESTION "GET /choose-question"
#define ROUTE_VALIDATE_ANSWER "GET /validate-answer"
#define ROUTE_FINISH_GAME "GET /finish-game"
#define ROUTE_STYLE_CSS "GET /style.css"

#define endTime 10000

enum Page
{
    INDEX,
    QUESTIONS,
    SOLUTION,
    ENDSCREEN
};

struct Answer
{
    String id;
    char letter;
};

const char *ssid = "Naturquiz (Gamemaster)";
const char *password = NULL;

WiFiClient client;
WiFiServer server(80);

Page page;
String key, validation, serial, question;

byte scoreMaster, scorePlayer;

unsigned long lastTime;
bool sleepTimerEnabled;
long sleepTimer;

const Answer correctAnswers[] = {
    {"01_01", 'A'},
    {"01_02", 'B'},
    {"01_03", 'C'},
    {"01_04", 'D'},
    {"02_01", 'A'},
    {"02_02", 'B'},
    {"02_03", 'C'},
    {"02_04", 'D'},
    {"03_01", 'A'},
    {"03_02", 'B'},
    {"03_03", 'C'},
    {"03_04", 'D'},
    {"04_01", 'A'},
    {"04_02", 'B'},
    {"04_03", 'C'},
    {"04_04", 'D'}};

char answer(String id) {
    for(const Answer &answer : correctAnswers) {
        if(answer.id.equals(id)) return answer.letter;
    }
    return ' ';
}

void setupWiFi()
{
    Serial.print("Setting up Acces Point");
    WiFi.softAP(ssid, password);

    IPAddress ip = WiFi.softAPIP();
    Serial.print(" at ");
    Serial.println(ip);

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

unsigned long randomKey(int positions)
{
    unsigned long upper = pow(10, positions);
    unsigned long lower = pow(10, positions - 1);
    return (millis() % (upper - lower)) + lower;
}

void updateScoreLEDs()
{
    if (scoreMaster == 0)
    {
        digitalWrite(LED_SCORE_1, LOW);
        digitalWrite(LED_SCORE_2, LOW);
        digitalWrite(LED_SCORE_3, LOW);
    }
    else if (scoreMaster == 1)
    {
        digitalWrite(LED_SCORE_1, HIGH);
        digitalWrite(LED_SCORE_2, LOW);
        digitalWrite(LED_SCORE_3, LOW);
    }
    else if (scoreMaster == 2)
    {
        digitalWrite(LED_SCORE_1, HIGH);
        digitalWrite(LED_SCORE_2, HIGH);
        digitalWrite(LED_SCORE_3, LOW);
    }
    else if (scoreMaster == 3)
    {
        digitalWrite(LED_SCORE_1, HIGH);
        digitalWrite(LED_SCORE_2, HIGH);
        digitalWrite(LED_SCORE_3, HIGH);
    }
}

void updateEndTimer()
{
    if (sleepTimerEnabled)
    {
        unsigned long time = millis();
        sleepTimer -= millis() - lastTime;
        lastTime = millis();
        if (endTime <= 0)
        {
            page = Page::INDEX;
            scoreMaster = 0;
            scorePlayer = 0;
            digitalWrite(LED_CORRECT, LOW);
            digitalWrite(LED_WRONG, LOW);
            sleepTimerEnabled = false;
            sleepTimer = endTime;
        }
    }
}

void setup()
{
    pinMode(LED_CORRECT, OUTPUT);
    pinMode(LED_WRONG, OUTPUT);
    pinMode(LED_SCORE_1, OUTPUT);
    pinMode(LED_SCORE_2, OUTPUT);
    pinMode(LED_SCORE_3, OUTPUT);
    Serial.begin(7200);
    setupWiFi();
    sleepTimerEnabled = false;
    sleepTimer = endTime;
    page = Page::INDEX;
    scoreMaster = 0;
    scorePlayer = 0;
    updateScoreLEDs();
    Serial.println("Setup done.");
}

void loop()
{
    if (Serial.available())
    {
        serial = Serial.readString();
        Serial.println(serial);
    }

    client = server.available();

    if (client)
    {
        while (client.connected())
        {
            if (client.available())
            {
                String header = client.readString();
                //Serial.println(header);
                if (header.endsWith("\n"))
                {
                    if (header.startsWith(ROUTE_INDEX_JS))
                    {
                        validation = randomKey(8);
                        key = randomKey(4);
                        String script = String(masterIndexJS);
                        script.replace("$KEY", key);
                        script.replace("$VALIDATION", validation);
                        respond(script, "application/javascript");
                        break;
                    }
                    else if (header.startsWith(ROUTE_QUESTIONS_JS))
                    {
                        validation = randomKey(8);
                        String script = String(masterQuestionsJS);
                        script.replace("$VALIDATION", validation);
                        respond(script, "application/javascript");
                        break;
                    }
                    else if (header.startsWith(ROUTE_SOLUTION_JS))
                    {
                        validation = randomKey(8);
                        String script = String(masterSolutionJS);
                        script.replace("$VALIDATION", validation);
                        respond(script, "application/javascript");
                        break;
                    }
                    else if (header.startsWith(ROUTE_VALIDATE_KEY))
                    {
                        if (serial.startsWith("?key=") && serial.indexOf(key) == 5)
                        {
                            respond(validation, "text/plain");
                            page = Page::QUESTIONS;
                            Serial.println("?valid=true");
                            serial = "";
                        }
                        else if (!serial.isEmpty())
                        {
                            respond("", "text/plain");
                            Serial.println("?valid=false");
                        }
                        else {
                            respond("", "text/plain");
                        }
                        break;
                    }
                    else if (header.startsWith(ROUTE_CHOOSE_QUESTION))
                    {
                        int index = strlen(ROUTE_CHOOSE_QUESTION);
                        respond(validation, "text/plain");
                        page = Page::SOLUTION;
                        String selectedQuestion = header.substring(index, index + 15);
                        question = selectedQuestion.substring(11, 15);
                        Serial.println(selectedQuestion);
                        serial = "";
                        digitalWrite(LED_CORRECT, LOW);
                        digitalWrite(LED_WRONG, LOW);
                        break;
                    }
                    else if (header.startsWith(ROUTE_VALIDATE_ANSWER))
                    {
                        if (serial.startsWith("?selectedAnswer="))
                        {
                            int index = serial.length();
                            respond(validation, "text/plain");
                            String selectedAnswer = serial.substring(index, index + 1);
                            // Überprüfung der Fragen
                            if (selectedAnswer[0] == answer(question))
                            {
                                Serial.println("?correct=true");
                                digitalWrite(LED_WRONG, HIGH);
                                scorePlayer++;
                            }
                            else
                            {
                                Serial.println("?correct=false");
                                digitalWrite(LED_CORRECT, HIGH);
                                scoreMaster++;
                            }
                            serial = "";
                            if (scoreMaster == 3 || scorePlayer == 3)
                                page = Page::ENDSCREEN;
                            else
                                page = Page::QUESTIONS;
                        }
                        else {
                            respond("", "text/plain");
                        }
                        break;
                    }
                    else if (header.startsWith(ROUTE_STYLE_CSS))
                    {
                        respond(masterStyle, "text/css");
                        break;
                    }

                    if (page == Page::ENDSCREEN)
                    {
                        if (scorePlayer == 3)
                        {
                            respond(masterDefeat);
                        }
                        else
                        {
                            respond(masterVictory);
                        }
                        sleepTimerEnabled = true;
                    }
                    else if (page == Page::SOLUTION)
                    {
                        respond(masterSolution);
                    }
                    else if (page == Page::QUESTIONS)
                    {
                        respond(masterQuestions);
                    }
                    else if (page == Page::INDEX)
                    {
                        respond(masterIndex);
                    }
                    else
                    {
                        respond(masterError);
                    }
                    break;
                }
            }
        }

        client.stop();

        updateScoreLEDs();
        updateEndTimer();
    }
}