#include <Arduino.h>
#include <Motor.h>

#define MOTOR_A 27
#define MOTOR_B 26
#define MOTOR_V 25
#define BUTTON_A 15
#define BUTTON_B 2
#define BUTTON_C 0
#define BUTTON_D 4
#define PIN_DATA 19

Motor* motor;

void setup() {
    Serial.begin(7200);
    pinMode(MOTOR_A, OUTPUT);
    pinMode(MOTOR_B, OUTPUT);
    pinMode(MOTOR_V, OUTPUT);
    motor = new Motor(MOTOR_A, MOTOR_B, MOTOR_V);
    motor->setBrake(true);
    // motor->rotateRight(2500);
    Serial.println("Setup DONE");
}

bool clockwise = true;

void loop() {
    // unsigned int time = calculateTime(10, 2500, 0.02) * 1000;
    // Serial.println((unsigned long)time);

    unsigned int time = 2500;

    if (clockwise) {
        motor->rotateRight(time);
    } else {
        motor->rotateLeft(time);
    }
    motor->run();

    if (!motor->isRotating()) {
        clockwise = !clockwise;
        motor->setBrake(true);
    }
}