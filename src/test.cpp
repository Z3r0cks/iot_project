#include <Arduino.h>

enum State { IDLE, SENDING, RECEIVING };

class Transceiver {
   private:
    byte _pin;
    byte _pout;
    State _state;
    bool _connection;

    void print(String message) {
        Serial.print(message);
        Serial.print(", state: ");
        Serial.print(this->_state);
        Serial.print(", conn: ");
        Serial.print(this->_connection);
        Serial.print(", pin: ");
        Serial.print(digitalRead(this->_pin));
        Serial.print(", pout: ");
        Serial.println(digitalRead(this->_pout));
    }

   public:
    Transceiver(byte pin, byte pout) {
        this->_pin = pin;
        this->_pout = pout;
        this->_state = State::IDLE;
        this->_connection = false;
        pinMode(pin, INPUT);
        pinMode(pout, OUTPUT);
    }

    void send(byte data) {
        if (this->_state == State::IDLE) {
            digitalWrite(this->_pout, HIGH);
            this->_state = State::SENDING;
            Serial.println("SENDING...");
        } else if (this->_state == State::SENDING) {
            if (!this->_connection && digitalRead(this->_pin) == HIGH) {
                this->_connection = true;
                Serial.println("Full connection established!");
            }
        }
    }

    byte receive() {
        Serial.println(this->_state);
        if (this->_state == State::IDLE && digitalRead(this->_pin) == HIGH) {
            digitalWrite(this->_pout, HIGH);
            this->_state = State::RECEIVING;
            this->_connection = true;
            Serial.println("RECEIVING...");
        } else if (this->_state == State::RECEIVING) {
            Serial.println("I am alive!");
        }
        delay(100);
        return 0;
    }
};

struct Pin {
   private:
    byte _number;
    byte _last;

   public:
    Pin(byte number) {
        this->_number = number;
        this->_last = number;
        pinMode(number, INPUT);
    }

    void save() { this->_last = this->value(); }
    byte value() { return touchRead(this->_number); }
    bool changed() { return this->_last != this->value(); }
};

Pin* touch;
Transceiver* transceiver;

// RECEIVER

void setup() {
    Serial.begin(7200);
    // touch = new Pin[4]{Pin(15), Pin(2), Pin(0), Pin(4)};

    transceiver = new Transceiver(35, 34);
}

void loop() {
    transceiver->receive();

    /*for (byte i = 0; i < sizeof(touch); i++) {
        if (touch[i].changed()) {
            Serial.print(touch[i].value());
            Serial.print(" ");
        }
    }
    Serial.println();*/
    delay(1000);
}