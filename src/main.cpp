#include <Arduino.h>

#include <esp_now.h>
#include <WiFi.h>

#include <SPIFFS.h>
#include <Pages.h>
#include <Scripts.h>
#include <Styles.h>

// PIN LAYOUT
#define LED_CORRECT 22
#define LED_WRONG 23
#define LED_SCORE_1 33
#define LED_SCORE_2 32
#define LED_SCORE_3 14 //old 27
#define TOUCH_PIN 2

// ROUTES QUIZ
#define ROUTE_INDEX_JS "GET /index.js"
#define ROUTE_QUESTIONS_JS "GET /questions.js"
#define ROUTE_SOLUTION_JS "GET /solution.js"
#define ROUTE_VALIDATE_KEY "GET /validate-key"
#define ROUTE_CHOOSE_QUESTION "GET /choose-question"
#define ROUTE_VALIDATE_ANSWER "GET /validate-answer"
#define ROUTE_STYLE_CSS "GET /style.css"

// ROUTES FORM
#define ROUTE_FORM "GET /suggest-question"
#define ROUTE_FORM_JS "GET /form.js"
#define ROUTE_FORM_SUBMIT "GET /submit-question"
#define ROUTE_FORM_GET "GET /download-questions"
#define ROUTE_DOWNLOAD_JS "GET /download.js"
#define ROUTE_FORM_CLEAR "GET /clear-questions"
#define ROUTE_FORM_CSS "GET /form.css"

// DURATIONS
#define RESET_TIME 300000
#define SLEEP_TIME 10000

// TOUCH
#define TOUCH_THRESHOLD 40

// FILE PATHS
const String QUESTIONS_JSON = "/questions.json";

// PAGES
enum Page
{
    INDEX,
    QUESTIONS,
    SOLUTION,
    ENDSCREEN
};

// GLOBAL VARS
Page page;
String key, validation, serial, question, download;
bool lockQuestion;
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

// WIFI SETTINGS
const char *ssid = "Nature Quiz (Game Master)";
const char *password = NULL;

WiFiClient client;
WiFiServer server(80);
String header;

// ESP NOW SETTINGS
uint8_t broadcastAddress[] = {0xAC, 0x67, 0xB2, 0x2D, 0x34, 0xB0};

typedef struct struct_message
{
    char message[32];
} struct_message;

struct_message sendData;
struct_message recieveData;

// ESP NOW CALLBACKS
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    if (status != ESP_NOW_SEND_SUCCESS)
        Serial.println("Error: Delivery failed.");
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
    memcpy(&recieveData, incomingData, sizeof(recieveData));
    Serial.print("<-- ");
    Serial.println(recieveData.message);
    serial = recieveData.message;
}

void sendMessage(String message)
{
    Serial.print("--> ");
    Serial.println(message);
    stpcpy(sendData.message, message.c_str());

    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&sendData, sizeof(sendData));
    if (result != ESP_OK)
        Serial.println("Error: Sending failed.");
}

const Question questions[] = {
    Question("01_01", "1", "Was macht den Schwarzwald so bienenfreundlich?", "Wegen der Höhenlage", "Wegen dem vielen Schnee", "Es wächst viel Löwenzahn in der Gegend", "Wegen der guten Luft", "Der Löwenzahn blüht von April bis Juni und hat auf die Entwicklung von Bienenvölkern großen Einfluss.", "C"),
    Question("01_02", "1", "Wieso verlieren Tannen im Winter nicht ihre Nadeln?", "Weil Tannen besser Wärme speichern können", "Weil die Nadeln eng beieinander liegen und sich dadurch gegenseitig wärmen", "Durch eine Art Salz, was als Frostschutzmittel dient", "Durch eine Art Zucker, was als Frostschutzmittel dient", "Durch Zucker gefriert das Wasser in den Zellen der Tannen nicht und die Nadeln müssen nicht abfallen. <br>Außerdem speichern Tannen weniger Wasser als Laubbäume, ihre Nadeln sind dicker und haben ein festeres Gewebe. Alles gemeinsam schützt vor dem Erfrieren.", "D"),
    Question("02_01", "2", "Von was ernähren sich Rehe in der Periode von März bis April?", "Bärlappgewächse ", "Beeren", "Gräser und Knospen", "Zweikeimblättrige Kräute", "Rehe fressen in fünf verschiedenen Äsungsperioden: <br> 1. Mitte März bis Ende April: Knospen und Gräser <br> 2. Mai bis Ende Juni: Einkeimblättrige Kräuter und Laubtriebe <br> 3. Juni bis Mitte Oktober: Zweikeimblättrige Kräuter <br> 4. Mitte Oktober bis Mitte Dezember: Farne, Bärlappgewächse, Schachtelhalme, <br> Brombeeren und verschiedene Knospen <br> 5. Überbrückung bis zur nächsten Äsung: Gräser, Brombeeren und Knospen", "C"),
    Question("02_01", "2", "Welcher Pilz ist nicht giftig?", "Kegelhütiger Knollenblätterpilz", "Pantherpilz", "Kastanienbrauner Schirmling", "Stockschwämmchen", "Das Stockschwämmchen wächst auf morschem Holz. Die Stieloberfläche ist beim Stockschwämmchen raufaserig bis feinschuppig aufgerissen und unterscheidet sich damit deutlich von der silbrig überfaserten Stiloberfläche beim Gift-Häubling.", "D"),
    Question("03_01", "3", "Aus was entstand die Hochschule Furtwangen?", "Uhrmacherschule", "Baumschule", "Katholisches Kloster", "Volkshochschulkurs", "1850 wurde die Großherzogliche Badische Uhrmacherschule Furtwangen von Robert Gerwig gegründet. <br> 1947 wurde diese zur Staatlichen Ingenieurschule Furtwangen/Schwarzwald. <br> 1971 wurde diese zur Fachhochschule Furtwangen FHF. <br> 1997 wurde sie zur Hochschule für Technik und Wissenschaft umfirmiert", "A"),
    Question("03_03", "3", "Welche Sage gibt es über Studierende der Hochschule Furtwangen?", "Wer mindestens drei mal in der Woche in der Mensa isst, besteht seine Prüfungen", "Wenn Studierende während ihres Studiums das Uhrenmuseum besuchen, schaffen sie ihren Abschluss nicht.", "Studierende die im Wohnheim GHB wohnen, werden von den Professor:inn:en bevorzugt.", "Die Statue an der Brücke vor dem B-Bau der Hochschule, soll die erste weibliche Absolventin darstellen.", "Schon im ersten Semester wird den Studierenden beigebracht, dass sie erst nach ihrem bestandenen Abschluss, das Uhrenmuseum betreten sollten, da sie sonst keinen Abschluss schaffen werden.", "B"),
    Question("03_04", "3", "Wie viele Höhenmeter trennen den niedrigsten vom höchsten Punkt der Stadt Furtwangen?", "ca 500m", "ca 600m", "ca 400m", "ca 300m", "Tiefster Punkt: 850 m (Allmendstraße bei REWE) <br><br>Höchster Punkt: 1148 m (Brend) <br><br> -> ca. 300 m", "D"),
    Question("04_01", "4", "Welche Berge kann man neben dem Feldberg und dem Kandel vom Brendturm aus sehen?", "Schauinsland", "Blickinstal", "Sehdasgrün", "Spührdasblau", "Der Brend ist mit 1148 m der höchste Berg der Stadt Furtwangen, auf dessen Spitze der Brendturm steht. Von seinem Aussichtspunkt (in 17 m Höhe), kann man den Schauinsland sehen. <br> Dieser ist 1284 m hoch und gleichzeitig der Hausberg der Stadt Freiburg. Bei schönem Wetter kann man ringsherum in die schönen Täler schauen.", "A"),
    Question("04_02", "4", "Welche Eselsbrücke gibt es, um sich die Zuflüsse der Donau (Brigach und Breg) zu merken?", "Brigach und Breg bringen die Donau zu Weg", "Brigach von rechts, Breg von links daraus macht die Donau springs", "Zwei Mal B, das tut der Donau nicht weh", "Durch Brigach und Bregs Mündung, kommt die Donau zur Verkündung", "Die Breg ist ist 48 km lang. <br> Die Brigach ist 40 km lang. <br> Sie fließen bei Donaueschingen zusammen und werden zur Donau.", "A"),
    Question("04_03", "4", "Wie heißt der Größte Berg im Schwarzwald?", "Mount Everest", "Zugspitze", "Feldberg", "Brend", "Der Feldberg ist der höchste Berg im Schwarzwald. Er ist 1493 Meter hoch. Mit der Seilbahn kann man fast bis zum Gipfel fahren.", "C"),
    Question("04_04", "4", "Wieso wurde die Linachtalsperre ursprünglich gebaut?", "Zum Schutz vor Überschwemmungen nach einer Schneeschmelze", "Zur Stromgewinnung", "Damit ein Badesee entsteht", "Damit eine Örtliche Fischzucht entstehen kann", "Die Linachtalsperre wurde von 1922-1925 gebaut. <br> Bis 1969 wurde sie, wie ursprünglich geplant, zur örtlichen Stromgewinnung genutzt. <br> Aus Sicherheitsgründen wurde 1988 das Wasser abgelassen. <br> Seit ihrer Renovierung 2007 staut sie wieder das Wasser der Linach.", "B"),
};

String getAnswer(String id)
{
    for (const Question &quest : questions)
    {
        if (quest.id.equals(id))
            return quest.correct;
    }
    return "";
}

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

// RANDOM KEY GENERATOR
unsigned long randomKey(int positions)
{
    unsigned long upper = pow(10, positions);
    unsigned long lower = pow(10, positions - 1);
    return (millis() % (upper - lower)) + lower;
}

// GET PAGE
String getPage(String raw, String title, String bodyClass, String script, String style)
{
    String html = String(masterPage);
    html.replace("$TITLE", title);
    if (!script.isEmpty())
        script = "<script src=\"" + script + "\" defer></script>";
    html.replace("$SCRIPT", script);
    html.replace("$STYLE", style);
    String page = String(raw);
    page.replace("$SCORE_PLAYER", String(scorePlayer));
    page.replace("$SCORE_MASTER", String(scoreMaster));
    page.replace("$CLASS", bodyClass);
    html.replace("$BODY", page);
    return html;
}

String getPage(String raw, String title, String bodyClass, String script)
{
    return getPage(raw, title, bodyClass, script, "style.css");
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
    sleepTimer = RESET_TIME;
    loopCount = 0;
    page = Page::INDEX;
    scoreMaster = 0;
    scorePlayer = 0;
    lockQuestion = false;
    digitalWrite(LED_CORRECT, LOW);
    digitalWrite(LED_WRONG, LOW);
}

// DEEP SLEEP
void deepSleep(String errorMessage)
{
    if (!errorMessage.isEmpty())
    {
        Serial.print("Error: ");
        Serial.println(errorMessage);
        delay(2500);
    }
    Serial.println("Going to sleep.");
    sendMessage("?sleep=true");
    delay(5000);
    esp_deep_sleep_start();
}

void deepSleep()
{
    deepSleep("");
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
            deepSleep();
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

void setup()
{
    // SERIAL MONITOR
    Serial.begin(7200);

    // SETUP PINS
    pinMode(LED_CORRECT, OUTPUT);
    pinMode(LED_WRONG, OUTPUT);
    pinMode(LED_SCORE_1, OUTPUT);
    pinMode(LED_SCORE_2, OUTPUT);
    pinMode(LED_SCORE_3, OUTPUT);

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // SETUP WIFI
    WiFi.mode(WIFI_STA);
    WiFi.softAP(ssid, password);
    IPAddress ip = WiFi.softAPIP();
    server.begin();
    Serial.print("Server running at ");
    Serial.print(ip);
    Serial.println(".");

    // SETUP SPIFFS
    if (!SPIFFS.begin(true))
        deepSleep("Failed to initialize SPIFFS.");

    // SETUP ESP-NOW
    if (esp_now_init() != ESP_OK)
        deepSleep("Failed to initialize ESP-NOW.");

    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);

    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    // ADD PEER LOOP
    byte retry = 10;
    while (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        if (retry == 0)
            deepSleep("Failed to add peer.");
        retry--;
        delay(1000);
    }

    // SETUP LOGIC
    resetLogic();
    updateScoreLEDs();

    // SETUP DEEP SLEEP
    touchAttachInterrupt(TOUCH_PIN, NULL, TOUCH_THRESHOLD);
    esp_sleep_enable_touchpad_wakeup();

    Serial.println("Master setup done.");
    sendMessage("?reset=true");

    startSleepTimer();
}

void loop()
{
    if (serial.startsWith("?reset=true"))
        resetLogic();
    else if (serial.startsWith("?sleep=true"))
    {
        resetLogic();
        deepSleep();
    }

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
                            sendMessage("?valid=true");
                            serial = "";
                        }
                        else if (!serial.isEmpty())
                        {
                            respond("", "text/plain");
                            sendMessage("?valid=false");
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
                            sendMessage(selectedQuestion);
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
                            sendMessage("?correct=" + correct);
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

                    // FORM ROUTES
                    else if (header.startsWith(ROUTE_FORM_JS))
                    {
                        validation = randomKey(8);
                        String script = String(formIndexJS);
                        script.replace("$VALIDATION", validation);
                        respond(script, "application/javascript");
                        break;
                    }
                    else if (header.startsWith(ROUTE_DOWNLOAD_JS))
                    {
                        validation = randomKey(8);
                        String script = String(formDownloadJS);
                        script.replace("$VALIDATION", validation);
                        script.replace("$DOWNLOAD", download);
                        respond(script, "application/javascript");
                        break;
                    }
                    else if (header.startsWith(ROUTE_FORM))
                    {
                        respond(getPage(formIndex, "Frage vorschlagen", "", "form.js", "form.css"));
                        break;
                    }
                    else if (header.startsWith(ROUTE_FORM_SUBMIT))
                    {
                        int start = header.indexOf("?data=") + 6;
                        int end = header.indexOf("&end=true");

                        String data = header.substring(start, end);
                        //Serial.println(data);

                        bool first = !SPIFFS.exists(QUESTIONS_JSON);
                        File file = SPIFFS.open(QUESTIONS_JSON, FILE_APPEND);

                        if (first)
                            file.print("%5B");
                        if (!first)
                            file.print("%2C");
                        file.print(data);
                        file.close();

                        respond(validation, "text/plain");
                        break;
                    }
                    else if (header.startsWith(ROUTE_FORM_GET))
                    {
                        download = "";
                        File file = SPIFFS.open(QUESTIONS_JSON);
                        while (file.available())
                            download += char(file.read());
                        file.close();
                        if (download.isEmpty())
                            download += "%5B";
                        download += "%5D";
                        respond(getPage("", "Fragen Download", "", "download.js", "form.css"));
                        break;
                    }
                    else if (header.startsWith(ROUTE_FORM_CLEAR))
                    {
                        SPIFFS.remove(QUESTIONS_JSON);
                        respond(validation, "text/plain");
                        break;
                    }
                    else if (header.startsWith(ROUTE_FORM_CSS))
                    {
                        respond(formStyle, "text/css");
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
                        startSleepTimer(SLEEP_TIME);
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