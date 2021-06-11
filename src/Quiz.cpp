#include <Quiz.h>
#include <Transfer.h>

#define Threshold 150

// TODO: set correct time to rotate
#define QuestionHeight 0.10
#define RPM 5000
#define Radius 0.02

Quiz::Quiz(Motor* motor,
           uint8_t pinA,  // up
           uint8_t pinB,  // back
           uint8_t pinC,  // ok
           uint8_t pinD,  // down
           uint8_t pinDataIn,
           uint8_t pinDataOut,
           uint8_t pinScoreA,
           uint8_t pinScoreB,
           uint8_t pinScoreC,
           uint8_t pinCorrect,
           uint8_t pinWrong,
           bool isGameMaster) {
    this->motor = motor;
    this->pinA = pinA;
    this->pinB = pinB;
    this->pinC = pinC;
    this->scorePins = new uint8_t[3]{pinScoreA, pinScoreB, pinScoreC};
    this->pinCorrect = pinCorrect;
    this->pinWrong = pinWrong;
    this->pinD = pinD;
    this->pinDataIn = pinDataIn;
    this->pinDataOut = pinDataOut;
    this->isGameMaster = isGameMaster;
    this->isAtStart = true;
    this->phase = Phase::DeepSleep;
    this->score = 0;
    this->opositeScore = 0;
    // TODO: set correct numbers for answers
    this->questions = new uint8_t[13]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    this->currentQuestion = 0;

    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    pinMode(pinC, INPUT);
    pinMode(pinD, INPUT);
    pinMode(pinDataIn, INPUT);
    pinMode(pinDataOut, OUTPUT);
};

void Quiz::deepSleep() {
    if (digitalRead(pinDataIn) == 1) {
        this->phase = Phase::QuestionChoice;
    };
}

void Quiz::rotateBack() {
    this->motor->rotateLeft(
        this->calculateTime(QuestionHeight * currentQuestion));
    this->phase = Phase::QuestionChoice;
}

void Quiz::wakeUp() {
    digitalWrite(pinDataOut, 1);
    this->phase = Phase::QuestionChoice;
}

void Quiz::goSleep() {
    if (this->score == 3 || this->opositeScore == 3) {
        this->phase = Phase::DeepSleep;
    }
}

double Quiz::calculateTime(double length) {
    return (60 * length) / (2 * PI * RPM * Radius);
}

void Quiz::chooseQuestion() {
    if (this->isGameMaster) {
        if (touchRead(this->pinA) > Threshold) {
            if (this->currentQuestion > 0) {
                this->motor->rotateLeft(this->calculateTime(QuestionHeight));
                currentQuestion--;
            }
        } else if (touchRead(this->pinC) > Threshold) {
            digitalWrite(this->pinDataOut, currentQuestion);
            this->phase = Phase::AnswerQuestion;
        } else if (touchRead(this->pinD) > Threshold) {
            if (this->currentQuestion < 12) {
                this->motor->rotateRight(this->calculateTime(QuestionHeight));
                currentQuestion++;
            }
        }
    } else {
        if (digitalRead(this->pinDataIn) > 0) {
            currentQuestion = digitalRead(this->pinDataIn);
            this->phase = Phase::AnswerQuestion;
            this->motor->rotateRight(
                this->calculateTime(QuestionHeight * currentQuestion));
        }
    }
}

void Quiz::answerQuestion() {
    if (!this->isGameMaster) {
        uint8_t pressedButton = 0;
        if (touchRead(this->pinA) > Threshold) {
            pressedButton = 1;
        } else if (touchRead(this->pinB) > Threshold) {
            pressedButton = 2;
        } else if (touchRead(this->pinC) > Threshold) {
            pressedButton = 3;
        } else if (touchRead(this->pinD) > Threshold) {
            pressedButton = 4;
        }
        digitalWrite(this->pinDataOut, pressedButton);
        if (pressedButton == this->questions[currentQuestion]) {
            this->score++;
            for (uint8_t i = 0; i < this->score; i++) {
                digitalWrite(this->scorePins[i], HIGH);
            }
            digitalWrite(this->pinCorrect, HIGH);
            delay(2000);
            digitalWrite(this->pinCorrect, LOW);
        } else {
            this->opositeScore++;
            digitalWrite(this->pinWrong, HIGH);
            delay(2000);
            digitalWrite(this->pinWrong, LOW);
        }
        this->rotateBack();
        this->goSleep();

    } else {
        if (digitalRead(this->pinDataIn) > 0) {
            if (this->questions[this->currentQuestion] ==
                digitalRead(this->pinDataIn)) {
                this->opositeScore++;
                digitalWrite(this->pinWrong, HIGH);
                delay(2000);
                digitalWrite(this->pinWrong, LOW);
            } else {
                this->score++;
                for (uint8_t i = 0; i < this->score; i++) {
                    digitalWrite(this->scorePins[i], HIGH);
                }
                digitalWrite(this->pinCorrect, HIGH);
                delay(2000);
                digitalWrite(this->pinCorrect, LOW);
            }
            this->rotateBack();
            this->goSleep();
        }
    }
}

void Quiz::run() {
    switch (this->phase) {
        case (Phase::DeepSleep):
            this->deepSleep();
            break;
        case (Phase::QuestionChoice):
            this->chooseQuestion();
            break;
        case (Phase::AnswerQuestion):
            this->answerQuestion();
            break;
    }
}
