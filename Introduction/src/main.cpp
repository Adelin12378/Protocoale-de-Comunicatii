// Objectives:
// 1. presenting ATmega32p datasheet
// 2. pin configuration for in/out
// 3. read/write from pins
// 4. introduction in electrical workbench equipments
//  4.1. presentation + utilization multimeter (measure R, Vd)
//  4.2. presentation + visualization on oscilloscope
//  4.3. presentation programmable power supply

// Components
// 2 LEDs + resistors
// 1 Push Button + resistor

#include "Arduino.h"
#include "avr8-stub.h"
#include "app_api.h" // only needed with flash breakpoints
#include "laboratory_types.h"


void setup() {
    // initialize GDB stub
    debug_init();

    DDRD &= ~(1 << 2);  // PD2 is Input (Button)
    DDRD |= (1 << 3);  // PD3 is Output (LED0)
    DDRD |= (1 << 4);  // PD4 is Output (LED1)

    PORTD &= ~(1 << 3);
    PORTD &= ~(1 << 4);
}

void inline setLed0State(LedState state) {
    // TODO: 1. turn led0 on or off based on state
    // PIN2 -> PD2
    if (state == LedState::ON) {
        PORTD |= (1 << 3); // aprins
    } else {
        PORTD &= ~(1 << 3); // stins
    }
}

int buttonState = 0;
unsigned char btn = PIND & (1 << 2);

// read button state
ButtonState inline readBtnState() {
    // TODO: 1. return if button is pressed
    btn = PIND & (1 << 2);
    if (btn != 0) { // buton apasat
        return ButtonState::PRESSED; 
    } else {   
        return ButtonState::NOT_PRESSED;
    } 
}

void inline setLed1State(LedState state) {
    // TODO: 2. turn led1 on or off based on state
    // PIN2 -> PD2
    if (state == LedState::ON) {
        PORTD |= (1 << 4); // aprins
    } else {
        PORTD &= ~(1 << 4); // stins
    }
}

static void oscillatingLed() {
    // TODO: 2. toggle the led1 with the pattern: 500ms on, 200ms off
        static int cnt = 0;
        cnt++;
        if (cnt ==5) {
            setLed1State(LedState::ON);
        }
        if(cnt ==7) {
            setLed1State(LedState::OFF);
        }
        if(cnt == 7) {
            cnt = 0;
        }
    // TODO: 3. analyze on oscliisop the  led1 signal and measure the timing
}

// detect button press and toggle the led0 state
static void toggleLed(ButtonState btnState) {
    static bool lastState = ButtonState::NOT_PRESSED;
    static bool ledState = false;
    if ( lastState == ButtonState::NOT_PRESSED && 
            btnState == ButtonState::PRESSED ) {
        ledState = !ledState;
        setLed0State(ledState ? LedState::ON : LedState::OFF);
    }
    lastState = btnState;
}

void loop() {
    toggleLed(readBtnState());
    oscillatingLed();
    delay(100);
}