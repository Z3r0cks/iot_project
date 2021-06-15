#include <Arduino.h>

enum State { IDLE, SENDING, RECEIVING };

class Transceiver {
   private:
    byte _pin;
    byte _pout;
    State _state;
    bool _connection;
    byte _value;
    unsigned long _time;

   public:
    Transceiver(byte pin, byte pout);
    void send(byte data);
    void receive();
    bool isReady();
    byte value();
};