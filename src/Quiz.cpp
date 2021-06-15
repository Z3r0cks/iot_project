#include <Quiz.h>

#define Threshold 150

// TODO: set correct time to rotate
#define QuestionHeight 0.10
#define RPM 5000
#define Radius 0.02

Quiz::Quiz(Motor* motor,
           Transceiver* transceiver,
           uint8_t pinA,  // up
           uint8_t pinB,  // back
           uint8_t pinC,  // ok
           uint8_t pinD,  // down
           uint8_t pinScoreA,
           uint8_t pinScoreB,
           uint8_t pinScoreC,
           uint8_t pinCorrect,
           uint8_t pinWrong,
           bool isGameMaster) {
    this->_motor = motor;
    this->_transceiver = transceiver;
    this->_pinA = pinA;
    this->_pinB = pinB;
    this->_pinC = pinC;
    this->_scorePins = new uint8_t[3]{pinScoreA, pinScoreB, pinScoreC};
    this->_pinCorrect = pinCorrect;
    this->_pinWrong = pinWrong;
    this->_pinD = pinD;
    this->_isGameMaster = isGameMaster;
    this->_isAtStart = true;
    this->_phase = Phase::DeepSleep;
    this->_score = 0;
    this->_opositeScore = 0;
    // TODO: set correct numbers for answers
    this->_questions = new uint8_t[13]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    this->_currentQuestion = 0;

    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    pinMode(pinC, INPUT);
    pinMode(pinD, INPUT);
};

void Quiz::deepSleep() {
    this->_transceiver->receive();
    if (this->_transceiver->isReady() && this->_transceiver->value() == 1) {
        this->_phase = Phase::QuestionChoice;
    };
}

void Quiz::rotateBack() {
    this->_motor->rotateLeft(
        this->calculateTime(QuestionHeight * _currentQuestion));
    this->_phase = Phase::QuestionChoice;
}

void Quiz::wakeUp() {
    this->_transceiver->send(1);
    this->_phase = Phase::QuestionChoice;
}

void Quiz::goSleep() {
    if (this->_score == 3 || this->_opositeScore == 3) {
        this->_phase = Phase::DeepSleep;
    }
}

double Quiz::calculateTime(double length) {
    return (60 * length) / (2 * PI * RPM * Radius);
}

void Quiz::chooseQuestion() {
    if (this->_isGameMaster) {
        if (touchRead(this->_pinA) > Threshold) {
            if (this->_currentQuestion > 0) {
                this->_motor->rotateLeft(this->calculateTime(QuestionHeight));
                _currentQuestion--;
            }
        } else if (touchRead(this->_pinC) > Threshold) {
            this->_transceiver->send(_currentQuestion);
            this->_phase = Phase::AnswerQuestion;
        } else if (touchRead(this->_pinD) > Threshold) {
            if (this->_currentQuestion < 12) {
                this->_motor->rotateRight(this->calculateTime(QuestionHeight));
                _currentQuestion++;
            }
        }
    } else {
        if (this->_transceiver->isReady() && this->_transceiver->value() > 0) {
            _currentQuestion = this->_transceiver->value();
            this->_phase = Phase::AnswerQuestion;
            this->_motor->rotateRight(
                this->calculateTime(QuestionHeight * _currentQuestion));
        }
    }
}

void Quiz::answerQuestion() {
    if (!this->_isGameMaster) {
        uint8_t pressedButton = 0;
        if (touchRead(this->_pinA) > Threshold) {
            pressedButton = 1;
        } else if (touchRead(this->_pinB) > Threshold) {
            pressedButton = 2;
        } else if (touchRead(this->_pinC) > Threshold) {
            pressedButton = 3;
        } else if (touchRead(this->_pinD) > Threshold) {
            pressedButton = 4;
        }
        this->_transceiver->send(pressedButton);
        if (pressedButton == this->_questions[_currentQuestion]) {
            this->_score++;
            for (uint8_t i = 0; i < this->_score; i++) {
                digitalWrite(this->_scorePins[i], HIGH);
            }
            digitalWrite(this->_pinCorrect, HIGH);
            delay(2000);
            digitalWrite(this->_pinCorrect, LOW);
        } else {
            this->_opositeScore++;
            digitalWrite(this->_pinWrong, HIGH);
            delay(2000);
            digitalWrite(this->_pinWrong, LOW);
        }
        this->rotateBack();
        this->goSleep();

    } else {
        if (this->_transceiver->isReady() && this->_transceiver->value() > 0) {
            if (this->_questions[this->_currentQuestion] ==
                this->_transceiver->value()) {
                this->_opositeScore++;
                digitalWrite(this->_pinWrong, HIGH);
                delay(2000);
                digitalWrite(this->_pinWrong, LOW);
            } else {
                this->_score++;
                for (uint8_t i = 0; i < this->_score; i++) {
                    digitalWrite(this->_scorePins[i], HIGH);
                }
                digitalWrite(this->_pinCorrect, HIGH);
                delay(2000);
                digitalWrite(this->_pinCorrect, LOW);
            }
            this->rotateBack();
            this->goSleep();
        }
    }
}

void Quiz::run() {
    switch (this->_phase) {
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
