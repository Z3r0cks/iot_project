#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <Pages.h>
#include <Scripts.h>
#include <Styles.h>

// PIN LAYOUT
#define RXD 16
#define TXD 17
#define LED_CORRECT 22
#define LED_WRONG 23
#define LED_SCORE_1 33
#define LED_SCORE_2 32
#define LED_SCORE_3 27
#define TOUCH_PIN 2

// TOUCH
#define TOUCH_THRESHOLD 40

// ROUTES
#define ROUTE_INDEX_JS "GET /index.js"
#define ROUTE_SOLUTION_JS "GET /solution.js"
#define ROUTE_QUESTION_JS "GET /question.js"
#define ROUTE_SUBMIT_KEY "GET /submit-key"
#define ROUTE_VALIDATE_KEY "GET /validate-key"
#define ROUTE_AWAIT_QUESTION "GET /await-question"
#define ROUTE_SUBMIT_ANSWER "GET /submit-answer"
#define ROUTE_VALIDATE_ANSWER "GET /validate-answer"
#define ROUTE_QUIT_GAME "GET /quit-game"
#define ROUTE_STYLE_CSS "GET /style.css"

// DURATIONS
#define RESET_TIME 300000
#define SLEEP_TIME 10000

// PAGES
enum Page
{
    INDEX,
    SOLUTION,
    QUESTION,
    ENDSCREEN
};

// WIFI SETTINGS
const char *ssid = "Nature Quiz (Player)";
const char *password = NULL;

WiFiClient client;
WiFiServer server(80);

// GLOBAL VARS
Page page;
String key, validation, serial, question, answer, correct;
bool lockAnswer;
int scoreMaster, scorePlayer;

// TIMER VARS
unsigned long lastTime;
bool sleepTimerEnabled;
long sleepTimer;
unsigned long loopCount;

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
    Question("02_01", "2", "Von was ernähren sich Rehe von März bis April?", "Bärlappgewächse ", "Beeren", "Gräser und Knospen", "Zweikeimblättrige Kräute", "Gräser und Knospen von Mitte März bis Ende April. Laubtriebe und einkeimblättrige Kräuter von Anfang Mai bis Ende Juni. Zweikeimblättrige Kräuter und Laubtriebe von Mitte Juni bis Mitte Oktober. In dieser Zeit ist die Zahl der als Äsungspflanzen generell in Frage kommenden Arten am größten und umfasst rund 134 verschiedene Arten. Schachtelhalme, Farne und Bärlappgewächse sowie Knospen und Brombeeren von Mitte Oktober bis Mitte Dezember. Gräser, Knospen und Brombeeren von Anfang Januar bis Mitte März.", "C"),
    Question("03_01", "3", "Aus was entstand die Hochschule Furtwangen?", "Uhrmacherschule", "Baumschule", "Katholisches Kloster", "Volkshochschulkurs", "1850 wurde die Großherzogliche Badische Uhrmacherschule Furtwangen von Robert Gerwig gegründet. 1947 wurde diese zur Staatlichen Ingenieurschule Furtwangen/Schwarzwald. 1971 wurde diese zur Fachhochschule Furtwangen FHF. 1997 wurde sie zur Hochschule für Technik und Wissenschaft umfirmiert", "A"),
    Question("03_02", "3", "Auf welcher Höhe befindest du dich gerade", "~ 90m ü.n.N.", "~ 580m ü.n.N.", "~ 1100m ü.n.N.", "~ 2370m ü.n.N.", "m ü.n.N steht für Meter über normal Null", "C"),
    Question("03_03", "3", "Welche Sage gibt es über Studierende der Hochschule Furtwangen?", "Wenn mindestens drei mal in der Woche in der Mensa isst, besteht man seine Prüfungen", "Wenn Studierende während ihres Studiums das Uhrenmuseum besuchen, schaffen sie ihren Abschluss nicht.", "Studierende die im Wohnheim GHB wohnen, werden von den Professor:inn:en bevorzugt.", "Die Statue an der Brücke vor dem B-Bau der Hochschule, soll die erste weibliche Absolventin darstellen.", "Schon im ersten Semester wird den Studierenden beigebracht, dass sie erst nach ihrem bestandenen Abschluss, das Uhrenmuseum betreten sollten", "B"),
    Question("03_04", "3", "Wie viele Höhenmeter trennen den niedrigsten vom höchsten Punkt der Stadt Furtwangen?", "500m", "600m", "400m", "300m", "50m (Breg) - 1150m (Brend)", "D"),
    Question("04_01", "4", "Welche Berge kann man neben dem Feldberg und dem Kandel vom Brendturm aus sehen?", "Schauinsland", "Blickinstal", "Sehdasgrün", "Spührdasblau", "Der Brend ist mit 1148 m der höchste Berg der Stadt Furtwangen, auf dessen Spitze der Brendturm steht. Von seinem Aussichtspunkt aus  17 m Höhe, kann man den Schauinsland sehen. Dieser ist 1284 m hoch und gleichzeitig der Hausberg der Stadt Freiburg. Bei schönem Wetter kann man ringsherum in die schönen Täler schauen.", "A"),
    Question("04_02", "4", "Welche Eselsbrücke gibt es, um sie die Zuflüsse (Brigach und Breg) der Donau zu merken?", "Brigach und Breg bringen die Donau zu Weg", "Brigach von rechts, Breg von links daraus macht die Donau springs", "Zwei Mal B, das tut der Donau nicht weh", "Durch Brigach und Bregs Mündung, kommt die Donau zur Verkündung", "Länge der Flüsse und wo sie münden.", "A"),
    Question("04_03", "4", "Wie heißt der Größte Berg im Schwarzwald", "Mount Everest", "Zugspitze", "Feldberg", "Brend", "Der Feldberg ist der höchste Berg im Schwarzwald. Er ist 1493 Meter hoch. Mit der Seilbahn kann man fast bis zum Gipfel fahren.", "C"),
    Question("04_04", "4", "Wieso wurde die Linachtalsperre ursprünglich gebaut?", "Zum Schutz vor Überschwemmungen nach der Schneeschmelze", "Zur Stromgewinnung", "Damit ein Badesee entsteht", "Damit eine Örtliche Fischzucht entstehen kann", "Die Linachtalsperre wurde von 1922-1925 gebaut. bis 1969 wurde sie, wie ursprünglich geplant, zur örtlichen Stromgewinnung genutzt. Aus Sicherheitsgründen wurde 1988 das Wasser abgelassen. Seit ihrer Renovierung 2007 staut sie wieder das Wasser der Linach.", "B"),
};

String questionToJSON(Question quest)
{
    String json = "{\"id\":\"$ID\", \"category\":\"$CATEGORY\", \"text\":\"$TEXT\", \"answers\":[\"$ANSWER_A\",\"$ANSWER_B\",\"$ANSWER_C\",\"$ANSWER_D\"], \"explanation\":\"$EXPLANATION\"}";
    json.replace("$ID", quest.id);
    json.replace("$TEXT", quest.text);
    json.replace("$CATEGORY", quest.category);
    json.replace("$ANSWER_A", quest.answers[0]);
    json.replace("$ANSWER_B", quest.answers[1]);
    json.replace("$ANSWER_C", quest.answers[2]);
    json.replace("$ANSWER_D", quest.answers[3]);
    json.replace("$EXPLANATION", quest.explanation);
    return json;
}

String getQuestion(String id)
{
    for (const Question &quest : questions)
    {
        if (quest.id.equals(id))
        {
            correct = quest.correct;
            return questionToJSON(quest);
        }
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
    String html = String(playerPage);
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

// GAME LOGIC
void resetLogic()
{
    sleepTimerEnabled = false;
    sleepTimer = RESET_TIME;
    loopCount = 0;
    page = Page::INDEX;
    scoreMaster = 0;
    scorePlayer = 0;
    question = "";
    answer = "";
    correct = "";
    lockAnswer = false;
    digitalWrite(LED_CORRECT, LOW);
    digitalWrite(LED_WRONG, LOW);
}

// SLEEP TIMER
void updateSleepTimer()
{
    if (sleepTimerEnabled)
    {
        unsigned long time = millis();
        sleepTimer -= time - lastTime;
        lastTime = time;
        if (sleepTimer <= 0)
        {
            resetLogic();
            Serial.println("Going to sleep.");
            esp_deep_sleep_start();
        }
    }
}

void startSleepTimer(long timeout)
{
    sleepTimer = timeout;
    lastTime = millis();
    sleepTimerEnabled = true;
}

void startSleepTimer()
{
    startSleepTimer(RESET_TIME);
}

// PRINTERS
void printSleepTimer()
{
    Serial.print(sleepTimer / 1000);
    Serial.println(" seconds until deep sleep.");
}

// CALLBACK
void callback()
{
    // Do Nothing
}

// SETUP
void setup()
{
    // SETUP SERIAL
    Serial.begin(7200);
    Serial2.begin(9600, SERIAL_8N1, RXD, TXD);
    delay(1000);

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

    // SETUP DEEP SLEEP
    touchAttachInterrupt(TOUCH_PIN, callback, TOUCH_THRESHOLD);
    esp_sleep_enable_touchpad_wakeup();

    Serial.println("Player setup done.");
    Serial2.println("?reset=true");

    startSleepTimer();
    printSleepTimer();
}

// MAIN LOOP
void loop()
{
    // if (loopCount % 10000 == 0)
        // printSleepTimer();

    /*if (Serial.available())
    {
        serial = Serial.readString();
        Serial.println(serial);
    }*/

    if (Serial2.available())
    {
        serial = Serial2.readString();
        Serial.println(serial);
        startSleepTimer();
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
                startSleepTimer();
                String header = client.readString();
                if (header.endsWith("\n"))
                {
                    if (header.startsWith(ROUTE_INDEX_JS))
                    {
                        validation = randomKey(8);
                        String script = String(playerIndexJS);
                        script.replace("$VALIDATION", validation);
                        respond(script, "application/javascript");
                        break;
                    }
                    else if (header.startsWith(ROUTE_SOLUTION_JS))
                    {
                        validation = randomKey(8);
                        String script = String(playerSolutionJS);
                        script.replace("$VALIDATION", validation);
                        script.replace("$ANSWER", answer);
                        script.replace("$CORRECT", correct);
                        script.replace("$QUESTION", getQuestion(question));
                        respond(script, "application/javascript");
                        break;
                    }
                    else if (header.startsWith(ROUTE_QUESTION_JS))
                    {
                        validation = randomKey(8);
                        String script = String(playerQuestionJS);
                        script.replace("$VALIDATION", validation);
                        script.replace("$QUESTION", getQuestion(question));
                        respond(script, "application/javascript");
                        break;
                    }
                    else if (header.startsWith(ROUTE_SUBMIT_KEY))
                    {
                        int index = strlen(ROUTE_SUBMIT_KEY);
                        respond(validation, "text/plain");
                        String key = header.substring(index, index + 9);
                        Serial.println(key);
                        Serial2.println(key);
                        serial = "";
                        break;
                    }
                    else if (header.startsWith(ROUTE_VALIDATE_KEY))
                    {
                        if (serial.startsWith("?valid=true"))
                        {
                            respond(validation, "text/plain");
                            page = Page::SOLUTION;
                        }
                        else if (serial.startsWith("?valid=false"))
                        {
                            respond("INVALID", "text/plain");
                        }
                        else
                        {
                            respond("", "text/plain");
                        }
                        serial = "";
                        break;
                    }
                    else if (header.startsWith(ROUTE_AWAIT_QUESTION))
                    {
                        if (serial.startsWith("?question="))
                        {
                            respond(validation, "text/plain");
                            page = Page::QUESTION;
                            question = serial.substring(10, 15);
                        }
                        else
                        {
                            respond("", "text/plain");
                        }
                        serial = "";
                        break;
                    }
                    else if (header.startsWith(ROUTE_SUBMIT_ANSWER))
                    {
                        if (!lockAnswer)
                        {
                            lockAnswer = true;
                            int index = strlen(ROUTE_SUBMIT_ANSWER);
                            respond(validation, "text/plain");
                            String selectedAnswer = header.substring(index, index + 9);
                            answer = selectedAnswer.substring(8, 9);
                            Serial.println(selectedAnswer);
                            Serial2.println(selectedAnswer);
                            serial = "";
                        }
                        break;
                    }
                    else if (header.startsWith(ROUTE_VALIDATE_ANSWER))
                    {
                        if (serial.startsWith("?correct="))
                        {
                            if (serial.startsWith("?correct=true"))
                            {
                                digitalWrite(LED_CORRECT, HIGH);
                                scorePlayer++;
                            }
                            else if (serial.startsWith("?correct=false"))
                            {
                                digitalWrite(LED_WRONG, HIGH);
                                scoreMaster++;
                            }
                            respond(validation, "text/plain");
                            if (scorePlayer == 3 || scoreMaster == 3)
                                page = Page::ENDSCREEN;
                            else
                                page = Page::SOLUTION;
                        }
                        else
                        {
                            respond("", "text/plain");
                        }
                        serial = "";
                        break;
                    }
                    else if (header.startsWith(ROUTE_STYLE_CSS))
                    {
                        respond(playerStyle, "text/css");
                        break;
                    }

                    // PAGES
                    if (page == Page::ENDSCREEN)
                    {
                        if (scorePlayer == 3)
                        {
                            respond(getPage(playerVictory, "Quiz gewonnen", "victoryBody", ""));
                        }
                        else
                        {
                            respond(getPage(playerDefeat, "Quiz verloren", "defeatBody", ""));
                        }
                        startSleepTimer(SLEEP_TIME);
                    }
                    else if (page == Page::SOLUTION)
                    {
                        lockAnswer = false;
                        String title = "Auflösung";
                        if (question.isEmpty())
                            title = "Warte auf Frage";
                        respond(getPage(playerSolution, title, "solutionBody", "solution.js"));
                    }
                    else if (page == Page::QUESTION)
                    {
                        respond(getPage(playerQuestion, "Frage", "questionBody", "question.js"));
                        digitalWrite(LED_CORRECT, LOW);
                        digitalWrite(LED_WRONG, LOW);
                    }
                    else if (page == Page::INDEX)
                    {
                        respond(getPage(playerIndex, "Code", "indexBody", "index.js"));
                    }
                    else
                    {
                        error(getPage(playerError, "Fehler", "errorBody", ""));
                    }
                    break;
                }
            }
        }

        client.stop();
    }

    updateScoreLEDs();
    updateSleepTimer();

    // UPDATE LOOP COUNT
    loopCount++;
}