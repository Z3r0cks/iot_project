#include <Arduino.h>

class Transfer {
   private:
    uint8_t counter;

   public:
    void send();
    uint8_t receive();
};