#include <Transceiver.h>

Transceiver::Transceiver(byte pin, byte pout) {
    this->_pin = pin;
    this->_pout = pout;
    this->_state = State::IDLE;
    this->_connection = false;
    this->_value = 0;
    this->_time = 0;
    pinMode(pin, INPUT);
    pinMode(pout, OUTPUT);
}

void Transceiver::send(byte data) {
    if (this->_state == State::IDLE) {
        Serial.println("IDLE");
        digitalWrite(this->_pout, HIGH);
        this->_state = State::SENDING;
    } else if (this->_state == State::SENDING) {
        if (!this->_connection && digitalRead(this->_pin) == HIGH) {
            Serial.print(", CONN");
            this->_connection = true;
            digitalWrite(this->_pout, LOW);
        } else if (this->_connection && this->_value < data) {
            if (digitalRead(this->_pin) == LOW) {
                Serial.print(", SEND");
                digitalWrite(this->_pout, HIGH);
            } else {
                Serial.print(", ACK");
                digitalWrite(this->_pout, LOW);
                this->_value++;
            }
        } else if (this->_connection) {
            Serial.println(", DISC");
            this->_connection = false;
            this->_value = 0;
        }
    }
}

void Transceiver::receive() {
    if (this->_state == State::IDLE && digitalRead(this->_pin) == HIGH) {
        Serial.println("CONN");
        digitalWrite(this->_pout, HIGH);
        this->_state = State::RECEIVING;
        this->_connection = true;
        digitalWrite(this->_pout, LOW);
    } else if (this->_state == State::RECEIVING) {
        if (this->_connection) {
            if (digitalRead(this->_pin) == HIGH) {
                Serial.print(", INC");
                this->_value++;
                digitalWrite(this->_pout, HIGH);
                this->_time = millis();
            } else {
                Serial.print(", ACK");
                digitalWrite(this->_pout, LOW);
                if (millis() - this->_time > 2500) {
                    Serial.println(", TOUT");
                    this->_connection = false;
                }
            }
        }
    }
}

bool Transceiver::isReady() {
    return !this->_connection && this->_value > 0;
}

byte Transceiver::value() {
    byte value = this->_value;
    this->_value = 0;
    return value;
}