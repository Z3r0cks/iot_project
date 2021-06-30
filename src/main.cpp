#include "Arduino.h"
#include <SPIFFS.h>
#include <WiFi.h>
#include <Pages.h>
#include <Scripts.h>
#include <Styles.h>

#define RXD 16
#define TXD 17
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

const char *ssid = "Nature Quiz (Game Master)";
const char *password = NULL;

WiFiClient client;
WiFiServer server(80);

Page page;
String key, validation, serial, question;
bool lockQuestion;

int scoreMaster, scorePlayer;

unsigned long lastTime;
bool sleepTimerEnabled;
long sleepTimer;

struct Question
{
    String id;
    String category;
    String text;
    String *answers;
    String explanation;
    String correct;
    Question(String id, String category, String text, String a, String b, String c, String d, String explanation, String correct)
    {
        this->id = id;
        this->category = category;
        this->text = text;
        this->answers = new String[4]{a, b, c, d};
        this->explanation = explanation;
        this->correct = correct;
    }
};

const Question questions[] = {
    Question("01_01", "1", "Warum ist die Banane krumm?", "Weil Baum.", "Weil Sonne.", "Weil keine Ahnung.", "Weil so.", "Die Banane wächst zur Sonne. Deshalb.", "B"),
    Question("01_02", "1", "01_02?", "A", "B", "C", "D", "Erklärung hier einfügen", "A"),
    Question("01_03", "1", "01_03?", "A", "B", "C", "D", "Erklärung hier einfügen", "D"),
    Question("01_04", "1", "01_04?", "A", "B", "C", "D", "Erklärung hier einfügen", "C"),
    Question("02_01", "2", "02_01?", "A", "B", "C", "D", "Erklärung hier einfügen", "D"),
    Question("02_02", "2", "02_02?", "A", "B", "C", "D", "Erklärung hier einfügen", "A"),
    Question("02_03", "2", "02_03?", "A", "B", "C", "D", "Erklärung hier einfügen", "B"),
    Question("02_04", "2", "02_04?", "A", "B", "C", "D", "Erklärung hier einfügen", "D"),
    Question("03_01", "3", "03_01?", "A", "B", "C", "D", "Erklärung hier einfügen", "D"),
    Question("03_02", "3", "03_02?", "A", "B", "C", "D", "Erklärung hier einfügen", "C"),
    Question("03_03", "3", "03_03?", "A", "B", "C", "D", "Erklärung hier einfügen", "B"),
    Question("03_04", "3", "03_04?", "A", "B", "C", "D", "Erklärung hier einfügen", "A"),
    Question("04_01", "4", "04_01?", "A", "B", "C", "D", "Erklärung hier einfügen", "C"),
    Question("04_02", "4", "04_02?", "A", "B", "C", "D", "Erklärung hier einfügen", "A"),
    Question("04_03", "4", "04_03?", "A", "B", "C", "D", "Erklärung hier einfügen", "D"),
    Question("04_04", "4", "04_04?", "A", "B", "C", "D", "Erklärung hier einfügen", "B")};

String questionToJSON(Question quest)
{
    String json = "{\"id\":\"$ID\", \"category\":\"$CATEGORY\", \"text\":\"$TEXT\", \"answers\":[\"$ANSWER_A\",\"$ANSWER_B\",\"$ANSWER_C\",\"$ANSWER_D\"], \"explanation\":\"$EXPLANATION\", \"correct\":\"$CORRECT\"}";
    json.replace("$ID", quest.id);
    json.replace("$TEXT", quest.text);
    json.replace("$CATEGORY", quest.category);
    json.replace("$ANSWER_A", quest.answers[0]);
    json.replace("$ANSWER_B", quest.answers[1]);
    json.replace("$ANSWER_C", quest.answers[2]);
    json.replace("$ANSWER_D", quest.answers[3]);
    json.replace("$EXPLANATION", quest.explanation);
    json.replace("$CORRECT", quest.correct);
    return json;
}

String getAnswer(String id)
{
    for (const Question &quest : questions)
    {
        if (quest.id.equals(id))
            return quest.correct;
    }
    return "";
}

String questionsToJSON()
{
    String json = "{";
    for (const Question &quest : questions)
        json += "\"" + quest.id + "\":" + questionToJSON(quest) + ",";
    return json + "}";
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
        client.println(data);
}

void respond(String html)
{
    respond(html, "text/html");
}

String getPage(String raw, String title, String script)
{
    String html = String(masterPage);
    html.replace("$TITLE", title);
    if (!script.isEmpty())
        script = "<script src=\"" + script + "\" defer></script>";
    html.replace("$SCRIPT", script);
    String page = String(raw);
    page.replace("$SCORE_PLAYER", String(scorePlayer));
    page.replace("$SCORE_MASTER", String(scoreMaster));
    html.replace("$BODY", page);
    return html;
}

void error(String html)
{
    client.println("HTTP/1.1 404 Not Found");
    client.println("Content-type: text/html");
    client.println("Connection: close");
    client.println();
    if (!html.isEmpty())
        client.println(html);
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

void updateSleepTimer()
{
    if (sleepTimerEnabled)
    {
        unsigned long time = millis();
        sleepTimer -= time - lastTime;
        lastTime = time;
        if (sleepTimer <= 0)
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

void startSleepTimer()
{
    lastTime = millis();
    sleepTimerEnabled = true;
}

void setup()
{
    Serial.begin(7200);
    Serial2.begin(9600, SERIAL_8N1, RXD, TXD);
    pinMode(LED_CORRECT, OUTPUT);
    pinMode(LED_WRONG, OUTPUT);
    pinMode(LED_SCORE_1, OUTPUT);
    pinMode(LED_SCORE_2, OUTPUT);
    pinMode(LED_SCORE_3, OUTPUT);
    setupWiFi();
    sleepTimerEnabled = false;
    sleepTimer = endTime;
    page = Page::INDEX;
    scoreMaster = 0;
    scorePlayer = 0;
    lockQuestion = false;
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

    /*if (Serial2.available())
    {
        serial = Serial2.readString();
        Serial.println(serial);
    }*/

    client = server.available();

    if (client)
    {
        while (client.connected())
        {
            if (client.available())
            {
                String header = client.readString();
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
                        script.replace("$QUESTIONS", questionsToJSON());
                        respond(script, "application/javascript");
                        break;
                    }
                    else if (header.startsWith(ROUTE_SOLUTION_JS))
                    {
                        validation = randomKey(8);
                        String script = String(masterSolutionJS);
                        script.replace("$ID", question);
                        script.replace("$VALIDATION", validation);
                        script.replace("$QUESTIONS", questionsToJSON());
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
                            Serial2.println("?valid=true");
                            serial = "";
                        }
                        else if (!serial.isEmpty())
                        {
                            respond("", "text/plain");
                            Serial.println("?valid=false");
                            Serial2.println("?valid=false");
                            serial = "";
                        }
                        else
                        {
                            respond("", "text/plain");
                        }
                        break;
                    }
                    else if (header.startsWith(ROUTE_CHOOSE_QUESTION))
                    {
                        if (!lockQuestion)
                        {
                            lockQuestion = true;
                            int index = strlen(ROUTE_CHOOSE_QUESTION);
                            respond(validation, "text/plain");
                            page = Page::SOLUTION;
                            String selectedQuestion = header.substring(index, index + 15);
                            question = selectedQuestion.substring(10, 15);
                            Serial.println(selectedQuestion);
                            Serial2.println(selectedQuestion);
                            serial = "";
                            digitalWrite(LED_CORRECT, LOW);
                            digitalWrite(LED_WRONG, LOW);
                            break;
                        }
                    }
                    else if (header.startsWith(ROUTE_VALIDATE_ANSWER))
                    {
                        if (serial.startsWith("?answer="))
                        {
                            int index = serial.length();
                            respond(validation, "text/plain");
                            String selectedAnswer = serial.substring(index - 1, index);
                            String correctAnswer = getAnswer(question);
                            if (selectedAnswer.equals(correctAnswer))
                            {
                                digitalWrite(LED_WRONG, HIGH);
                                scorePlayer++;
                            }
                            else
                            {
                                digitalWrite(LED_CORRECT, HIGH);
                                scoreMaster++;
                            }
                            Serial.println("?correct=" + correctAnswer);
                            Serial2.println("?correct=" + correctAnswer);
                            serial = "";
                            if (scoreMaster == 3 || scorePlayer == 3)
                                page = Page::ENDSCREEN;
                            else
                                page = Page::QUESTIONS;
                        }
                        else
                        {
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
                            respond(getPage(masterDefeat, "Niederlage", ""));
                        }
                        else
                        {
                            respond(getPage(masterVictory, "Niederlage", ""));
                        }
                        startSleepTimer();
                    }
                    else if (page == Page::SOLUTION)
                    {
                        lockQuestion = false;
                        respond(getPage(masterSolution, "Auflösung", "solution.js"));
                    }
                    else if (page == Page::QUESTIONS)
                    {
                        respond(getPage(masterQuestions, "Fragenauswahl", "questions.js"));
                    }
                    else if (page == Page::INDEX)
                    {
                        respond(getPage(masterIndex, "Code", "index.js"));
                    }
                    else
                    {
                        error(getPage(masterError, "Fehler", ""));
                    }
                    break;
                }
            }
        }

        client.stop();
    }

    updateScoreLEDs();
    updateSleepTimer();
}