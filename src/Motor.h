#include <Arduino.h>

class Motor {
   private:
    bool braking;
    int8_t rotating;
    unsigned long runTime;
    unsigned long lastTime;

    byte motorA;
    byte motorB;
    byte motorV;

    void time();
    unsigned long diff();
    void rotateLeft();
    void rotateRight();
    void rotate(unsigned long time, bool right);
    void rotate(byte a, byte b, byte v);
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