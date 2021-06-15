#include <Arduino.h>
#include <Motor.h>
#include <Transceiver.h>

enum Phase { DeepSleep, QuestionChoice, AnswerQuestion };

class Quiz {
   private:
    Motor* _motor;
    Transceiver* _transceiver;
    uint8_t _pinA;
    uint8_t _pinB;
    uint8_t _pinC;
    uint8_t _pinD;
    uint8_t* _scorePins;
    uint8_t _pinCorrect;
    uint8_t _pinWrong;
    bool _isAtStart;
    bool _isGameMaster;
    Phase _phase;
    uint8_t _score;
    uint8_t _opositeScore;
    uint8_t* _questions;
    uint8_t _currentQuestion;
    void deepSleep();
    void chooseQuestion();
    double calculateTime(double length);
    void answerQuestion();
    void rotateBack();

   public:
    Quiz(Motor* motor,
         Transceiver* transceiver,
         uint8_t pinA,
         uint8_t pinB,
         uint8_t pinC,
         uint8_t pinD,
         uint8_t pinScoreA,
         uint8_t pinScoreB,
         uint8_t pinScoreC,
         uint8_t pinCorrect,
         uint8_t pinWrong,
         bool isGameMaster);
    void wakeUp();
    void goSleep();
    void run();
};
