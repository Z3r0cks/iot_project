#include <Motor.h>

#define MOTOR_BRAKE_TIME 90

Motor::Motor(uint8_t motorA, uint8_t motorB, uint8_t motorV) {
    this->_motorA = motorA;
    this->_motorB = motorB;
    this->_motorV = motorV;

    this->_braking = false;
    this->_rotating = 0;
    this->_runTime = millis();
    this->_lastTime = millis();
}

void Motor::time() {
    this->_lastTime = millis();
}

unsigned long Motor::diff() {
    unsigned long time = millis();
    return time - this->_lastTime;
}

void Motor::rotate(uint8_t inputA, uint8_t inputB, uint8_t inputV) {
    digitalWrite(this->_motorA, inputA);
    digitalWrite(this->_motorB, inputB);
    digitalWrite(this->_motorV, inputV);
}

void Motor::rotate(unsigned long time, bool right) {
    if (this->_rotating == 0) {
        this->_runTime = time;
        this->time();
        if (right) {
            this->rotateRight();
        } else {
            this->rotateLeft();
        }
    }
}

void Motor::rotateLeft(unsigned long time) {
    this->rotate(time, false);
}

void Motor::rotateRight(unsigned long time) {
    this->rotate(time, true);
}

void Motor::rotateLeft() {
    this->rotate(255, 0, 255);
    this->_rotating = -1;
    Serial.println("LEFT");
}

void Motor::rotateRight() {
    this->rotate(0, 255, 255);
    this->_rotating = 1;
    Serial.println("RITE");
}

void Motor::stop() {
    digitalWrite(this->_motorV, LOW);
    this->_rotating = 0;
}

void Motor::brake() {
    if (this->_rotating < 0) {
        this->_rotating = 0;
        this->rotateRight(MOTOR_BRAKE_TIME);
    } else if (this->_rotating > 0) {
        this->_rotating = 0;
        this->rotateLeft(MOTOR_BRAKE_TIME);
    }
    this->_braking = false;
}

void Motor::run() {
    if (this->diff() > this->_runTime) {
        if (this->_braking) {
            this->brake();
            Serial.println("BRAKE");
        } else {
            this->stop();
            Serial.println("STOP");
        }
    }
}

int8_t Motor::isRotating() {
    return this->_rotating;
}

void Motor::setBrake(bool brake) {
    this->_braking = brake;
}