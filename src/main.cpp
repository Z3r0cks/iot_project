#include <Arduino.h>
#include <Quiz.h>

#define MOTOR_A 27
#define MOTOR_B 26
#define MOTOR_V 25
#define BUTTON_A 15
#define BUTTON_B 2
#define BUTTON_C 0
#define BUTTON_D 4
#define LED_CORRECT 16
#define LED_WRONG 17
#define LED_SCORE_A 32
#define LED_SCORE_B 33
#define LED_SCORE_C 34
#define PIN_DATA_IN 18
#define PIN_DATA_OUT 19

Motor* motor;
Transceiver* transceiver;
Quiz* quiz;

void setup() {
    Serial.begin(7200);
    pinMode(MOTOR_A, OUTPUT);
    pinMode(MOTOR_B, OUTPUT);
    pinMode(MOTOR_V, OUTPUT);
    motor = new Motor(MOTOR_A, MOTOR_B, MOTOR_V);
    motor->setBrake(true);
    transceiver = new Transceiver(PIN_DATA_IN, PIN_DATA_OUT);
    quiz = new Quiz(motor, transceiver, BUTTON_A, BUTTON_B, BUTTON_C, BUTTON_D,
                    LED_SCORE_A, LED_SCORE_B, LED_SCORE_C, LED_CORRECT,
                    LED_WRONG, true);
    Serial.println("Setup DONE");
}

bool clockwise = true;

void loop() {
    motor->run();
    quiz->run();
}