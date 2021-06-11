#include <Arduino.h>
#include <Motor.h>

enum Phase { DeepSleep, QuestionChoice, AnswerQuestion };

class Quiz {
   private:
    Motor* motor;
    uint8_t pinA;
    uint8_t pinB;
    uint8_t pinC;
    uint8_t pinD;
    uint8_t pinDataIn;
    uint8_t pinDataOut;
    uint8_t* scorePins;
    uint8_t pinCorrect;
    uint8_t pinWrong;
    bool isAtStart;
    bool isGameMaster;
    Phase phase;
    uint8_t score;
    uint8_t opositeScore;
    uint8_t* questions;
    uint8_t currentQuestion;
    void deepSleep();
    void chooseQuestion();
    double calculateTime(double length);
    void answerQuestion();
    void rotateBack();

    Quiz(Motor* motor,
         uint8_t pinA,
         uint8_t pinB,
         uint8_t pinC,
         uint8_t pinD,
         uint8_t pinScoreA,
         uint8_t pinScoreB,
         uint8_t pinScoreC,
         uint8_t pinCorrect,
         uint8_t pinWrong,
         uint8_t pinDataIn,
         uint8_t pinDataOut,
         bool isGameMaster);
    void wakeUp();
    void goSleep();
    void run();
};

/* Motor A: 27
Motor B: 26
Motor V: 25
Button A: 15
Button B: 2
Button C: 0
Button D:4
LED Korrekt: 16
LED Falsch: 17
LED Score A: 32
LED Score B: 33
LED Score C: 34
PIN DATA IN:  18
PIN DATA OUT: 19
Batterie +: 5V
Batterie -: GND */
