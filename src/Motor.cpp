#include <Motor.h>

#define MOTOR_BRAKE_TIME 90

Motor::Motor(uint8_t motorA, uint8_t motorB, uint8_t motorV) {
    this->motorA = motorA;
    this->motorB = motorB;
    this->motorV = motorV;

    this->braking = false;
    this->rotating = 0;
    this->runTime = millis();
    this->lastTime = millis();
}

void Motor::time() {
    this->lastTime = millis();
}

unsigned long Motor::diff() {
    unsigned long time = millis();
    return time - this->lastTime;
}

void Motor::rotate(uint8_t a, uint8_t b, uint8_t v) {
    digitalWrite(this->motorA, a);
    digitalWrite(this->motorB, b);
    digitalWrite(this->motorV, v);
}

void Motor::rotate(unsigned long time, bool right) {
    if (this->rotating == 0) {
        this->runTime = time;
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
    this->rotating = -1;
    Serial.println("LEFT");
}

void Motor::rotateRight() {
    this->rotate(0, 255, 255);
    this->rotating = 1;
    Serial.println("RITE");
}

void Motor::stop() {
    digitalWrite(this->motorV, LOW);
    this->rotating = 0;
}

void Motor::brake() {
    if (this->rotating < 0) {
        this->rotating = 0;
        this->rotateRight(MOTOR_BRAKE_TIME);
    } else if (this->rotating > 0) {
        this->rotating = 0;
        this->rotateLeft(MOTOR_BRAKE_TIME);
    }
    this->braking = false;
}

void Motor::run() {
    if (this->diff() > this->runTime) {
        if (this->braking) {
            this->brake();
            Serial.println("BRAKE");
        } else {
            this->stop();
            Serial.println("STOP");
        }
    }
}

int8_t Motor::isRotating() {
    return this->rotating;
}

void Motor::setBrake(bool brake) {
    this->braking = brake;
}