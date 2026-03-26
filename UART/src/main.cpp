// Objectives:
// 1. configure UART on ATmega328p
// 2. write bytes on UART
// 3. receive bytes on UART
// 4. text protocols

// Components
// 2 LEDs + resistors
// 1 Push Button + resistor

#include "Arduino.h"
#include <Uart.h>


Uart uart;

enum ButtonState {
    PRESSED,
    NOT_PRESSED
};

enum LedState {
    ON,
    OFF
};

int BUTTON_PIN = 0;
unsigned char btn = PIND & (1 << 2);

ButtonState inline readBtnState() {
    btn = PIND & (1 << 2);
    if (btn != 0) { // buton apasat
        return ButtonState::PRESSED;
        uart.writeString("Buton apasat lol\n");
    } else {   
        return ButtonState::NOT_PRESSED;
    } 
}


void inline setLed0State(LedState state) {
    // PIN3 -> PD3
    if (state == LedState::ON) {
        PORTD |= (1 << 3); // aprins
    } else {
        PORTD &= ~(1 << 3); // stins
    }
}

void inline setLed1State(LedState state) {
    // PIN4 -> PD4
    if (state == LedState::ON) {
        PORTD |= (1 << 4); // aprins
    } else {
        PORTD &= ~(1 << 4); // stins
    }
}

void setup() {
    DDRD &= ~(1 << 2);  // PD2 is Input (Button)
    DDRD |= (1 << 3);  // PD3 is Output (LED0)
    DDRD |= (1 << 4);  // PD4 is Output (LED1)

    PORTD &= ~(1 << 3);
    PORTD &= ~(1 << 4);

    uart.init();
    uart.writeByte('a');

    DDRB |= (1 << 1); // OC1A pin as output

}

void responseBack(const char* userMsg) {
    uart.writeString("You said: ");
    uart.writeString(userMsg);
    uart.writeString("\n");
}

char buffer[1024];
int index = 0;

void loop() {    
    if (uart.available()) {
        char c = uart.readByte();
        if (c == '\n') {
            buffer[index] = '\0';
            if (strcmp(buffer, "ledon") == 0) {
                setLed0State(LedState::OFF);
            } else if (strcmp(buffer, "ledoff") == 0) {
                setLed0State(LedState::ON);
            } else if (strcmp(buffer, "oscledon") == 0) {
                TCCR1A = (0b10<<0) | (0b11<<6);
                TCCR1B = (0b11<<3) | (0b101<<0);
                ICR1 = 10937;
                OCR1A = 7812;
            } else if (strcmp(buffer, "oscledoff") == 0) {
                TCCR1A = 0;
                TCCR1B = 0;
                ICR1 = 0;
                OCR1A = 0;
                PORTB &= ~(1 << 1);
            }
            responseBack(buffer);
            index = 0;
        } else if (c != '\r') {
            buffer[index++] = c;
        }
    }
}