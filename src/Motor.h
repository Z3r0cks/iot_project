#include <Arduino.h>

class Motor {
   private:
    bool _braking;
    int8_t _rotating;
    unsigned long _runTime;
    unsigned long _lastTime;

    byte _motorA;
    byte _motorB;
    byte _motorV;

    void time();
    unsigned long diff();
    void rotateLeft();
    void rotateRight();
    void rotate(unsigned long time, bool right);
    void rotate(byte inputA, byte inputB, byte inputV);
    void stop();
    void brake();

   public:
    Motor(byte motorA, byte motorB, byte motorV);
    void rotateLeft(unsigned long time);
    void rotateRight(unsigned long time);
    void run();
    int8_t isRotating();
    void setBrake(bool brake);
};