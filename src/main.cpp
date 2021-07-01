#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <Pages.h>
#include <Scripts.h>
#include <Styles.h>

// PIN LAYOUT
#define RXD 16
#define TXD 17
#define LED_CORRECT 19
#define LED_WRONG 18
#define LED_SCORE_1 25
#define LED_SCORE_2 26
#define LED_SCORE_3 27

// ROUTES
#define ROUTE_INDEX_JS "GET /index.js"
#define ROUTE_QUESTIONS_JS "GET /questions.js"
#define ROUTE_SOLUTION_JS "GET /solution.js"
#define ROUTE_VALIDATE_KEY "GET /validate-key"
#define ROUTE_CHOOSE_QUESTION "GET /choose-question"
#define ROUTE_VALIDATE_ANSWER "GET /validate-answer"
#define ROUTE_QUIT_GAME "GET /quit-game"
#define ROUTE_STYLE_CSS "GET /style.css"

// DURATIONS
#define resetTime 300000
#define endTime 10000

// PAGES
enum Page
{
    INDEX,
    QUESTIONS,
    SOLUTION,
    ENDSCREEN
};

// WIFI SETTINGS
const char *ssid = "Nature Quiz (Game Master)";
const char *password = NULL;

WiFiClient client;
WiFiServer server(80);

// GLOBAL VARS
Page page;
String key, validation, serial, question;
bool lockQuestion;
int scoreMaster, scorePlayer;

// TIMER VARS
unsigned long lastTime0;
bool resetTimerEnabled;
long resetTimer;

unsigned long lastTime1;
bool sleepTimerEnabled;
long sleepTimer;

// QUESTIONS
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
    Question("01_01", "1", "Warum ist der Schwarzwald so Bienenfreundlich?", "Wegen der Höhenlage", "Wegen dem vielen Schnee", "Es wächst viel Löwenzahn in der Gegend", "Wegen der guten Luft", "Löwenzahn besitzen nicht nur eine Blüte sondern viele. Bienen werden davon angelockt.", "C"),
    Question("01_02", "1", "Wieso verlieren Tannen im Winter nicht ihre Nadeln?", "Weil Tannen besser Wärme speichern können", "Weil die Nadeln eng beieinander liegen und sich dadurch gegenseitig wärmen", "Durch eine Art Salz, was als Frostschutzmittel dient", "Durch eine Art Zucker, was als Frostschutzmittel dient", "Tannen speichen außerdem weniger Wasser und die Nadeln sind dicker. Bei Laubblätter haben kaum festes Gewebe und würden im Winter deshalb erfrieren.", "D"),
    Question("02_01", "1", "Von was ernähren sich Rehe von März bis April?", "Bärlappgewächse ", "Beeren", "Gräser und Knospen", "Zweikeimblättrige Kräute", "Gräser und Knospen von Mitte März bis Ende April. Laubtriebe und einkeimblättrige Kräuter von Anfang Mai bis Ende Juni. Zweikeimblättrige Kräuter und Laubtriebe von Mitte Juni bis Mitte Oktober. In dieser Zeit ist die Zahl der als Äsungspflanzen generell in Frage kommenden Arten am größten und umfasst rund 134 verschiedene Arten. Schachtelhalme, Farne und Bärlappgewächse sowie Knospen und Brombeeren von Mitte Oktober bis Mitte Dezember. Gräser, Knospen und Brombeeren von Anfang Januar bis Mitte März.", "C"),
    Question("03_01", "1", "Aus was entstand die Hochschule Furtwangen?", "Uhrmacherschule", "Baumschule", "Katholisches Kloster", "Volkshochschulkurs", "1850 wurde die Großherzogliche Badische Uhrmacherschule Furtwangen von Robert Gerwig gegründet. 1947 wurde diese zur Staatlichen Ingenieurschule Furtwangen/Schwarzwald. 1971 wurde diese zur Fachhochschule Furtwangen FHF. 1997 wurde sie zur Hochschule für Technik und Wissenschaft umfirmiert", "A"),
    Question("03_02", "2", "Auf welcher Höhe befindest du dich gerade", "~ 90m ü.n.N.", "~ 580m ü.n.N.", "~ 1100m ü.n.N.", "~ 2370m ü.n.N.", "m ü.n.N steht für Meter über normal Null", "C"),
    Question("03_03", "2", "Welche Sage gibt es über Studierende der Hochschule Furtwangen?", "Wenn mindestens drei mal in der Woche in der Mensa isst, besteht man seine Prüfungen", "Wenn Studierende während ihres Studiums das Uhrenmuseum besuchen, schaffen sie ihren Abschluss nicht.", "Studierende die im Wohnheim GHB wohnen, werden von den Professor:inn:en bevorzugt.", "Die Statue an der Brücke vor dem B-Bau der Hochschule, soll die erste weibliche Absolventin darstellen.", "Schon im ersten Semester wird den Studierenden beigebracht, dass sie erst nach ihrem bestandenen Abschluss, das Uhrenmuseum betreten sollten", "B"),
    Question("03_04", "2", "Wie viele Höhenmeter trennen den niedrigsten vom höchsten Punkt der Stadt Furtwangen?", "500m", "600m", "400m", "300m", "50m (Breg) - 1150m (Brend)", "D"),
    Question("04_01", "2", "Welche Berge kann man neben dem Feldberg und dem Kandel vom Brendturm aus sehen?", "Schauinsland", "Blickinstal", "Sehdasgrün", "Spührdasblau", "Der Brend ist mit 1148 m der höchste Berg der Stadt Furtwangen, auf dessen Spitze der Brendturm steht. Von seinem Aussichtspunkt aus  17 m Höhe, kann man den Schauinsland sehen. Dieser ist 1284 m hoch und gleichzeitig der Hausberg der Stadt Freiburg. Bei schönem Wetter kann man ringsherum in die schönen Täler schauen.", "A"),
    Question("04_02", "3", "Welche Eselsbrücke gibt es, um sie die Zuflüsse (Brigach und Breg) der Donau zu merken?", "Brigach und Breg bringen die Donau zu Weg", "Brigach von rechts, Breg von links daraus macht die Donau springs", "Zwei Mal B, das tut der Donau nicht weh", "Durch Brigach und Bregs Mündung, kommt die Donau zur Verkündung", "Länge der Flüsse und wo sie münden.", "A"),
    Question("04_03", "3", "Wie heißt der Größte Berg im Schwarzwald", "Mount Everest", "Zugspitze", "Feldberg", "Brend", "Der Feldberg ist der höchste Berg im Schwarzwald. Er ist 1493 Meter hoch. Mit der Seilbahn kann man fast bis zum Gipfel fahren.", "C"),
    Question("04_04", "3", "Wieso wurde die Linachtalsperre ursprünglich gebaut?", "Zum Schutz vor Überschwemmungen nach der Schneeschmelze", "Zur Stromgewinnung", "Damit ein Badesee entsteht", "Damit eine Örtliche Fischzucht entstehen kann", "Die Linachtalsperre wurde von 1922-1925 gebaut. bis 1969 wurde sie, wie ursprünglich geplant, zur örtlichen Stromgewinnung genutzt. Aus Sicherheitsgründen wurde 1988 das Wasser abgelassen. Seit ihrer Renovierung 2007 staut sie wieder das Wasser der Linach.", "B"),
};

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

String questionsToJSON()
{
    String json = "{";
    for (const Question &quest : questions)
        json += "\"" + quest.id + "\":" + questionToJSON(quest) + ",";
    return json + "}";
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

// HTTP RESPONSES
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

void error(String html)
{
    client.println("HTTP/1.1 404 Not Found");
    client.println("Content-type: text/html");
    client.println("Connection: close");
    client.println();
    if (!html.isEmpty())
        client.println(html);
}

// RANDOM KEY GENERATOR
unsigned long randomKey(int positions)
{
    unsigned long upper = pow(10, positions);
    unsigned long lower = pow(10, positions - 1);
    return (millis() % (upper - lower)) + lower;
}

// GET PAGE
String getPage(String raw, String title, String bodyClass, String script)
{
    String html = String(masterPage);
    html.replace("$TITLE", title);
    if (!script.isEmpty())
        script = "<script src=\"" + script + "\" defer></script>";
    html.replace("$SCRIPT", script);
    String page = String(raw);
    page.replace("$SCORE_PLAYER", String(scorePlayer));
    page.replace("$SCORE_MASTER", String(scoreMaster));
    page.replace("$CLASS", bodyClass);
    html.replace("$BODY", page);
    return html;
}

// SCORE LEDS
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

// GAME LOGIC
void resetLogic()
{
    sleepTimerEnabled = false;
    sleepTimer = endTime;
    page = Page::INDEX;
    scoreMaster = 0;
    scorePlayer = 0;
    lockQuestion = false;
    digitalWrite(LED_CORRECT, LOW);
    digitalWrite(LED_WRONG, LOW);
}

// RESET TIMER
void updateResetTimer()
{
    if (resetTimerEnabled)
    {
        unsigned long time = millis();
        resetTimer -= time - lastTime0;
        lastTime0 = time;
        if (resetTimer <= 0)
            resetLogic();
    }
}

// SLEEP TIMER
void updateSleepTimer()
{
    if (sleepTimerEnabled)
    {
        unsigned long time = millis();
        sleepTimer -= time - lastTime1;
        lastTime1 = time;
        if (sleepTimer <= 0)
            resetLogic();
    }
}

void startResetTimer()
{
    lastTime0 = millis();
    resetTimerEnabled = true;
}

void stopResetTimer()
{
    resetTimerEnabled = false;
}

void startSleepTimer()
{
    lastTime1 = millis();
    sleepTimerEnabled = true;
}

// SETUP
void setup()
{
    // SETUP SERIAL
    Serial.begin(7200);
    Serial2.begin(9600, SERIAL_8N1, RXD, TXD);

    // SETUP PINS
    pinMode(LED_CORRECT, OUTPUT);
    pinMode(LED_WRONG, OUTPUT);
    pinMode(LED_SCORE_1, OUTPUT);
    pinMode(LED_SCORE_2, OUTPUT);
    pinMode(LED_SCORE_3, OUTPUT);

    // SETUP WIFI
    WiFi.softAP(ssid, password);
    IPAddress ip = WiFi.softAPIP();
    server.begin();
    Serial.print("Server running at ");
    Serial.print(ip);
    Serial.println(".");

    // SETUP LOGIC
    resetLogic();
    updateScoreLEDs();

    Serial.println("Master setup done.");
    Serial2.println("?reset=true");
}

// MAIN LOOP
void loop()
{
    /*if (Serial.available())
    {
        serial = Serial.readString();
        Serial.println(serial);
    }*/

    if (Serial2.available())
    {
        serial = Serial2.readString();
        Serial.println(serial);
        startResetTimer();
    }

    if (serial.startsWith("?reset=true"))
        resetLogic();

    client = server.available();

    if (client)
    {
        while (client.connected())
        {
            if (client.available())
            {
                startResetTimer();
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
                            respond(validation, "text/plain");
                            String selectedAnswer = serial.substring(8, 9);
                            String correctAnswer = getAnswer(question);
                            String correct = "";
                            if (selectedAnswer.equals(correctAnswer))
                            {
                                digitalWrite(LED_WRONG, HIGH);
                                scorePlayer++;
                                correct = "true";
                            }
                            else
                            {
                                digitalWrite(LED_CORRECT, HIGH);
                                scoreMaster++;
                                correct = "false";
                            }
                            Serial.println("?correct=" + correct);
                            Serial2.println("?correct=" + correct);
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

                    // PAGES
                    if (page == Page::ENDSCREEN)
                    {
                        if (scorePlayer == 3)
                        {
                            respond(getPage(masterDefeat, "Quiz verloren", "defeatBody", ""));
                        }
                        else
                        {
                            respond(getPage(masterVictory, "Quiz gewonnen", "victoryBody", ""));
                        }
                        stopResetTimer();
                        startSleepTimer();
                    }
                    else if (page == Page::SOLUTION)
                    {
                        lockQuestion = false;
                        respond(getPage(masterSolution, "Erklärung", "solutionBody", "solution.js"));
                    }
                    else if (page == Page::QUESTIONS)
                    {
                        respond(getPage(masterQuestions, "Frage wählen", "questionsBody", "questions.js"));
                    }
                    else if (page == Page::INDEX)
                    {
                        respond(getPage(masterIndex, "Code", "indexBody", "index.js"));
                    }
                    else
                    {
                        error(getPage(masterError, "Fehler", "errorBody", ""));
                    }
                    break;
                }
            }
        }

        client.stop();
    }

    updateScoreLEDs();
    updateResetTimer();
    updateSleepTimer();
}